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

/*
 * Decision tree testing phase
 * In this phase, the testing is read
 * from the file, parsed and stored.
 * Each row in the table is made to
 * traverse down the decision tree
 * till a class label is found.
 */
void test(vvd& dataTable, node* &root, int defaultClass) {
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
}


int main(int argc, const char *argv[]) {
	vvd dataTable;													// Input data in the form of a vector of vector of strings
	node* root = NULL;
	int defaultClass;
	readTrainFile("data/top20.csv", dataTable);
	cout << "1";
	defaultClass = train(dataTable, root);
	cout << "2";
	dataTable.clear(); 												// clear dataTable of training data to store testing data
	cout << "3";
	readTestFile("data/top20.csv", dataTable);
	cout << "4";
	if (root == NULL) {
		cout << "NULL" << endl;
		return 0;
	}
	test(dataTable, root, defaultClass);
	cout << "5";
	dataTable.clear();
	return 0;
}








