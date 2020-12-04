#include "../include/util/file_manager.h"

#include <algorithm>
#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <vector>

//产生数据，需要三个参数，column-size, column-path和column-dist，分别对应大小、路径和分布
//产生段错误很可能是generate_column中这个vector<int>的原因
using namespace std;
int COLUMN_SIZE, COLUMN_DISTRIBUTION;
string COLUMN_FILE_PATH;

int generate_column(int N) {
	vector<int> data;
	for (int i = 0; i < N; i++) {
		data.push_back(i);
	}
	random_shuffle(data.begin(), data.end());//产生1-N个数据，然后进行随机排序
	FILE *f = fopen(COLUMN_FILE_PATH.c_str(), "w+");
	fwrite(&data[0], sizeof(int), N, f);
	fclose(f);
	return 0;
}

int generate_column_skewed(int N) {//偏斜数据，10%在0-49.9%, 10在50-100%, 80%在49.9-50%
	vector<int> data;
	for (int i = 0; i < N; i++) {
		int aux, min, max;
		int variance = rand() % 10;
		if (variance == 0) {
			min = 0;
			max = N * 0.499;
			aux = rand() % (max - min + 1) + min; //! 0 - 40
		} else if (variance == 9) {
			min = N * 0.5;
			max = N;
			aux = rand() % (max - min + 1) + min;
		} else { //! 40 -50
			min = N * 0.499;
			max = N * 0.5;
			aux = rand() % (max - min + 1) + min;
		}
		data.push_back(aux);
	}
	random_shuffle(data.begin(), data.end());
	FILE *f = fopen(COLUMN_FILE_PATH.c_str(), "w+");
	fwrite(&data[0], sizeof(int), N, f);
	fclose(f);
	return 0;
}

//检查数据
void print_help(int argc, char **argv) {
	fprintf(stderr, "Unrecognized command line option.\n");
	fprintf(stderr, "Usage: %s [args]\n", argv[0]);
	fprintf(stderr, "   --column-path\n");
	fprintf(stderr, "   --column-size\n");
	fprintf(stderr, "   --column-dist\n");
}

//分割数据
pair<string, string> split_once(string delimited, char delimiter) {
	auto pos = delimited.find_first_of(delimiter);
	return {delimited.substr(0, pos), delimited.substr(pos + 1)};
}

int main(int argc, char **argv) {
	COLUMN_FILE_PATH = "column";
	COLUMN_SIZE = 100;
	COLUMN_DISTRIBUTION = 1;
	for (int i = 1; i < argc; i++) {
		auto arg = string(argv[i]);
		if (arg.substr(0, 2) != "--") {
			print_help(argc, argv);
			exit(EXIT_FAILURE);
		}
		arg = arg.substr(2);
		auto p = split_once(arg, '=');
		auto &arg_name = p.first;
		auto &arg_value = p.second;
		if (arg_name == "column-path") {
			COLUMN_FILE_PATH = arg_value;
		} else if (arg_name == "column-size") {
			COLUMN_SIZE = atoi(arg_value.c_str());
		} else if (arg_name == "column-dist") {
			COLUMN_DISTRIBUTION = atoi(arg_value.c_str());
		} else {
			print_help(argc, argv);
			exit(EXIT_FAILURE);
		}
	}
	if (COLUMN_DISTRIBUTION == 1)
		generate_column(COLUMN_SIZE);
	else
		generate_column_skewed(COLUMN_SIZE);
}
