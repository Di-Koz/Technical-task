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

//проверка строку на шаблонность
bool checkParam(string str, string temp) {
	if (regex_match(str, regex(temp))) {
		return true;
	}
	return false;
}
//заполнение структуры параметров из исходной строки
void fillParam(string str, parameters& userStruct, vector<string>& names) {
	//первый необязательный параметр
	if ((str.find("-a") != -1) || (str.find("-d") != -1)) {
		if (str.find("-a") != -1) {
			userStruct.mode = "-a";
		}
		else if (str.find("-d") != -1) {
			userStruct.mode = "-d";
		}
	}
	else {
		userStruct.mode = "-a";
	}
	//второй обязательный параметр
	if (str.find("-s") != -1) {
		userStruct.type = "-s";
	}
	else if (str.find("-i") != -1) {
		userStruct.type = "-i";
	}
	//имя выходного файла
	int pos2 = str.find(".txt"), pos1{ pos2 };
	string name{ "" };
	while (str[pos1] != ' ') {
		--pos1;
	}
	for (int i{ pos1 + 1 }; i < pos2 + 4; ++i) {
		name += str[i];
	}
	userStruct.outputFile = name;
	//число файлов для сортировки и заполнение вектора именами входных файлов
	int lenght = str.size(), step{ 1 };
	for (int i{ pos2 + 5 }; i < lenght; i += step) {
		string subStr{ "" };
		int	ind{ i };
		while ((str[ind] != ' ') && (ind < lenght)) {
			subStr += str[ind];
			++ind;
		}
		//сдвиг на длину + 1, чтобы сразу перепрыгнуть пробел
		step = subStr.size() + 1;
		names.push_back(subStr);
		++userStruct.numberOfFiles;
	}
}
//проверка на существование всех файлов
bool checkNames(vector<string>& names, string outputName) {
	int dlina = names.size();
	for (int i{ 0 }; i < dlina; ++i) {
		//проверка на то, что один из файлов называется также, как и
		//выходной файл
		if (names[i] == outputName) {
			return false;
		}
		//проверка на открытие файла
		ifstream file(names[i]);
		if (!file) {
			return false;
		}
		file.close();
	}
	return true;
}
//заполнение вектора
void fillVector(vector<int>& arr, string fileName) {
	ifstream firstFile(fileName);
	while (!firstFile.eof()) {
		string str{ "" };
		firstFile >> str;

		arr.push_back(stoi(str));
	}
	firstFile.close();
}
//слияние
void merge(vector<int>& arr1, vector<int>& arr2, vector<int>& arrRes) {
	int dlina1 = arr1.size(), dlina2 = arr2.size(), ind1{ 0 }, ind2{ 0 };
	//пока не вышли за границы одного из массивов
	while ((ind1 < dlina1) && (ind2 < dlina2)) {
		//если эл-т какого-то массива меньше эл-та другого массива, 
		//записываем его и сдвигаем границу этого же массива 
		if (arr1[ind1] <= arr2[ind2]) {
			arrRes.push_back(arr1[ind1]);
			++ind1;
		}
		else {
			arrRes.push_back(arr2[ind2]);
			++ind2;
		}
	}
	//если значения какого-то массива закончились, 
	//записываем хвост другого массива
	if (ind1 == dlina1) {
		for (int i{ ind2 }; i < dlina2; ++i) {
			arrRes.push_back(arr2[i]);
		}
	}
	if (ind2 == dlina2) {
		for (int i{ ind1 }; i < dlina1; ++i) {
			arrRes.push_back(arr1[i]);
		}
	}
}
//проверка на отсортированность по возрастанию
bool checkOrder(vector<int>& arr) {
	int dlina = arr.size();
	for (int i{ 0 }; i < dlina - 1; ++i) {
		if (arr[i] > arr[i + 1]) {
			return false;
		}
	}
	return true;
}
//обмен
void swap(int& a, int& b) {
	int tmp = a;
	a = b;
	b = tmp;
}
//быстрая сортировка(рекурсия)
void sort_quick_rec(vector<int>& arr, int indLeft, int indRight) {
	//допустим, пилотируемый элемент самый левый
	int left{ indLeft }, right{ indRight }, ch{ arr[left] };

	do {
		while (arr[left] < ch) {
			++left;
		}

		while (arr[right] > ch) {
			--right;
		}

		if (left <= right) {
			if (left != right) {
				swap(arr[left], arr[right]);
			}
			++left;
			--right;
		}
	} while (left <= right);
	//спуск
	if (left < indRight) {
		sort_quick_rec(arr, left, indRight);
	}
	if (indLeft < right) {
		sort_quick_rec(arr, indLeft, right);
	}
}
//частичная сортировка
void sort_partial(vector<int>& arr) {
	int dlina = arr.size(), ind{ 0 };
	//находим элемент, на котором "ломается" последовательность 
	for (int i{ 0 }; i < dlina - 1; ++i) {
		if (arr[i] > arr[i + 1]) {
			ind = i + 1;
			break;
		}
	}
	int indLeft{ ind - 1 }, indRight{ 0 };
	//страхуемся на случай, если этот элемент последний
	if (ind != dlina - 1) {
		indRight = ind + 1;
	}
	else {
		indRight = ind;
	}
	//определяем левую границу, которую нужно отсортировать
	while ((arr[indLeft] > arr[ind]) && (indLeft > 0)) {
		--indLeft;
	}
	//определяем правую границу, которую нужно отсортировать
	while ((arr[indRight] < arr[ind]) && (indRight < dlina - 1)) {
		++indRight;
	}

	sort_quick_rec(arr, indLeft, indRight);
}
//запись в файл
void fillFile(vector<int>& arr, string fileName) {
	int dlina = arr.size();
	ofstream file(fileName, ios::app);
	for (int i{ 0 }; i < dlina; ++i) {
		file << arr[i];
		file << '\n';
	}
	file.close();
}

static clock_t timer_start_time{ 0 }, timer_stop_time{ 0 };
void start_timer() {
	timer_start_time = clock();
}
void stop_timer() {
	timer_stop_time = clock();
	cout << double(timer_stop_time - timer_start_time) / CLOCKS_PER_SEC << " sec" << endl;
}
