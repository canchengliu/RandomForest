#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#include <math.h>
#include <float.h>
#include <cstdlib>
#include <iomanip>
#include <limits.h>
#include <cstring>
#include <algorithm>

#define MAX_BIN 2
#define MAX_CLASS_COUNT 26 // the total number of class
#define MAX_DEEPTH 3
using namespace std;


struct splitBound;

typedef vector<string> vs;
typedef vector<vs> vvs;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef map<string, int> msi;
typedef map<int, int> mii;
typedef vector<double> vd;
typedef vector<vd> vvd;
typedef vector<vvd> vvvd;
typedef vector<splitBound> vsb;
typedef vector<vsb> vvsb;

#endif