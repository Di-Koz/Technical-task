#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;
struct parameters {
	string mode;
	string type;
	string outputFile;
	int numberOfFiles = 0;
};

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

bool checkParam(string str, string temp);
void fillParam(string str, parameters &userStruct, vector<string>& names);
bool checkNames(vector<string>& names, string outputName);
void fillVector(vector<int>& arr, string fileName);
void merge(vector<int>& arr1, vector<int>& arr2, vector<int>& arrRes);
bool checkOrder(vector<int>& arr);
void swap(int& a, int& b);
void sort_quick_rec(vector<int>& arr, int indLeft, int indRight);
void sort_partial(vector<int>& arr);
void fillFile(vector<int>& arr, string fileName);
void start_timer();
void stop_timer();

#endif
