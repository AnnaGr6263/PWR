#include "utils.hpp"
#include <matplot/matplot.h>
#include <string>
#include <algorithm>
#include <cmath>

// Funkcja konwersji osi na double
template<typename T>
data_axis<double> convert_to_double(const data_axis<T>& input_axis) {
    data_axis<double> converted_axis;
    converted_axis.x = std::vector<double>(input_axis.x.begin(), input_axis.x.end());
    converted_axis.y = std::vector<double>(input_axis.y.begin(), input_axis.y.end());
    return converted_axis;
}

int main() {
    // Prefiks nazw plików wyjściowych
    std::string output_prefix = "Simulation_Plot_";

    // Symulacja dla d = 1
    full_simulation_results simulation_results_d1 = run_full_simulation(50, 1);

    // Wykresy dla d = 1
    draw_scatter_plot(convert_to_double(simulation_results_d1.raw_L), simulation_results_d1.avg_L, 
                      output_prefix + "1_L.png", "Maksymalne obciążenie (L), d=1", "blue", "red");

    auto create_ratio_plot = [&](const data_axis<double>& input_data, const std::string& file_name, 
                                 const std::string& plot_title, auto ratio_fn) {
        data_axis<double> ratio_data;
        ratio_data.x = input_data.x;
        std::transform(
            input_data.y.begin(),
            input_data.y.end(),
            input_data.x.begin(),
            std::back_inserter(ratio_data.y),
            ratio_fn
        );
        draw_line_plot(ratio_data, file_name, plot_title, "blue");
    };

    // Asymptotyka dla L(n) dla d=1
    create_ratio_plot(simulation_results_d1.avg_L, output_prefix + "1_f1.png", "L(1)_n / (ln n / ln ln n)", 
                      [](double l, double n) { return l / (std::log(n) / std::log(std::log(n))); });
    create_ratio_plot(simulation_results_d1.avg_L, output_prefix + "1_f2.png", "L(1)_n / (ln ln n / ln 2)", 
                      [](double l, double n) { return l / (std::log(std::log(n)) / std::log(2)); });

    // Symulacja dla d = 2
    full_simulation_results simulation_results_d2 = run_full_simulation(50, 2);

    // Wykresy dla d = 2
    draw_scatter_plot(convert_to_double(simulation_results_d2.raw_L), simulation_results_d2.avg_L, 
                      output_prefix + "2_L.png", "Maksymalne obciążenie (L), d=2", "blue", "red");

    // Asymptotyka dla L(n) dla d=2
    create_ratio_plot(simulation_results_d2.avg_L, output_prefix + "2_f1.png", "L(2)_n / (ln n / ln ln n)", 
                      [](double l, double n) { return l / (std::log(n) / std::log(std::log(n))); });
    create_ratio_plot(simulation_results_d2.avg_L, output_prefix + "2_f2.png", "L(2)_n / (ln ln n / ln 2)", 
                      [](double l, double n) { return l / (std::log(std::log(n)) / std::log(2)); });

    return 0;
}
