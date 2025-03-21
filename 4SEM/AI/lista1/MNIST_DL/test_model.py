import numpy as np
import matplotlib.pyplot as plt
import keras
from keras.models import load_model
from keras.datasets import mnist
from sklearn.metrics import classification_report, confusion_matrix
import seaborn as sns

# Załadowanie zbioru testowego MNIST
(_, _), (x_test, y_test) = mnist.load_data()

# Normalizacja danych
x_test = x_test / 255.0

# Spłaszczenie obrazków
x_test = x_test.reshape(x_test.shape[0], -1)

# Załadowanie wytrenowanego modelu
model = load_model("mnist_model.h5")
print("Model załadowany.")

# Ewaluacja modelu na zbiorze testowym
test_loss, test_acc = model.evaluate(x_test, keras.utils.to_categorical(y_test, 10))
print(f"Test Loss: {test_loss}, Test Accuracy: {test_acc}")

# Przewidywanie klas dla zbioru testowego
y_pred = model.predict(x_test)
y_pred_classes = np.argmax(y_pred, axis=1)

# Obliczenie metryk: czułość (recall) i precyzja (precision)
print("\nClassification Report:\n")
print(classification_report(y_test, y_pred_classes))

# Macierz pomyłek
confusion_mtx = confusion_matrix(y_test, y_pred_classes)

# Wizualizacja macierzy pomyłek
plt.figure(figsize=(10, 7))
sns.heatmap(confusion_mtx, annot=True, fmt='d', cmap="Blues", xticklabels=range(10), yticklabels=range(10))
plt.xlabel('Predicted Label')
plt.ylabel('True Label')
plt.title('Confusion Matrix')
plt.show()
