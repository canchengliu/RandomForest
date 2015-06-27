#include "header.h"

string linefilter(string &sourceline) {
	return sourceline.substr(sourceline.find(',') + 2, sourceline.size()); // get rid of the id of each line
}

/*
* Split a String by char c
* return a vector of string
*/

vd split(string str, char c) {
	vd result;
	size_t pos;
	string item;
	while (!str.empty() && str.find(c) != string::npos) {
		pos = str.find(c);
		item = str.substr(0, pos);
		result.push_back(atof(item.c_str()));
		str.erase(0, pos + 1);
	}
	result.push_back(atof(str.c_str()));
	return result;
}

/*
* Parses a string and stores data
* into a vector of vector of doubles
*/
void parse(string someString, vvd &attributeTable){
	vd lineValue = split(someString, ',');
	attributeTable.push_back(lineValue);
	lineValue.clear();
}

/*
* Prints a vector of vector of strings
* For debugging purposes only.
*/
void printAttributeTable(vvd &attributeTable){
	size_t inner, outer;
	for (outer = 0; outer < attributeTable.size(); outer++) {
		for (inner = 0; inner < attributeTable[outer].size(); inner++) {
			cout << attributeTable[outer][inner] << "\t";
		}
		cout << endl;
	}
}

bool isBetweenBound(double value, splitBound &bound) {
	return (bound.leftBound < value && value < bound.rightBound);
}

/*
* Prunes a table based on a column/attribute's name
* and value of that attribute. Removes that column
* and all rows that have that value for that column.
*/
vvd pruneTable(vvd &attributeTable, int colIndex, splitBound& bound){
	size_t i, j;
	vvd prunedTable;

	for (i = 0; i < attributeTable.size(); i++) {
		vd auxRow;
		if (isBetweenBound(attributeTable[i][colIndex], bound)) {
			for (j = 0; j < attributeTable[i].size(); j++) {
				if (j != colIndex) {
					auxRow.push_back(attributeTable[i][j]);
				}
			}
			prunedTable.push_back(auxRow);
		}
	}
	return prunedTable;
}

vsb getSplitBoundOfColumn(vd &columnVal) {
	sort(columnVal.begin(), columnVal.end());
	int pre = 0;
	int size = columnVal.size();
	int bins = min(MAX_BIN, size);
	int perlen = size / bins;
	vsb sbound;
	int rest = size - perlen * bins;
	//sbound.push_back(splitBound(DBL_MIN, columnVal[0]));
	for (int i = 0; i < bins; i++) {
		if (i < rest) {
			sbound.push_back(splitBound(columnVal[pre], columnVal[pre + perlen], perlen + 1));
			pre += perlen + 1;
		}
		else {
			sbound.push_back(splitBound(columnVal[pre], columnVal[pre + perlen - 1], perlen));
			pre += perlen;
		}
	}
	//sbound.append(splitBound(columnVal[len - 1], DBL_MAX));
	return sbound;
}

/*
* Returns a vvs which contains information about
* the data table. The vvs contains the names of
* all the columns and the values that each
* column can take
*/
vvsb generateTableInfo(vvd &dataTable) {
	vvsb tableInfo;
	for (size_t i = 0; i < dataTable[0].size() - 1; i++) {
		vd columnVal;
		for (size_t j = 0; j < dataTable.size(); j++) {
			columnVal.push_back(dataTable[j][i]);
		}
		vsb tempInfo = getSplitBoundOfColumn(columnVal);
		tableInfo.push_back(tempInfo);
	}
	return tableInfo;
}

