/*
* LAB 6, Вариант №4
* Дана последовательность символов С = {с0 …сn–1} и символ b. Соз-
* дать OpenMP-приложение для определения количество вхождений символа b
* в строку C. Количество символов и потоков являются входными параметра-
* ми программы, количество символов в строке может быть не кратно количе-
* ству потоков.
*
* Студентка - Ли Ицзя
* гр. 5140904/30202
*/
#include <omp.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

int MAX_THREADS_NUMBER = 0;
int RUNNING_THREADS_NUMBER = 4;
int STRING_LENGTH = 0;
char b = 'a';
std::string C = "";

void print_vec(std::vector<int> vec) {
	for (auto i : vec) {
		std::cout << i << "\n";
	}
}

std::string random_string_generator(int n) {
	srand((unsigned)time(NULL));
	std::string str = "";
	for (int i = 1; i <= n; i++) {
		int flag;
		flag = rand() % 2; // Randomly make flag to 1 or 0. 1 - upcase，else lowercase.
		if (flag == 1)
			str += rand() % ('Z' - 'A' + 1) + 'A';
		else
			str += rand() % ('z' - 'a' + 1) + 'a';

	}
	return str;
}

int main() {
	std::ofstream fout;
	fout.open("C.txt");

	MAX_THREADS_NUMBER = omp_get_max_threads(); // максимальное количество нитей
	STRING_LENGTH = 1234567; // длина строки

	std::cout << "# Max threads number: " << MAX_THREADS_NUMBER << '\n';
	std::cout << "# Working threads number: " << RUNNING_THREADS_NUMBER << '\n';
	if (MAX_THREADS_NUMBER < RUNNING_THREADS_NUMBER) {
		std::cout << "Not enough thread.";
		return 0;
	}
	else {
		omp_set_num_threads(RUNNING_THREADS_NUMBER);
	}
	C = random_string_generator(STRING_LENGTH); // последовотелность символов С
	b = 'a'; // символ b

	std::cout << "# String(C): " << C << '\n';
	std::cout << "# Length of C: " << C.size() << '\n';
	std::cout << "# Symbol(b): " << b << '\n';
	fout << C;

	int hit = 0; // количество попадания
#pragma omp parallel for schedule(static, RUNNING_THREADS_NUMBER)
	for (int i = 0; i < C.size(); ++i) {
		if (C[i] == b) {
			++hit;
		}
	}
	std::cout << "# Hit: " << hit << '\n';
	int ground_truth = 0;
	ground_truth = std::count(C.begin(), C.end(), b);
	std::cout << "# Ground truth: " << hit << '\n';

	return 0;
}
