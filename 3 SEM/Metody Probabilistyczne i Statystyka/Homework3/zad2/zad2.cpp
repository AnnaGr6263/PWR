#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <cmath>
#include <matplot/matplot.h>

// algorytm sortowania przez wstawianie
void insertion_sort(std::vector<int>& arr, int& comparisons, int& movements) {
	int n = arr.size();
	for (int j = 1; j < n; ++j) {
		int key = arr[j];
		int i = j - 1;

		while (i >= 0 && arr[i] > key) {
			arr[i + 1] = arr[i];
			--i;
			++comparisons;
			++movements;
		}
		++comparisons;
		if (arr[i + 1] != key) {
			arr[i + 1] = key;
			++movements;
		}
	}
}

// permutacje
std::vector<int> generate_random_permutation(int n) {
	std::vector<int> perm(n);
	std::iota(perm.begin(), perm.end(), 1);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::shuffle(perm.begin(), perm.end(), rng);
	return perm;
}

void plot_results(const std::string& file, const std::string& title,
				  const std::vector<int>& x, const std::vector<int>& y, const std::string& ylabel) {

	std::vector<double> x_double(x.begin(), x.end());
	std::vector<double> y_double(y.begin(), y.end());

	matplot::figure();
	matplot::scatter(x_double, y_double)->marker(".").marker_size(5).color("blue");
	matplot::title(title);
	matplot::xlabel("Rozmiar tablicy (n)");
	matplot::ylabel(ylabel);
	matplot::grid(true);
	matplot::save(file);
}

void plot_line_results(const std::string& file, const std::string& title,
					   const std::vector<int>& x, const std::vector<double>& y, const std::string& ylabel) {

	std::vector<double> x_double(x.begin(), x.end());

	matplot::figure();
	matplot::plot(x_double, y)->line_width(2).color("red");
	matplot::title(title);
	matplot::xlabel("Rozmiar tablicy (n)");
	matplot::ylabel(ylabel);
	matplot::grid(true);
	matplot::save(file);
}

int main() {
	const int start_n = 100;
	const int end_n = 10000;
	const int step_n = 100;
	const int repetitions = 50;

	std::vector<int> n_values;
	std::vector<int> total_comparisons, total_movements;
	std::vector<double> avg_cmp_n, avg_cmp_n2, avg_mov_n, avg_mov_n2;

	for (int n = start_n; n <= end_n; n += step_n) {
		int sum_comparisons = 0, sum_movements = 0;

		for (int rep = 0; rep < repetitions; ++rep) {
			int comparisons = 0, movements = 0;
			std::vector<int> array = generate_random_permutation(n);

			insertion_sort(array, comparisons, movements);
			sum_comparisons += comparisons;
			sum_movements += movements;
		}

		n_values.push_back(n);
		total_comparisons.push_back(sum_comparisons / repetitions);
		total_movements.push_back(sum_movements / repetitions);

		avg_cmp_n.push_back(double(sum_comparisons / repetitions) / n);
		avg_cmp_n2.push_back(double(sum_comparisons / repetitions) / (n * n));
		avg_mov_n.push_back(double(sum_movements / repetitions) / n);
		avg_mov_n2.push_back(double(sum_movements / repetitions) / (n * n));
	}

	// Wykresy
	plot_results("cmp_n.png", "Porównania (cmp(n))", n_values, total_comparisons, "Porównania");
	plot_results("mov_n.png", "Ruchy (s(n))", n_values, total_movements, "Ruchy");

	plot_line_results("cmp_n_ratio.png", "cmp(n) / n", n_values, avg_cmp_n, "Stosunek");
	plot_line_results("cmp_n2_ratio.png", "cmp(n) / n^2", n_values, avg_cmp_n2, "Stosunek");
	plot_line_results("mov_n_ratio.png", "s(n) / n", n_values, avg_mov_n, "Stosunek");
	plot_line_results("mov_n2_ratio.png", "s(n) / n^2", n_values, avg_mov_n2, "Stosunek");

	std::cout << "Wykresy wygenerowane pomyślnie!" << std::endl;
	return 0;
}