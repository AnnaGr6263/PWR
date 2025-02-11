#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <cmath>
#include <matplot/matplot.h>
#include <set>
#include <algorithm>

// Struktura przechowująca dane dla osi wykresu
template<typename T>
struct data_axis {
    std::vector<T> x; // Wartości na osi X
    std::vector<T> y; // Wartości na osi Y
};

// Wyniki pojedynczej symulacji
struct single_simulation_results {
    int first_collision;      // Moment pierwszej kolizji
    int empty_bins;           // Liczba pustych urn
    int filled_bins;          // Moment, gdy każda urna ma co najmniej jedną kulę
    int double_filled_bins;   // Moment, gdy każda urna ma co najmniej dwie kule
    int max_load;             // Maksymalne zapełnienie urny (L)
};

// Wyniki pełnej symulacji
struct full_simulation_results {
    data_axis<int> raw_B;       // Surowe dane B(n)
    data_axis<double> avg_B;    // Średnie wartości B(n)

    data_axis<int> raw_U;       // Surowe dane U(n)
    data_axis<double> avg_U;    // Średnie wartości U(n)

    data_axis<int> raw_C;       // Surowe dane C(n)
    data_axis<double> avg_C;    // Średnie wartości C(n)

    data_axis<int> raw_D;       // Surowe dane D(n)
    data_axis<double> avg_D;    // Średnie wartości D(n)

    data_axis<int> raw_L;       // Surowe dane L(n)
    data_axis<double> avg_L;    // Średnie wartości L(n)

    void append_data(int n, const single_simulation_results& result); // Dodaj dane
    void compute_averages(int trials); // Oblicz średnie wartości
};

// Deklaracje funkcji
single_simulation_results run_single_simulation(int n, int d); 
full_simulation_results run_full_simulation(int trials, int d);

void draw_scatter_plot(const data_axis<double>& data, const data_axis<double>& avg_data, 
                       const std::string& file, const std::string& title, 
                       const std::string& point_color, const std::string& line_color);

void draw_line_plot(const data_axis<double>& data, const std::string& file, 
                    const std::string& title, const std::string& line_color);

#endif
