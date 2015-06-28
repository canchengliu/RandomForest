#include "DecisionTree.h"
#include "RandomForest.h"

int main(int argc, const char *argv[]) {
	vvd trainTable;													
	vvd testTable;
	readTrainFile("data/top20.csv", trainTable);
	readTestFile("data/top20.csv", testTable);							// Input data in the form of a vector of vector of strings
	randomForest(3, trainTable, testTable);
	trainTable.clear(); 												// clear dataTable of training data to store testing data
	testTable.clear(); 													// clear dataTable of training data to store testing data
	return 0;
}