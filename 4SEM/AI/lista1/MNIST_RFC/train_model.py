import numpy as np
from sklearn.ensemble import RandomForestClassifier
from keras.datasets import mnist
import joblib

# Loading data from the MNIST dataset
(x_train, y_train), (_, _) = mnist.load_data()

# Reshape the train dataset from 3D to 2D shape to feed into the sklearn model
x_train = x_train.reshape((x_train.shape[0], 784))

# Create and train random forest model
forest = RandomForestClassifier(n_estimators=150, criterion='gini', random_state=1)
forest.fit(x_train, y_train)

# Save model
joblib.dump(forest, 'random_forest_model.pkl')
print("Model trained and saved as random_forest_model.pkl")