/*
* Recursively builds the decision tree based on
* the data that it is passed and tha table info.
* @Param
* int splitOn;											// Stores which attribute to split on at a particular node
* int label;											// Stores the class label for leaf nodes. For nodes that are not leaf nodes, it stores the value of the attribute of the parent's' split
* bool isLeaf;											// boolean flag for leaf nodes
* vsb childrenSplitBound;							// Stores the values of the childrens' attributes
* vector<node*> children;
*/
node* buildDecisionTree(vvd &table, node* nodePtr, vvsb &tableInfo){
	if (tableIsEmpty(table)) {
		return NULL;
	} else if (isHomogeneous(table)) {
		nodePtr->isLeaf = true;
		nodePtr->label = (int)table[0][table[0].size() - 1];
		return nodePtr;
	} else if (nodePtr->deepth >= MAX_DEEPTH) {
		nodePtr->isLeaf = true;
		nodePtr->label = returnMostFrequentClass(table);
	}
	else {
		int colIndex = decideSplittingColumn(table);
		//cout << "*11";
		nodePtr->splitOn = colIndex;
		nodePtr->isLeaf = false;
		cout << colIndex;
		cout << "size:" << tableInfo.size();
		//int colIndex = returnColumnIndex(splittingCol, tableInfo);
		for (size_t i = 0; i < tableInfo[colIndex].size(); i++) {
			//cout << "*33&&&&&";
			node* newNode = (node*) new node;
			newNode->label = 0;//tableInfo[colIndex][i];
			newNode->deepth = nodePtr->deepth + 1;
			newNode->isLeaf = false;
			newNode->splitOn = colIndex;
			nodePtr->childrenSplitBound.push_back(tableInfo[colIndex][i]);
			vvd auxTable = pruneTable(table, colIndex, tableInfo[colIndex][i]);
			//cout << "*66";
			nodePtr->children.push_back(buildDecisionTree(auxTable, newNode, tableInfo));
			//cout << "*77";
		}
		//cout << "*44";
	}
	return nodePtr;
}

/*
* Returns true if all rows in a subtable
* have the same class label.
* This means that that node's class label
* has been decided.
*/
bool isHomogeneous(vvd &table){
	size_t i;
	int lastCol = table[0].size() - 1;
	int firstLabel = (int)table[0][lastCol];
	for (i = 0; i < table.size(); i++) {
		if (firstLabel != (int)table[i][lastCol]) {
			return false;
		}
	}
	return true;
}

/*
* Returns a vector of integers containing the counts
* of all the various values of an attribute/column.
*/
vi countDistinctClass(vvd &table) {
	vi classCount;
	mii foundClass;
	int lastCol = table[0].size() - 1;
	for (size_t i = 0; i < table.size(); i++) {
		int temp = (int)table[i][lastCol];
		if (foundClass.find(temp) == foundClass.end()) {
			foundClass[temp] = 1;
		}
		else {
			foundClass[temp]++;
		}
	}
	mii::iterator iter;
	for (iter = foundClass.begin(); iter != foundClass.end(); ++iter) {
		classCount.push_back(iter->second);
	}
	return classCount;
}

/*
* Decides which column to split on
* based on entropy. Returns the column
* with the least entropy.
*/
int decideSplittingColumn(vvd &table){
	size_t column, i;
	int splittingColumn = 0;
	double minEntropy = DBL_MAX;
	vvsb tableInfo = generateTableInfo(table);
	splitBound currSplitBound;

	for (column = 0; column < tableInfo.size(); column++) { // Calculate the Entropy when split on different column and get the best strategy to split
		vd attributeEntropy;
		double columnEntropy = 0.0;
		double entropy = 0.0;
		for (i = 0; i < tableInfo[column].size(); i++) {
			currSplitBound = tableInfo[column][i];
			//cout << "*ss";
			vvd tempTable = pruneTable(table, column, currSplitBound);
			if (tableIsEmpty(tempTable)) {
				attributeEntropy.push_back(0.0);
				continue;
			}
			//cout << "*tt";
			vi classCounts = countDistinctClass(tempTable);
			//cout << "*jj";
			size_t j, classCountSize = classCounts.size();
			//cout << "*ip";
			for (j = 0; j < classCountSize; j++) {
				double temp = (double)classCounts[j];
				entropy -= (temp / classCounts[classCountSize - 1])*(log(temp / classCounts[classCountSize - 1]) / log(2));
			}
			//cout << "*wi";
			attributeEntropy.push_back(entropy);
			entropy = 0.0;
			//cout << "*QQ";
		}
		//cout << "*ii";
		for (i = 0; i < tableInfo[column].size(); i++) {
			columnEntropy += ((double)tableInfo[column][i].numInBound * (double)attributeEntropy[i]);
		}
		//cout << "*ij";
		columnEntropy = columnEntropy / ((double)table.size() - 1);
		if (columnEntropy <= minEntropy) {
			minEntropy = columnEntropy;
			splittingColumn = column;
		}
	}
	return splittingColumn;
}

