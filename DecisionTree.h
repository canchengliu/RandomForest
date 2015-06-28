#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "header.h"

struct splitBound {
	double leftBound;
	double rightBound;
	int numInBound;
	splitBound(double l = DBL_MIN, double r = DBL_MAX, int n = 0) {
		leftBound = l;
		rightBound = r;
		numInBound = n;
	}
};

struct node {												// struct node defines the structure of a node of the decision tree
	int splitOn;											// Stores which attribute to split on at a particular node
	int label;												// Stores the class label for leaf nodes. For nodes that are not leaf nodes, it stores the value of the attribute of the parent's' split 
	bool isLeaf;											// boolean flag for leaf nodes
	int deepth;												// Stores the deepth of the decision tree, the deepth of root is 0
	splitBound sb;											// 
	vsb childrenSplitBound;									// Stores the values of the childrens' attributes
	vector<node*> children;									// Stores pointers to the children of a node
};

string linefilter(string&);									// get rid of id of each line	
void parse(string, vvd&);									// Parses a single line from the input file and stores the information into a vector of vector of strings 
void printAttributeTable(vvd&);								// For debugging purposes only. Prints a data table
vvd pruneTable(vvd&, int, splitBound&);						// Prunes a table based on a column/attribute's name and the value of that attribute. Removes that column and all instances that have that value for that column
node* buildDecisionTree(vvd&, node*, vvsb&);				// Builds the decision tree based on the table it is passed
bool isHomogeneous(vvd&);									// Returns true if all instances in a subtable at a node have the same class label
vi countDistinctClass(vvd&);								// Returns a vector of integers containing the counts of all the various values of an attribute/column
int decideSplittingColumn(vvd&);							// Returns the column on which to split on. Decision of column is based on entropy

bool tableIsEmpty(vvd&);									// Returns true if a subtable is empty
int testDataOnDecisionTree(vd&, node*, int);				// Runs a single instance of the test data through the decision tree. Returns the predicted class label
int getIndexOfChildren(vsb&, double);						// Returns the index of a string in a vector of strings
double printPredictionsAndCalculateAccuracy(vi&, vi&);		// Outputs the predictions to file and returns the accuracy of the classification
vvsb generateTableInfo(vvd&);								// Generates information about the table in a vector of vector of stings
int returnMostFrequentClass(vvd&);							// Returns the most frequent class from the training data. This class is used as the default class during the testing phase


#endif