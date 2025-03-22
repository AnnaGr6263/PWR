import numpy as np
import matplotlib.pyplot as plt
import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout

# Setting random seed for reproducibility
np.random.seed(0)

# Loading data from the MNIST dataset
from keras.datasets import mnist
(x_train, y_train), (x_test, y_test) = mnist.load_data()

# Displaying the shapes of training and test datasets
print(x_train.shape, y_train.shape) 
print(x_test.shape, y_test.shape) 

# Visualize Examples
num_classes = 10
f, ax = plt.subplots(1, num_classes, figsize=(20, 20))

for i in range(0, num_classes):
     # Retrieve the first image for each class
    sample = x_train[y_train == i][0]
    ax[i].imshow(sample, cmap='gray')
    ax[i].set_title('Label: {}'.format(i), fontsize=16)

# Display the first 10 labels before converting to one-hot encoding
for i in range (10):
    print(y_train[i])
    
# Convert labels to one-hot encoding
y_train = keras.utils.to_categorical(y_train, num_classes)
y_test = keras.utils.to_categorical(y_test, num_classes)

for i in range (10):
    print(y_train[i])

# Normalize data (0-255 → 0-1)
x_train =x_train / 255.0
x_test = x_test / 255.0

# Flatten 28x28 images to 784 values
x_train = x_train.reshape(x_train.shape[0], -1)
x_test = x_test.reshape(x_test.shape[0], -1)
print(x_train.shape)

# Creating the model
model = Sequential()
model.add(Dense(units=128,input_shape=(784,), activation='relu'))
model.add(Dense(units=128, activation='relu'))
model.add(Dropout(0.25))
model.add(Dense(units=10, activation='softmax'))

model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
model.summary() # Print model summary

# Train Model
batch_size = 512 # number of samples in one batch
epochs = 10 # Number of epochs - full passes through the training data
model.fit(x=x_train, y=y_train, batch_size=batch_size, epochs=epochs)

model.save("mnist_model.h5")
print("Model saved as 'mnist_model.h5'")