/*
* Returns true if the table is empty
* returns false otherwise
*/
inline bool tableIsEmpty(vvd &table){
	return (table.size() == 0);
}

/*
* Recursively prints the decision tree
* For debugging purposes only

void printDecisionTree(node* nodePtr){
if(nodePtr == NULL) {
return;
}
if (!nodePtr->children.empty()) {
cout << " Value: " << nodePtr->label << endl;
cout << "Split on: " << nodePtr->splitOn;
size_t i;
for (i = 0; i < nodePtr->children.size(); i++) {
cout << "\t";
printDecisionTree(nodePtr->children[i]);
}
return;
} else {
cout << "Predicted class = " << nodePtr->label;
return;
}
}
*/

/*
* Takes a row and traverses that row through
* the decision tree to find out the
* predicted class label. If none is found
* returns the default class label which is
* the class label with the highest frequency.
*/
int testDataOnDecisionTree(vd &singleLine, node* nodePtr, int defaultClass){
	int prediction = 0;
	int colIndex, childIndex;
	double value;
	//nodePtr->isLeaf = false;
	////cout << "*" <<( (nodePtr->isLeaf == false) ? 1:3 )<< "*" << nodePtr->children.size() << endl;
	while (!nodePtr->isLeaf && !nodePtr->children.empty()) {
		//cout << "*@@@";
		colIndex = nodePtr->splitOn;
		value = singleLine[colIndex];
		childIndex = getIndexOfChildren(nodePtr->childrenSplitBound, value);
		nodePtr = nodePtr->children[childIndex];
		if (nodePtr == NULL) {
			//cout << "*(((((((((((";
			prediction = defaultClass;
			break;
		}
		prediction = nodePtr->label;
		
	}
	return prediction;
}

/*
* Returns an integer which is the index
* of a string in a vector of strings
*/
int getIndexOfChildren(vsb &childsb, double value) {
	for (size_t i = 0; i < childsb.size(); i++) {
		if (isBetweenBound(value, childsb[i])) {
			return i;
		}
	}
	return 0;
}

/*
* Returns an integer which is the index
* of a string in a vector of strings

int returnIndexOfVector(vs &stringVector, string value){
size_t i;
for (i = 0; i < stringVector.size(); i++) {
if (stringVector[i] == value)	{
return i;
}
}
return -1;
}
*/

/*
* Outputs the predictions to file
* and returns the accuracy of the classification
*/
double printPredictionsAndCalculateAccuracy(vi &givenData, vi &predictions){
	ofstream outputFile;
	outputFile.open("test_decisionTreeOutput.txt");
	int correct = 0;
	outputFile << setw(3) << "#" << setw(16) << "Given Class" << setw(31) << right << "Predicted Class" << endl;
	outputFile << "--------------------------------------------------" << endl;
	for (size_t i = 0; i < givenData.size(); i++) {
		outputFile << setw(3) << i + 1 << setw(16) << givenData[i];
		if (givenData[i] == predictions[i]) {
			correct++;
			outputFile << "  ------------  ";
		}
		else {
			outputFile << "  xxxxxxxxxxxx  ";
		}
		outputFile << predictions[i] << endl;
	}
	outputFile << "--------------------------------------------------" << endl;
	outputFile << "Total number of instances in test data = " << givenData.size() << endl;
	outputFile << "Number of correctly predicted instances = " << correct << endl;
	outputFile.close();
	return (double)correct / givenData.size() * 100;
}


/*
* Returns the most frequent class from the training data
* This class will be used as the default class label
*/
int returnMostFrequentClass(vvd &dataTable){
	int lastCol = dataTable[0].size() - 1, mostFrequentClass = 0, maxCount = 0;
	int classCount[MAX_CLASS_COUNT + 1];
	memset(classCount, 0, (MAX_CLASS_COUNT + 1) * sizeof(int));

	for (size_t i = 0; i < dataTable.size(); i++) {
		classCount[(int)dataTable[i][lastCol]]++;
	}
	for (size_t i = 0; i < MAX_CLASS_COUNT + 1; i++) {
		if (classCount[i] > maxCount) {
			mostFrequentClass = i;
			maxCount = classCount[i];
		}
	}
	return mostFrequentClass;
}

