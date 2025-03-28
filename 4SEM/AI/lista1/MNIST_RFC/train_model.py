import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, classification_report
from keras.datasets import mnist
import joblib

# Loading data from the MNIST dataset
(x_full, y_full), (_, _) = mnist.load_data()

# Reshape the dataset shape to feed into sklearn (70000, 784)
x_full = x_full.reshape((x_full.shape[0], 784))

# Split the data into training and validation sets (80% train, 20% val), divide into 150 trees
x_train, x_val, y_train, y_val = train_test_split(x_full, y_full, test_size=0.2, random_state=1)

# Create and train random forest model, building trees with Gini impurity
forest = RandomForestClassifier(n_estimators=150, criterion='gini', random_state=1)
forest.fit(x_train, y_train)

# Evaluate model on validation set
y_val_pred = forest.predict(x_val)
val_accuracy = accuracy_score(y_val, y_val_pred)

print(f"Validation accuracy: {val_accuracy:.4f}")
print("\nClassification Report (Validation Set):")
print(classification_report(y_val, y_val_pred))

# Save model
joblib.dump(forest, 'random_forest_model.pkl')
print("Model trained and saved as random_forest_model.pkl")
