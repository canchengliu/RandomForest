#include "rfmpi.h"

int MAX_BIN;
unsigned int MAX_CLASS_COUNT; // the total number of class
int MAX_DEEPTH;

int main(int argc, char ** argv) {
	int is_initialized = 0;
	unsigned int numTrees;
	vvd trainingData;
	vvd testingData;
	readTrainFile("data/Nonlinear/train.csv", trainingData);
	readTestFile("data/Nonlinear/train100.csv", testingData);							// Input data in the form of a vector of vector of strings

	// Determine if MPI has been initialized already.
	MPI_Initialized(&is_initialized);
	if (!is_initialized) {
		// Initialize MPI.
		MPI_Init(&argc, &argv);

		// Determine if this it the master process or a child.
		int rank = 0;
		int processes = 0;
		char name[MPI_MAX_PROCESSOR_NAME];
		int name_size = 0;

		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &processes);
		MPI_Get_processor_name(name, &name_size);
		cout << "Thread Process: " << processes << " Rank: " << rank << " Name: " << name << endl;
		// Check enough arguments ?
		if (argc != 6) {
			if ( rank == 0 ) {
				cout
					<< "Usage: mpirun -n [0] rfmpi [1] [2] [3] [4] [5]\n"
					<< "	[0] - Number of threads to run\n"
					<< "	[1] - Trees per forest handle by each child thread\n"
					<< "	[2] - Number of different class label\n"
					<< "	[3] - Split keys per node\n"
					<< "	[4] - Maximun deepth of each tree and the deepth of root is 0\n"
					<< "	[5] - Training set sample probability (%)" << endl;
					//mpirun -n 4 ./rfmpi 10 26 3 4 70
			}
		} else {
			//Extract parameters.
			numTrees = atoi(argv[1]);
			MAX_CLASS_COUNT = atoi(argv[2]); // the total number of class
			MAX_BIN = atoi(argv[3]);
			MAX_DEEPTH = atoi(argv[4]);

			if (rank == processes - 1) {
				double sample_probability = atof(argv[5]);

				if ( sample_probability < 1.0 ) {
					sample_probability = 1.0;
				}
				if ( sample_probability > 100.0 ) {
					sample_probability = 100.0;
				}
				execute_main(processes, sample_probability / 100.0, trainingData, testingData, numTrees);
			}
			else {
				execute_child(processes - 1, rank, testingData, numTrees);
			}
		}
	}
	// Finalize MPI.
	MPI_Finalize();
	return 0;
}


