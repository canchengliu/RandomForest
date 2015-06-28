#include "header.h"


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
int train(vvd& dataTable, node* &root) {
	//node* root = *proot;
	vvsb tableInfo = generateTableInfo(dataTable);					// Stores all the attributes and their values in a vector of vector of strings named tableInfo
	cout << "*t1";
	root = new node;											// Declare and assign memory for the root node of the Decision Tree
	root->deepth = 0;
	cout << "*t2";
	root = buildDecisionTree(dataTable, root, tableInfo);			// Recursively build and train decision tree
	cout << "*t3";
	int defaultClass = returnMostFrequentClass(dataTable);			// Stores the most frequent class in the training data. This is used as the default class label
	cout << "*t4";
	return defaultClass;
}

int _train(vvd& dataTable, node* &root) {
	//node* root = *proot;
	vvsb tableInfo = generateTableInfo(dataTable);					// Stores all the attributes and their values in a vector of vector of strings named tableInfo
	cout << "*t1";
	root = new node;											// Declare and assign memory for the root node of the Decision Tree
	root->deepth = 0;
	cout << "*t2";
	root = buildDecisionTree(dataTable, root, tableInfo);			// Recursively build and train decision tree
	cout << "*t3";
	int defaultClass = returnMostFrequentClass(dataTable);			// Stores the most frequent class in the training data. This is used as the default class label
	cout << "*t4";
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
double test(vvd& dataTable, node* &root, int defaultClass) {
	//node* root = *proot;
	vi predictedClassLabels;										// Stores the predicted class labels for each row
	vi givenClassLabels;											// Stores the given class labels in the test data
	int lastCol = dataTable[0].size()-1;
	for (size_t i = 0; i < dataTable.size(); i++) {						// Store given class labels in vector of strings named givenClassLabels
		int label = (int)dataTable[i][lastCol];
		givenClassLabels.push_back(label);
	}
	int predictedLabel;
	for (size_t i = 0; i < dataTable.size(); i++) {						// Predict class labels based on the decision tree
		predictedLabel = testDataOnDecisionTree(dataTable[i], root, defaultClass);
		predictedClassLabels.push_back(predictedLabel);
	}
	dataTable.clear();

	/* Print output */
	ofstream outputFile;
	outputFile.open("test_decisionTreeOutput.txt", ios::app);
	outputFile << endl << "--------------------------------------------------" << endl;
	double accuracy = printPredictionsAndCalculateAccuracy(givenClassLabels, predictedClassLabels);			// calculate accuracy of classification
	outputFile << "Accuracy of decision tree classifier = " << accuracy << "%"; 							// Print out accuracy to console
	outputFile.close();
	return accuracy;
}

vi _test(vvd& dataTable, node* &root, int defaultClass) {
	//node* root = *proot;
	vi predictedClassLabels;										// Stores the predicted class labels for each row
	int predictedLabel;
	for (size_t i = 0; i < dataTable.size(); i++) {						// Predict class labels based on the decision tree
		predictedLabel = testDataOnDecisionTree(dataTable[i], root, defaultClass);
		predictedClassLabels.push_back(predictedLabel);
	}
	dataTable.clear();
	return predictedClassLabels;
}

void _printResult(vi& predictedClassLabels, vvd& testingData) {
	vi givenClassLabels;											// Stores the given class labels in the test data
	int lastCol = testingData[0].size()-1;
	for (size_t i = 0; i < testingData.size(); i++) {						// Store given class labels in vector of strings named givenClassLabels
		int label = (int)testingData[i][lastCol];
		givenClassLabels.push_back(label);
	}
	/* Print output */
	ofstream outputFile;
	outputFile.open("test_decisionTreeOutput.txt", ios::app);
	outputFile << endl << "--------------------------------------------------" << endl;
	double accuracy = printPredictionsAndCalculateAccuracy(givenClassLabels, predictedClassLabels);			// calculate accuracy of classification
	outputFile << "Accuracy of decision tree classifier = " << accuracy << "%"; 							// Print out accuracy to console
	outputFile.close();
}

vi decisionTree(vvd& trainingData, node* &treeRoot) {
	int defaultClass = _train(trainingData, treeRoot);
	return _test(trainingData, treeRoot, defaultClass);
}

int findMax(int a[], int n) {
	if (n <= 0) return -1;
	int max = a[0];
	for (int i = 1; i < n; i++) {
		if (a[i] > max) max = a[i];
	}
	return max;
}

void devideTrainingData(vvd& trainingData, int numTrees, vvvd& devideTrainingData) {
	int i, j, dataSize = (int)trainingData.size() / numTrees;
	vvd tmpTrainingData;
	for (i = 0; i < numTrees - 1; i++) {
		for (j = i * dataSize; j < (i+1) * dataSize; j++) {
			tmpTrainingData.push_back(trainingData[j]);
		}
		devideTrainingData.push_back(tmpTrainingData);
		tmpTrainingData.clear();
	}
	for (i = (i+1) * dataSize; i < trainingData.size(); i++) {
		tmpTrainingData.push_back(trainingData[i]);
	}
	devideTrainingData.push_back(tmpTrainingData);
	tmpTrainingData.clear();
}

void randomForest(int numTrees, vvd& trainingData, vvd& testingData) {
	//set: int numClasses=2, int maxDepth=4, int maxBins=32
	node** treeRoot = new node*[numTrees];
	vvi predictedClassLabels; // Store all the predicted class label result of the randomforest
	vi mostPredictedClassLabels; //Store all the most frequent predicted label of the random foret to be the final predicted class label
	vvvd devidedTrainingData;
	devideTrainingData(trainingData, numTrees, devidedTrainingData);
	int* mostLabel = new int[MAX_CLASS_COUNT + 1]; // 
	memset(mostLabel, 0, (MAX_CLASS_COUNT + 1) * sizeof(int));

	for (int i = 0; i < numTrees; i++) {
		vi tmp = decisionTree(devidedTrainingData[i], treeRoot[i]);
		predictedClassLabels.push_back(tmp);
	}
	int predictedLabel = 0; // Get the most frequent predicted label of the random foret to be the final predicted class label
	for (size_t column = 0; column < predictedClassLabels[0].size(); column++) {
		for (size_t tree = 0; tree < predictedClassLabels.size(); tree++) {
			mostLabel[predictedClassLabels[tree][column]]++;
		}
		predictedLabel = findMax(mostLabel, MAX_CLASS_COUNT);
		mostPredictedClassLabels.push_back(predictedLabel);
		memset(mostLabel, 0, (MAX_CLASS_COUNT + 1) * sizeof(int));
	}
	_printResult(mostPredictedClassLabels, testingData);
}


int main(int argc, const char *argv[]) {
	vvd trainTable;													
	vvd testTable;
	readTrainFile("data/top20.csv", trainTable);
	readTestFile("data/top20.csv", testTable);							// Input data in the form of a vector of vector of strings
	randomForest(2, trainTable, testTable);
	trainTable.clear(); 												// clear dataTable of training data to store testing data
	testTable.clear(); 												// clear dataTable of training data to store testing data
	return 0;
}