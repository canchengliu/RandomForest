#include "rfmpi.h"


void execute_main(const int process_count, const double sample_probability, vvd& trainingData, vvd& testingData, unsigned int numTrees) {
	unsigned int child_process_count = process_count - 1;

	cout << "Master: Child Process: " << child_process_count << ", Number of Trees:"<< numTrees * child_process_count << ", Sample Probability: " << sample_probability << "%" << endl;
	int sample_probability_int = static_cast<int>(RAND_MAX * sample_probability);

	// Buffers.
	unsigned int col_count = trainingData[0].size();
	double row_buffer[col_count];
	cout << "Master: Sending training data to children..." << endl;
	for (size_t i = 0; i < trainingData.size(); i++) {
		copy(trainingData[i].begin(), trainingData[i].end(), row_buffer);
		// Send to child.
		for ( unsigned int child_rank = 0; child_rank < child_process_count; ++child_rank ) {
			if ( rand() < sample_probability_int ) {
				MPI_Send(
					&row_buffer,
					col_count,
					MPI_DOUBLE,
					child_rank,
					MessageTag::RowBuffer,
					MPI_COMM_WORLD);
			}
		}
	}
	// Stop the loading.
	// Sending stop signal for stoping sending training data to children
	row_buffer[0] = numeric_limits<double>::quiet_NaN();
	for (unsigned int child_rank = 0; child_rank < child_process_count; ++child_rank ) {
		// Send to child.
		MPI_Send(
			&row_buffer,
			col_count,
			MPI_DOUBLE,
			child_rank,
			MessageTag::RowBuffer,
			MPI_COMM_WORLD );
	}
	cout << "Master: Sending finished." << endl;

	// Grab the trees.
	MPI_Status status;
	vvi allPredictedClassLabels; // Store all the predicted class label result of the randomforest
	vi predictedClassLabels;
	unsigned int row_count = testingData.size();
	unsigned int buffer_size = row_count * numTrees;
	int label_buffer[buffer_size];
	cout << "Master: Waiting for result of predicted class labels from children." << endl;
	for ( unsigned int child_rank = 0; child_rank < child_process_count; ++child_rank ) {
		cout << "Master: Waiting on slave " << child_rank << "..." << endl;
		// Wait on child.
		MPI_Recv(
			&label_buffer,
			buffer_size,
			MPI_INT,
			child_rank,
			MessageTag::TreeFinished,
			MPI_COMM_WORLD,
			&status );

		for (size_t i = 0; i < numTrees; i++) {
			predictedClassLabels.assign(label_buffer + i * row_count, label_buffer + (i + 1) * row_count);
			allPredictedClassLabels.push_back(predictedClassLabels);
			predictedClassLabels.clear();
		}
	}
	cout << "Master: Loaded all forests ! Classifying..." << endl;
	// Load testing data.
	allRandomForest(allPredictedClassLabels, testingData);
	cout << "Master: Finished!\n" << endl;
}

void execute_child(const unsigned int parent_rank, const unsigned int rank, vvd& testingData, unsigned int numTrees) {
	cout << testingData.size() << endl;
	unsigned int col_count = testingData[0].size();

	MPI_Status status;
	//cout << "Slave [" << rank << "] online: [BD: " << bootstrap_divisor << ", SK: " << split_keys_per_node << ", TPF: " << trees_per_forest << "]" << endl;

	cout << "Slave [" << rank << "]: Receiving the trainingData" << endl;

	double buffer[col_count];
	vd data;
	buffer[0] = 0.0;
	vvd trainingData;

	while ( true ) {
		MPI_Recv(
			&buffer,
			col_count,
			MPI_DOUBLE,
			parent_rank,
			MessageTag::RowBuffer,
			MPI_COMM_WORLD,
			&status );

		// Stop on NaN.
		if ( buffer[0] != buffer[0] ) {
			break;
		}
		// Add elements to data vector.
		data.assign(buffer, buffer + col_count);
		trainingData.push_back(data);
		data.clear();
	}
	cout << "Slave [" << rank << "]: Growing tree..." << endl;
	vvi predictedClassLabels;
	randomForest(numTrees, trainingData, testingData, predictedClassLabels);
	unsigned int row_count = predictedClassLabels[0].size();
	unsigned int buffer_size = numTrees * row_count;
	int label_buffer[buffer_size];
	for (size_t i = 0; i < numTrees; i++) {
		copy(predictedClassLabels[i].begin(), predictedClassLabels[i].end(), label_buffer + i * row_count);
	}
	// Send finished signal.

	MPI_Send(
		&label_buffer,
		buffer_size,
		MPI_INT,
		parent_rank,
		MessageTag::TreeFinished,
		MPI_COMM_WORLD);

	cout << "Slave [" << rank << "]: Finished!" << endl;
}

