#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <algorithm>
#include <fstream>
#include "functions.h"

using namespace std;

int main() {
	string userStr{ "" };
	//шаблон строки
	const string TEMP = "(-a |-d )?(-s|-i){1}( [a-z]+.txt){2,}";
	getline(cin, userStr);
	//userStr = "-a -i output.txt tra.txt trb.txt trc.txt trd.txt tre.txt trf.txt trg.txt trh.txt tri.txt trj.txt";

	start_timer();
	if (checkParam(userStr, TEMP)) {
		parameters userParameters;
		vector<string> filesName;
		fillParam(userStr, userParameters, filesName);

		if (checkNames(filesName, userParameters.outputFile)) {
			vector<int> masRes;
			for (int i{ 0 }; i < userParameters.numberOfFiles; ++i) {
				vector<int> masCur, masZapas;
				fillVector(masCur, filesName[i]);
				//если последовательность изначально не по возрастанию, 
				//проводим частичную сортировку
				if (!checkOrder(masCur)) {					
					while (!checkOrder(masCur)) {
						sort_partial(masCur);
					}

					cout << "In " << filesName[i] << " was incorrect order, but we fixed it" << endl;
				}

				merge(masRes, masCur, masZapas);
				masRes = masZapas;
			}

			if (checkOrder(masRes)) {
				//если нужна последовательность по убыванию
				if (userParameters.mode == "-d") {
					reverse(masRes.begin(), masRes.end());
				}
				fillFile(masRes, userParameters.outputFile);
			}
			else {
				cerr << "Something went wrong";
			}
		}
		else {
			cerr << "Some of your files doesn't exist";
		}
	}
	else {
		cerr << "Wrong string";
	}
	stop_timer();
}
