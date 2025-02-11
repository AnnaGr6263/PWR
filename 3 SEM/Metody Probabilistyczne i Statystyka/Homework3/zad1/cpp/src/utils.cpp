#include "utils.hpp"
#include <matplot/matplot.h>
#include <iostream>
#include <cmath>

// Generowanie wykresu punktowego
void draw_scatter_plot(const data_axis<double>& data, const data_axis<double>& avg_data, 
                       const std::string& file, const std::string& title, 
                       const std::string& point_color, const std::string& line_color) {
    std::cout << "Saving scatter plot: " << file << std::endl;
    matplot::scatter(data.x, data.y)->marker(".").marker_size(1.5).color(point_color);
    matplot::hold(matplot::on);
    matplot::plot(avg_data.x, avg_data.y)->line_width(2).color(line_color);
    matplot::title(title);
    matplot::xlabel("Liczba kul (N)");
    matplot::ylabel("Wartość");
    matplot::grid(true);
    matplot::hold(matplot::off);
    matplot::save(file);
    std::cout << "Scatter plot saved: " << file << std::endl;
}

// Generowanie wykresu liniowego
void draw_line_plot(const data_axis<double>& data, const std::string& file, 
                    const std::string& title, const std::string& line_color) {
    std::cout << "Saving line plot: " << file << std::endl;
    matplot::plot(data.x, data.y)->line_width(2).color(line_color);
    matplot::title(title);
    matplot::xlabel("Liczba kul (N)");
    matplot::ylabel("Wartość");
    matplot::grid(true);
    matplot::save(file);
    std::cout << "Line plot saved: " << file << std::endl;
}

// Pojedyncza symulacja
single_simulation_results run_single_simulation(int n, int d) {
    single_simulation_results sim_results{0, 0, 0, 0, 0};

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> urn_dist(0, n - 1);

    std::vector<int> urns(n, 0);
    int total_throws = 0;
    int single_ball_bins = 0, double_ball_bins = 0;

    auto get_index_ballanced = [&](std::uniform_int_distribution<int>& dist, std::mt19937& rng, const std::vector<int>& bins, int d) {
        std::set<int> unique_index;
        while (unique_index.size() < d) {
            unique_index.insert(dist(rng));
        }
        return *std::min_element(unique_index.begin(), unique_index.end(),
            [&bins](int a, int b) { return bins[a] < bins[b]; });
    };

    for (int i = 0; i < n; ++i) {
        int urn = get_index_ballanced(urn_dist, rng, urns, d);
        ++total_throws;

        if (urns[urn] == 0) {
            ++single_ball_bins;
        } else if (urns[urn] == 1) {
            if (sim_results.first_collision == 0) {
                sim_results.first_collision = total_throws;
            }
            ++double_ball_bins;
        }
        ++urns[urn];
    }

    sim_results.empty_bins = n - single_ball_bins;
    sim_results.max_load = *std::max_element(urns.begin(), urns.end());

    while (single_ball_bins < n) {
        int urn = get_index_ballanced(urn_dist, rng, urns, d);
        ++total_throws;

        if (urns[urn] == 0) {
            ++single_ball_bins;
        } else if (urns[urn] == 1) {
            ++double_ball_bins;
        }
        ++urns[urn];
    }

    sim_results.filled_bins = total_throws;

    while (double_ball_bins < n) {
        int urn = get_index_ballanced(urn_dist, rng, urns, d);
        ++total_throws;

        if (urns[urn] == 1) {
            ++double_ball_bins;
        }
        ++urns[urn];
    }

    sim_results.double_filled_bins = total_throws;

    return sim_results;
}

// Pełna symulacja
full_simulation_results run_full_simulation(int trials, int d) {
    full_simulation_results all_results;

    for (int n = 1000; n <= 100000; n += 1000) {
        std::cout << "Simulating for n = " << n << ", d = " << d << std::endl;
        for (int t = 0; t < trials; ++t) {
            single_simulation_results sim = run_single_simulation(n, d);
            all_results.append_data(n, sim);
        }
    }

    all_results.compute_averages(trials);
    return all_results;
}

// Dodawanie danych do pełnej symulacji
void full_simulation_results::append_data(int n, const single_simulation_results& result) {
    raw_B.x.push_back(n);
    raw_B.y.push_back(result.first_collision);

    raw_L.x.push_back(n);
    raw_L.y.push_back(result.max_load);
}

// Obliczanie średnich wyników
void full_simulation_results::compute_averages(int trials) {
    for (size_t i = 0; i < raw_B.x.size(); i += trials) {
        avg_B.x.push_back(raw_B.x[i]);
        avg_B.y.push_back(std::accumulate(raw_B.y.begin() + i, raw_B.y.begin() + i + trials, 0.0) / trials);

        avg_L.x.push_back(raw_L.x[i]);
        avg_L.y.push_back(std::accumulate(raw_L.y.begin() + i, raw_L.y.begin() + i + trials, 0.0) / trials);
    }
}
