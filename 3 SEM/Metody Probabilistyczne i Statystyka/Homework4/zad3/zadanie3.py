import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


# Funkcja generująca jedną realizację błądzenia losowego
def generate_random_walk(N):
    steps = np.random.choice([-1, 1], size=N)  # Kroki ±1
    S_N = np.cumsum(steps)  # Sumowanie kroków
    return S_N


# Funkcja obliczająca frakcję czasu nad osią OX
def compute_fraction_above_ox(S_N):
    N = len(S_N)
    D_N = (S_N > 0) | (S_N == 0)  # Warunek Dn = 1, jeśli S_N >= 0
    L_N = np.sum(D_N)  # Liczenie czasu nad osią OX
    return L_N / N  # Frakcja czasu


# Lista wartości N do analizy
N_values = [100, 1000, 10000]
num_samples = 5000  # Liczba realizacji procesu

# Wyniki do przechowania
results = []

for N in N_values:
    P_N_samples = []  # Lista dla frakcji czasu

    # Generowanie trajektorii i liczenie P_N próbka po próbce
    for _ in range(num_samples):
        S_N = generate_random_walk(N)
        P_N_samples.append(compute_fraction_above_ox(S_N))  # Obliczenie frakcji czasu

    # Tworzenie histogramu dopasowanego do wzorca
    plt.figure(figsize=(8, 6))
    plt.hist(P_N_samples, bins=20, density=True, alpha=0.7, color="blue", edgecolor="black",
             label="Empiryczna gęstość P_N")

    # Dodanie teoretycznej gęstości rozkładu arcus sinus
    x_values = np.linspace(0.01, 0.99, 1000)  # Unikanie 0 i 1, aby uniknąć dzielenia przez 0
    arcus_sinus_pdf = 1 / (np.pi * np.sqrt(x_values * (1 - x_values)))  # Gęstość rozkładu arcus sinus

    plt.plot(x_values, arcus_sinus_pdf, linestyle="dashed", color="red", linewidth=2, label="Rozkład arcus sinus")
    plt.xlabel("Frakcja czasu P_N")
    plt.ylabel("Gęstość prawdopodobieństwa (PDF)")
    plt.title(f"Histogram frakcji czasu nad osią OX (N = {N})")
    plt.legend()
    plt.grid()
    plt.show()

    # Zapis wyników do analizy
    mean_P_N = np.mean(P_N_samples)
    var_P_N = np.var(P_N_samples)
    results.append([N, mean_P_N, var_P_N])

# Tworzenie tabeli wyników
columns = ["N", "E(P_N)", "Var(P_N)"]
df_results = pd.DataFrame(results, columns=columns)

# Wyświetlenie tabeli w terminalu
print(df_results)

# Zapisanie wyników do pliku CSV
df_results.to_csv("wyniki_bledzenia_losowego_OX_final.csv", index=False)
print("\n Wyniki zostały zapisane do pliku 'wyniki_bledzenia_losowego_OX_final.csv'")
