import numpy as np
import cv2
import os
import keras
from keras.models import load_model
import matplotlib.pyplot as plt
from sklearn.metrics import classification_report, confusion_matrix
import seaborn as sns

# Load the trained model
model = load_model("mnist_model.h5")
print("Model loaded.")

# Path to the folder with test images
test_folder = "My_test_set"

# List to store data
test_images = []
true_labels = []

# Load each file in the folder
for filename in os.listdir(test_folder):
    if filename.endswith(".png"):  # Ensure only PNG files are loaded
        # Read the label from the filename (e.g., "3.1.png" -> digit 3)
        label = int(filename.split(".")[0])
        
        # Load the image in grayscale
        img = cv2.imread(os.path.join(test_folder, filename), cv2.IMREAD_GRAYSCALE)

        # Invert colors (negative)
        img = 255 - img

        # Normalize to the range [0,1]
        img = img / 255.0

        # Flatten to a 1x784 vector
        img = img.reshape(-1)  

        # Save the image and label
        test_images.append(img)
        true_labels.append(label)

# Convert lists to NumPy arrays
test_images = np.array(test_images)
true_labels = np.array(true_labels)

print("Loaded {} test images.".format(len(test_images)))

# Model prediction
predictions = model.predict(test_images)
predicted_labels = np.argmax(predictions, axis=1)

# Display sample images
plt.figure(figsize=(10, 5))
for i in range(10):
    plt.subplot(2, 5, i + 1)
    plt.imshow(test_images[i].reshape(28, 28), cmap='gray')
    plt.title(f"P: {predicted_labels[i]}, T: {true_labels[i]}")
    plt.axis('off')

plt.show()

# Calculate accuracy
accuracy = np.mean(predicted_labels == true_labels)
print(f"Model accuracy on custom data: {accuracy:.2%}")

# Confusion matrix
confusion_mtx = confusion_matrix(true_labels, predicted_labels)

# Visualize the confusion matrix
plt.figure(figsize=(10, 7))
sns.heatmap(confusion_mtx, annot=True, fmt='d', cmap="Blues", xticklabels=range(10), yticklabels=range(10))
plt.xlabel('Prediction')
plt.ylabel('Actual')
plt.title('Confusion Matrix for Custom Test Set')
plt.show()

# Classification report
print("\nClassification Report:\n")
print(classification_report(true_labels, predicted_labels))

