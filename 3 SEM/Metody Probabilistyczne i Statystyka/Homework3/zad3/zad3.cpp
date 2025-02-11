#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <matplot/matplot.h>

// Symulacja procesu nadawania i obliczanie liczby rund
int simulate_broadcast(int n, double p, std::mt19937 &rng) {
    int rounds = 0;
    int remaining_stations = n;
    std::bernoulli_distribution distribution(p);

    while (remaining_stations > 0) {
        for (int i = 0; i < remaining_stations; ++i) {
            if (distribution(rng)) {
                --remaining_stations;
            }
        }
        ++rounds;
    }
    return rounds;
}

// Generowanie wyników symulacji
void run_simulation(int start_n, int end_n, int step_n, int repetitions, double p,
                    std::vector<int> &n_values, std::vector<double> &avg_rounds, std::vector<std::vector<int>> &all_rounds) {
    std::random_device rd;
    std::mt19937 rng(rd());

    for (int n = start_n; n <= end_n; n += step_n) {
        int total_rounds = 0;
        std::vector<int> rounds_for_n;

        for (int rep = 0; rep < repetitions; ++rep) {
            int rounds = simulate_broadcast(n, p, rng);
            rounds_for_n.push_back(rounds);
            total_rounds += rounds;
        }

        n_values.push_back(n);
        avg_rounds.push_back(static_cast<double>(total_rounds) / repetitions);
        all_rounds.push_back(rounds_for_n);
    }
}

// Rysowanie wyników na wykresie
void plot_results(const std::string &filename, const std::string &title, const std::vector<int> &n_values,
                  const std::vector<std::vector<int>> &all_rounds, const std::vector<double> &avg_rounds, const std::string &ylabel) {
    matplot::figure();

    // Przygotowanie danych do rysowania
    std::vector<double> x_data;
    std::vector<double> y_data;

    for (size_t i = 0; i < all_rounds.size(); ++i) {
        for (size_t j = 0; j < all_rounds[i].size(); ++j) {
            x_data.push_back(static_cast<double>(n_values[i])); 
            y_data.push_back(static_cast<double>(all_rounds[i][j])); 
        }
    }

    // Rysowanie niebieskich punktów
    auto scatter_plot = matplot::scatter(x_data, y_data);
    scatter_plot->marker(".");
    scatter_plot->marker_face_color("blue");
    scatter_plot->marker_size(2);

    matplot::hold(matplot::on);

    // Rysowanie średniej czerwonej linii
    std::vector<double> n_values_d(n_values.begin(), n_values.end());
    auto avg_line = matplot::plot(n_values_d, avg_rounds, "r");
    avg_line->line_width(1.5);

    // Ustawienia wykresu
    matplot::title(title);
    matplot::xlabel("Liczba stacji (n)");
    matplot::ylabel(ylabel);
    matplot::grid(true);

    // Zapis wykresu
    matplot::save(filename);
    std::cout << "Wykres zapisany: " << filename << std::endl;
}

int main() {
    // Parametry symulacji
    const int start_n = 1000;       // Początkowa liczba stacji
    const int end_n = 100000;       // Końcowa liczba stacji
    const int step_n = 1000;        // Krok zwiększania liczby stacji
    const int repetitions = 50;    // Liczba powtórzeń dla każdego n

    // Wyniki dla p = 0.5
    std::vector<int> n_values_p1;
    std::vector<double> avg_rounds_p1;
    std::vector<std::vector<int>> all_rounds_p1;

    run_simulation(start_n, end_n, step_n, repetitions, 0.5, n_values_p1, avg_rounds_p1, all_rounds_p1);
    plot_results("T_p0_5.png", "Liczba rund nadawania (p = 0.5)", n_values_p1, all_rounds_p1, avg_rounds_p1, "Rundy");

    // Wyniki dla p = 0.1
    std::vector<int> n_values_p2;
    std::vector<double> avg_rounds_p2;
    std::vector<std::vector<int>> all_rounds_p2;

    run_simulation(start_n, end_n, step_n, repetitions, 0.1, n_values_p2, avg_rounds_p2, all_rounds_p2);
    plot_results("T_p0_1.png", "Liczba rund nadawania (p = 0.1)", n_values_p2, all_rounds_p2, avg_rounds_p2, "Rundy");

    std::cout << "Symulacja zakończona pomyślnie!" << std::endl;
    return 0;
}
