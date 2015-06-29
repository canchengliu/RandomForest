#include "RandomForest.h"

extern unsigned int MAX_CLASS_COUNT; // the total number of class

void readTrainFile(const char* filename, vvd& dataTable) {
	ifstream inputFile;												// Input file stream
	string singleInstance;											// Single line read from the input file 
	inputFile.open(filename);
	if (!inputFile) {												// If input file does not exist, print error and exit
		cerr << "Error: Training data file not found!" << endl;
		exit(-1);
	}
	getline(inputFile, singleInstance);								// Drop the first line of  attribute name
	while (getline(inputFile, singleInstance)) {					// Read from file, parse and store data
		parse(linefilter(singleInstance), dataTable);
	}
	//printAttributeTable(dataTable);
	inputFile.clear();
	inputFile.close();
}

void readTestFile(const char* filename, vvd& dataTable) {
	ifstream inputFile;												// Input file stream
	string singleInstance;											// Single line read from the input file 
	inputFile.open(filename); 										// Open test file
	if (!inputFile) { 												// Exit if test file is not found
		cerr << "Error: Testing data file not found!" << endl;
		exit(-1);
	}
	getline(inputFile, singleInstance);								// Drop the first line of  attribute name
	while (getline(inputFile, singleInstance)) { 					// Store test data in a table
		parse(linefilter(singleInstance), dataTable);
	}
	inputFile.clear();
	inputFile.close(); 												// Close input file
}

/*
* Decision tree training phase
* In this phase, the training data is read
* from the file and stored into a vvs using
* the parse() function. The generateTableInfo()
* function extracts the attribute (column) names
* and also the values that each column can take.
* This information is also stored in a vvs.
* buildDecisionTree() function recursively
* builds trains the decision tree.
*/
int _train(vvd& dataTable, node* &root) {
	//node* root = *proot;
	vvsb tableInfo = generateTableInfo(dataTable);					// Stores all the attributes and their values in a vector of vector of strings named tableInfo
	root = new node;											// Declare and assign memory for the root node of the Decision Tree
	root->deepth = 0;
	root = buildDecisionTree(dataTable, root, tableInfo);			// Recursively build and train decision tree
	int defaultClass = returnMostFrequentClass(dataTable);			// Stores the most frequent class in the training data. This is used as the default class label
	return defaultClass;
}

/*
* Decision tree testing phase
* In this phase, the testing is read
* from the file, parsed and stored.
* Each row in the table is made to
* traverse down the decision tree
* till a class label is found.
*/
vi _test(vvd& dataTable, node* &root, int defaultClass) {
	vi predictedClassLabels;										// Stores the predicted class labels for each row
	int predictedLabel;
	for (size_t i = 0; i < dataTable.size(); i++) {						// Predict class labels based on the decision tree
		predictedLabel = testDataOnDecisionTree(dataTable[i], root, defaultClass);
		predictedClassLabels.push_back(predictedLabel);
	}
	return predictedClassLabels;
}

void printResult(vi& predictedClassLabels, vvd& testingData) {
	vi givenClassLabels;											// Stores the given class labels in the test data
	int lastCol = testingData[0].size() - 1;
	for (size_t i = 0; i < testingData.size(); i++) {						// Store given class labels in vector of strings named givenClassLabels
		int label = (int)testingData[i][lastCol];
		givenClassLabels.push_back(label);
	}
	/* Print output */
	ofstream outputFile;
	outputFile.open("test_decisionTreeOutput.txt", ios::app);
	outputFile << endl << "--------------------------------------------------" << endl;
	double accuracy = printPredictionsAndCalculateAccuracy(givenClassLabels, predictedClassLabels);			// calculate accuracy of classification
	cout << "\nAccuracy: " << accuracy << endl;
	outputFile << "Accuracy of decision tree classifier = " << accuracy << "%"; 							// Print out accuracy to console
	outputFile.close();
}

void printKaggleResult(vi& predictedClassLabels) {
	ofstream outputFile("kaggel_submit.txt");
	for (size_t st = 0; st < predictedClassLabels.size(); st++) {
		outputFile << st + 1 << ',' << predictedClassLabels[st] << '\n';
	}
	outputFile.close();
}

vi decisionTree(vvd& trainingData, vvd& testingData) {
	node* root;
	int defaultClass = _train(trainingData, root);
	return _test(testingData, root, defaultClass);
}

int _findMax(int a[], int n) {
	if (n <= 0) return -1;
	int max = a[0], maxIndex = 0;
	for (int i = 1; i < n; i++) {
		if (a[i] > max) {
			max = a[i];
			maxIndex = i;
		}
	}
	return maxIndex;
}

void allRandomForest(vvi& allPredictedClassLabels, vvd& testingData) { //int numTrees, vvd& trainingData, vvd& testingData) {
	//set: int numClasses=2, int maxDepth=4, int maxBins=32
	vi mostPredictedClassLabels; //Store all the most frequent predicted label of the random foret to be the final predicted class label
	int mostLabel[MAX_CLASS_COUNT + 1]; // 
	memset(mostLabel, 0, (MAX_CLASS_COUNT + 1) * sizeof(int));

	int predictedLabel = 0; // Get the most frequent predicted label of the random foret to be the final predicted class label

	for (size_t column = 0; column < allPredictedClassLabels[0].size(); column++) {
		for (size_t tree = 0; tree < allPredictedClassLabels.size(); tree++) {
			mostLabel[allPredictedClassLabels[tree][column]]++;
		}
		predictedLabel = _findMax(mostLabel, MAX_CLASS_COUNT);
		mostPredictedClassLabels.push_back(predictedLabel);
		memset(mostLabel, 0, (MAX_CLASS_COUNT + 1) * sizeof(int));
	}
	printResult(mostPredictedClassLabels, testingData);
	printKaggleResult(mostPredictedClassLabels);
}

void randomForest(int numTrees, vvd& trainingData, vvd& testingData, vvi& predictedClassLabels) {
	const double SELECT_PERCENT = 0.8;
	int treeTrainDataSize = (int)(trainingData.size() * SELECT_PERCENT);
	vvd tmpTrainingData;
	for (int i = 0; i < numTrees; i++) {
		random_shuffle(trainingData.begin(), trainingData.end());
		tmpTrainingData.assign(trainingData.begin(), trainingData.begin() + treeTrainDataSize);
		vi tmp = decisionTree(tmpTrainingData, testingData);
		predictedClassLabels.push_back(tmp);
		tmpTrainingData.clear();
	}
}










