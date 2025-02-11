import numpy as np
import matplotlib.pyplot as plt
from typing import List, Callable, Tuple


class Coordinate:
    def __init__(self, x_value, y_value):
        self.x_value = x_value
        self.y_value = y_value


class MonteCarloEstimator:
    def __init__(self, func: Callable[[float], float], range_vals: Tuple[float, float], upper_bound: float):
        """
        Ustawienia początkowe dla estymatora Monte Carlo.
        :author: Anna Grelewska
        :param func: Funkcja, której całkę przybliżamy.
        :param range_vals: Krotka określająca przedział (start, koniec).
        :param upper_bound: Górny zakres wartości dla generowania punktów y.
        """
        self.func = func
        self.range_vals = range_vals
        self.upper_bound = upper_bound

    def generate_random_points(self, num_points: int) -> List[Coordinate]:
        """Generowanie losowych punktów w zakresie [a, b] x [0, M]."""
        generator = np.random.default_rng()
        return [
            Coordinate(
                generator.uniform(self.range_vals[0], self.range_vals[1]),
                generator.uniform(0, self.upper_bound)
            ) for _ in range(num_points)
        ]

    def is_point_under_curve(self, coord: Coordinate) -> bool:
        """Sprawdzenie, czy punkt znajduje się poniżej funkcji"""
        return coord.y_value <= self.func(coord.x_value)

    def count_points_below_curve(self, coords: List[Coordinate]) -> int:
        """Zliczanie punktów, które są pod krzywą funkcji"""
        return sum(self.is_point_under_curve(coord) for coord in coords)

    def estimate_area(self, num_points: int) -> float:
        """Obliczanie przybliżonego pola pod krzywą funkcji."""
        points = self.generate_random_points(num_points)
        points_below = self.count_points_below_curve(points)
        rectangle_area = (self.range_vals[1] - self.range_vals[0]) * self.upper_bound
        return (points_below / num_points) * rectangle_area


class PlotGenerator:
    @staticmethod
    def plot_results(points: List[Coordinate], averages: List[Coordinate], actual_value: float, title: str, subtitle: str, output_file: str = None):
        """
        Tworzenie wykresu z wynikami symulacji Monte Carlo.
        """
        plt.figure(figsize=(10, 6))
        plt.grid(True)

        # Wykres punktów indywidualnych
        plt.scatter([p.x_value for p in points], [p.y_value for p in points], color="purple", s=2, label="Wyniki prób")

        # Wykres średnich wyników
        plt.plot([m.x_value for m in averages], [m.y_value for m in averages], color="orange", linestyle='-', linewidth=2, label="Średnia wartość")

        # Dokładna wartość
        plt.axhline(y=actual_value, color='darkgreen', linestyle='--', linewidth=2, label="Prawdziwa wartość całki")

        # Tytuły wykresu
        plt.title(title, fontsize=12)
        plt.suptitle(subtitle, y=0.95, fontsize=10)
        plt.xlabel("Liczba punktów (n)")
        plt.ylabel("Przybliżona wartość")
        plt.legend()

        if output_file:
            plt.savefig(output_file)
        plt.show()


class Functions:
    @staticmethod
    def cube_root(x: float) -> float:
        """f(x) = x^(1/3)"""
        return x ** (1 / 3)

    @staticmethod
    def sine(x: float) -> float:
        """f(x) = sin(x)"""
        return np.sin(x)

    @staticmethod
    def polynomial(x: float) -> float:
        """f(x) = 4x(1 - x)^3"""
        return 4 * x * (1 - x) ** 3

    @staticmethod
    def quarter_circle(x: float) -> float:
        """f(x) = sqrt(1 - x^2), używane do aproksymacji liczby pi"""
        return 4 * np.sqrt(1 - x ** 2)


def execute_simulation(target_func, func_name, integration_range, max_y, n_values, repeats, exact_value, plot_title, plot_subtitle, filename_template):
    estimator = MonteCarloEstimator(target_func, integration_range, max_y)
    result_points = []
    mean_points = []

    min_n, max_n, step_n = n_values
    for n in range(min_n, max_n + step_n, step_n):
        sample_results = []
        for _ in range(repeats):
            approx_area = estimator.estimate_area(n)
            sample_results.append(Coordinate(n, approx_area))
        mean_value = np.mean([p.y_value for p in sample_results])
        mean_points.append(Coordinate(n, mean_value))
        result_points.extend(sample_results)

    # Tworzenie wykresu
    PlotGenerator.plot_results(result_points, mean_points, exact_value, plot_title, plot_subtitle, filename_template.format(repeats=repeats))


def main():
    # Parametry symulacji
    min_points, max_points, step_points = 50, 5000, 50

    # Funkcje do przetestowania
    test_cases = [
        (Functions.cube_root, "f(x) = x^(1/3)", (0, 8), 2, 12, "$f(x) = \\sqrt[3]{x}$"),
        (Functions.sine, "f(x) = sin(x)", (0, np.pi), 1, 2, "f(x) = \\sin(x)$"),
        (Functions.polynomial, "f(x) = 4x(1-x)^3", (0, 1), 0.421875, 0.2, "$f(x) = 4x(1-x)^3$"),
        (Functions.quarter_circle, "f(x) = 4*sqrt(1 - x^2)", (0, 1), 4, np.pi, "pi")
    ]

    repetitions = [5, 50]

    # Przeprowadzanie symulacji dla każdej funkcji i każdej liczby powtórzeń
    for func, desc, interval, max_y, exact, title in test_cases:
        for rep in repetitions:
            subtitle = f"Liczba prób: {rep}"
            clean_name = desc.replace(' ', '_').replace('=', '').replace('(', '').replace(')', '').replace('^', '').replace('/', '_')
            output_file = f"{clean_name}_repeats_{rep}.png"
            execute_simulation(
                target_func=func,
                func_name=desc,
                integration_range=interval,
                max_y=max_y,
                n_values=(min_points, max_points, step_points),
                repeats=rep,
                exact_value=exact,
                plot_title=title,
                plot_subtitle=subtitle,
                filename_template=output_file
            )


if __name__ == "__main__":
    main()
