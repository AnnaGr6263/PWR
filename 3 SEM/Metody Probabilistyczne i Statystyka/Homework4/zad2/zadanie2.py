import numpy as np
import scipy.stats as stats
import matplotlib.pyplot as plt
import pandas as pd


def generate_random_walk(N, num_samples=10000):
    """Generuje próbki dla zmiennej losowej S_N."""
    steps = np.random.choice([-1, 1], size=(num_samples, N))
    S_N = np.sum(steps, axis=1)  # Sumowanie kroków
    return S_N

# Lista wartości N do analizy
N_values = [5, 10, 15, 20, 25, 30, 100]
num_samples = 10000  # Liczba próbek

# Wyniki do porównania
results = []

# Generowanie i analiza dystrybuanty dla różnych N
for N in N_values:
    S_N_samples = generate_random_walk(N, num_samples)
    sorted_SN = np.sort(S_N_samples)  # Sortowanie wartości S_N
    empirical_cdf = np.arange(1, num_samples + 1) / num_samples

    # Aproksymacja normalna
    normal_cdf = stats.norm.cdf(sorted_SN, loc=0, scale=np.sqrt(N))

    # Tworzenie wykresu empirycznej dystrybuanty
    plt.figure(figsize=(8, 6))
    plt.step(sorted_SN, empirical_cdf, where="mid", label="Empiryczna dystrybuanta", color="blue")
    plt.plot(sorted_SN, normal_cdf, linestyle="dashed", label="Dystrybuanta normalna", color="orange")
    plt.xlabel("$S_N$")
    plt.ylabel("Dystrybuanta")
    plt.title(f"Porównanie dystrybuant dla N={N}")
    plt.legend()
    plt.grid()
    plt.show()

    # Zapis wyników do tabeli
    mean_S_N = np.mean(S_N_samples)
    var_S_N = np.var(S_N_samples)
    results.append([N, mean_S_N, var_S_N])

# Tworzenie tabeli wyników
columns = ["N", "E(S_N)", "Var(S_N)"]
df_results = pd.DataFrame(results, columns=columns)

# Wyświetlenie tabeli w terminalu
print(df_results)

# Zapisanie wyników do pliku CSV
df_results.to_csv("wyniki_bledzenia_losowego.csv", index=False)
print("\n Wyniki zostały zapisane do pliku 'wyniki_bledzenia_losowego.csv'")
