import numpy as np
import scipy.stats as stats
import pandas as pd

# Definiujemy wartości n
n_values = [100, 1000, 10000]

# Parametry rozkładu dwumianowego
p = 0.5

# Wyniki przechowywane w tabeli
results = []

for n in n_values:
    # Parametry oczekiwanej wartości i wariancji
    E_X = n * p  # E(X) = np
    Var_X = n * p * (1 - p)  # Var(X) = np(1-p)
    std_dev_X = np.sqrt(Var_X)  # Odchylenie standardowe

    threshold_a = 1.2 * E_X  # Próg na 1.2 * E(X)
    markov_a = E_X / threshold_a  # Nierówność Markowa: P(X ≥ a) ≤ E(X) / a
    chebyshev_a = 0.5 * (Var_X / (threshold_a - E_X) ** 2)  # Nierówność Czebyszewa
    exact_a = 1 - stats.binom.cdf(int(threshold_a) - 1, n, p)  # Dokładne prawdopodobieństwo

    threshold_b = 0.1 * E_X  # Próg na 0.1 * E(X)

    # Nierówność Markowa
    markov_b = 2 * (E_X / (E_X + threshold_b))

    # Nierówność Czebyszewa
    chebyshev_b = Var_X / (threshold_b ** 2)

    # Dokładne wartości prawdopodobieństwa
    exact_b = stats.binom.cdf(int(E_X - threshold_b), n, p) + (1 - stats.binom.cdf(int(E_X + threshold_b) - 1, n, p))

    # Dodajemy wyniki do tabeli
    results.append([n, markov_a, chebyshev_a, exact_a, markov_b, chebyshev_b, exact_b])

# Tworzymy tabelę wyników
columns = ["n", "Markov P(X ≥ 1.2E(X))", "Chebyshev P(X ≥ 1.2E(X))", "Exact P(X ≥ 1.2E(X))",
           "Markov P(|X - E(X)| ≥ 0.1E(X))", "Chebyshev P(|X - E(X)| ≥ 0.1E(X))", "Exact P(|X - E(X)| ≥ 0.1E(X))"]

df_results = pd.DataFrame(results, columns=columns)

# Ustawienia Pandas dla pełnego wyświetlania
pd.set_option("display.max_columns", None)  # Wyświetlanie wszystkich kolumn
pd.set_option("display.width", 1000)  # Szerokość terminala
pd.set_option("display.float_format", '{:.10f}'.format)  # Wyświetlanie większej precyzji wartości


print(df_results)
df_results.to_csv("wyniki_zadanie1.csv", index=False)
print("\n Wyniki zostały zapisane do pliku 'wyniki_zadanie1.csv'.")
