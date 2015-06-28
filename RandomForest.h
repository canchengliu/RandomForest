#ifndef RANDOMFOREST_H
#define RANDOMFOREST_H

#include "header.h"

void readTrainFile(const char* filename, vvd& dataTable);
void readTestFile(const char* filename, vvd& dataTable);
int train(vvd& dataTable, node* &root);
int _train(vvd& dataTable, node* &root);
double test(vvd& dataTable, node* &root, int defaultClass);
vi _test(vvd& dataTable, node* &root, int defaultClass);
void _printResult(vi& predictedClassLabels, vvd& testingData);
vi decisionTree(vvd& trainingData, node* &treeRoot, vvd& testingData);
int findMax(int a[], int n);
void randomSelectTrainingData(vvd& trainingData, int numTrees, vvvd& selectedTrainingData);
void randomForest(int numTrees, vvd& trainingData, vvd& testingData);

#endif
