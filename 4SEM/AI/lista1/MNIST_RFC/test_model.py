import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix, ConfusionMatrixDisplay
from keras.datasets import mnist
import joblib
from sklearn.ensemble import RandomForestClassifier  # Required for loading the model
import random

# Load the pre-trained Random Forest model
forest = joblib.load('random_forest_model.pkl')
print("Random Forest model loaded.")

# Load the MNIST test dataset
(_, _), (x_test, y_test) = mnist.load_data()

# Reshape the test dataset from 28x28 images to 1D arrays of size 784
x_test = x_test.reshape((x_test.shape[0], 784))

# Predict class labels for the test dataset
y_pred = forest.predict(x_test)

# Calculate and print the accuracy of the model
accuracy = accuracy_score(y_test, y_pred)
print("Accuracy:", accuracy)

# Generate a confusion matrix to analyze misclassifications
cm = confusion_matrix(y_test, y_pred)

# Visualize the confusion matrix using a heatmap
disp = ConfusionMatrixDisplay(confusion_matrix=cm)
fig, ax = plt.subplots(figsize=(15, 10))
disp.plot(ax=ax)
plt.title("Confusion Matrix - Random Forest on MNIST")
plt.show()

# Print a detailed classification report (precision, recall, F1-score)
print("\nClassification Report:\n")
print(classification_report(y_test, y_pred, digits=4))

# Display 10 random test images along with their true and predicted labels
random_indexes = random.sample(range(len(x_test)), 10)
plt.figure(figsize=(15, 5))
for i, idx in enumerate(random_indexes):
    image = x_test[idx].reshape(28, 28)  # Reshape back to 28x28 for visualization
    plt.subplot(2, 5, i + 1)
    plt.imshow(image, cmap='gray')
    plt.title(f"True: {y_test[idx]}\nPred: {y_pred[idx]}")  # True and predicted labels
    plt.axis('off')  # Remove axes for better visualization

plt.tight_layout()
plt.show()
