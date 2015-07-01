#ifndef RANDOMFOREST_H
#define RANDOMFOREST_H

#include "DecisionTree.h"



void readTrainFile(const char* filename, vvd& dataTable);
void readTestFile(const char* filename, vvd& dataTable);
int _train(vvd& dataTable, node* &root);
vi _test(vvd& dataTable, node* &root, int defaultClass);
vi decisionTree(vvd& trainingData, vvd& testingData);
int _findMax(int a[], int n);
void randomForest(int numTrees, vvd& trainingData, vvd& testingData, vvi& predictedClassLabels);
void allRandomForest(vvi& allPredictedClassLabels, vvd& testingData);
void printResult(vi& predictedClassLabels, vvd& testingData);
void printKaggleResult(vi& predictedClassLabels);

#endif
