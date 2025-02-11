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

    // Przeprowadzenie pełnej symulacji
    full_simulation_results simulation_results = run_full_simulation(50);

    // Utworzenie różnicy D(n) - C(n)
    data_axis<int> difference_axis;
    difference_axis.x = simulation_results.raw_D.x;
    std::transform(
        simulation_results.raw_D.y.begin(),
        simulation_results.raw_D.y.end(),
        simulation_results.raw_C.y.begin(),
        std::back_inserter(difference_axis.y),
        [](int d, int c) { return d - c; }
    );

    // Konwersja różnicy na oś typu double
    data_axis<double> difference_axis_double = convert_to_double(difference_axis);

    // Średnie wartości różnicy D - C
    data_axis<double> avg_difference_axis;
    avg_difference_axis.x = simulation_results.avg_D.x;
    std::transform(
        simulation_results.avg_D.y.begin(),
        simulation_results.avg_D.y.end(),
        simulation_results.avg_C.y.begin(),
        std::back_inserter(avg_difference_axis.y),
        [](double avg_d, double avg_c) { return avg_d - avg_c; }
    );

    // Wykresy główne
    draw_scatter_plot(convert_to_double(simulation_results.raw_B), simulation_results.avg_B, 
                      output_prefix + "B.png", "Pierwsza kolizja (B)", "blue", "red");
    draw_scatter_plot(convert_to_double(simulation_results.raw_U), simulation_results.avg_U, 
                      output_prefix + "U.png", "Puste urny po N kulach (U)", "blue", "red");
    draw_scatter_plot(convert_to_double(simulation_results.raw_C), simulation_results.avg_C, 
                      output_prefix + "C.png", "Każda urna z co najmniej jedną kulą (C)", "blue", "red");
    draw_scatter_plot(convert_to_double(simulation_results.raw_D), simulation_results.avg_D, 
                      output_prefix + "D.png", "Każda urna z co najmniej dwiema kulami (D)", "blue", "red");
    draw_scatter_plot(difference_axis_double, avg_difference_axis, 
                      output_prefix + "Difference_D_C.png", "Różnica D - C", "blue", "red");

    // Wykresy szczegółowe
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

    // a) B(n) / n oraz B(n) / sqrt(n)
    create_ratio_plot(simulation_results.avg_B, output_prefix + "1a.png", "B(n) / n", 
                      [](double b, double n) { return b / n; });
    create_ratio_plot(simulation_results.avg_B, output_prefix + "2a.png", "B(n) / sqrt(n)", 
                      [](double b, double n) { return b / std::sqrt(n); });

    // b) U(n) / n
    create_ratio_plot(simulation_results.avg_U, output_prefix + "1b.png", "U(n) / n", 
                      [](double u, double n) { return u / n; });

    // c) C(n) / n, C(n) / (n ln n), C(n) / n^2
    create_ratio_plot(simulation_results.avg_C, output_prefix + "1c.png", "C(n) / n", 
                      [](double c, double n) { return c / n; });
    create_ratio_plot(simulation_results.avg_C, output_prefix + "2c.png", "C(n) / (n ln n)", 
                      [](double c, double n) { return c / (n * std::log(n)); });
    create_ratio_plot(simulation_results.avg_C, output_prefix + "3c.png", "C(n) / n^2", 
                      [](double c, double n) { return c / (n * n); });

    // d) D(n) / n, D(n) / (n ln n), D(n) / n^2
    create_ratio_plot(simulation_results.avg_D, output_prefix + "1d.png", "D(n) / n", 
                      [](double d, double n) { return d / n; });
    create_ratio_plot(simulation_results.avg_D, output_prefix + "2d.png", "D(n) / (n ln n)", 
                      [](double d, double n) { return d / (n * std::log(n)); });
    create_ratio_plot(simulation_results.avg_D, output_prefix + "3d.png", "D(n) / n^2", 
                      [](double d, double n) { return d / (n * n); });

    // e) (D(n) - C(n)) / n, (D(n) - C(n)) / (n ln n), (D(n) - C(n)) / (n ln ln n)
    create_ratio_plot(avg_difference_axis, output_prefix + "1e.png", "(D(n) - C(n)) / n", 
                      [](double diff, double n) { return diff / n; });
    create_ratio_plot(avg_difference_axis, output_prefix + "2e.png", "(D(n) - C(n)) / (n ln n)", 
                      [](double diff, double n) { return diff / (n * std::log(n)); });
    create_ratio_plot(avg_difference_axis, output_prefix + "3e.png", "(D(n) - C(n)) / (n ln ln n)", 
                      [](double diff, double n) { return diff / (n * std::log(std::log(n))); });

    return 0;
}
