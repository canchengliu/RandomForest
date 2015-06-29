#ifndef RFMPI_H
#define RFMPI_H

#include "RandomForest.h"

void execute_main(
	const int process_count, 
	const double sample_probability, 
	vvd& trainingData, 
	vvd&testingData, 
	unsigned int numTrees);

void execute_child(
	const unsigned int parent_rank, 
	const unsigned int rank, 
	vvd& testingData, 
	unsigned int numTrees);

#endif