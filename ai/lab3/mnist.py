import numpy as np
from tensorflow import keras
from keras import layers
import matplotlib.pyplot as plt

# Model / data parameters
num_classes = 10
input_shape = (28, 28, 1)
batch_size = 128
epochs = 15


# step 1:
# Load the data and split it between train and test sets
(x_train, y_train), (x_test, y_test) = keras.datasets.mnist.load_data()

# show 9 images in train dataset and 9 in test dataset
plt.figure(figsize=(10, 10))
for i in range(9):
    ax = plt.subplot(6, 3, i + 1)
    plt.imshow(x_train[i].astype("uint8"))
    plt.title(int(y_train[i]))
    plt.axis("off")
for i in range(9):
    ax = plt.subplot(6, 3, i+10)
    plt.imshow(x_test[i].astype("uint8"))
    plt.title(int(y_test[i]))
    plt.axis("off")
plt.show()

# Scale images to the [0, 1] range
x_train = x_train.astype("float32") / 255
x_test = x_test.astype("float32") / 255
# Make sure images have shape (28, 28, 1)
x_train = np.expand_dims(x_train, -1)
x_test = np.expand_dims(x_test, -1)
print("x_train shape:", x_train.shape)
print(x_train.shape[0], "train samples")
print(x_test.shape[0], "test samples")
#
# # convert class vectors to binary class matrices
y_train = keras.utils.to_categorical(y_train, num_classes)
y_test = keras.utils.to_categorical(y_test, num_classes)



# step 2:
# build model
model = keras.Sequential(
    [
        keras.Input(shape=input_shape),
        layers.Conv2D(64, kernel_size=(3, 3), activation="relu"),
        layers.MaxPooling2D(pool_size=(3, 3)),
        layers.Conv2D(32, kernel_size=(3, 3), padding="same", activation="relu"),
        layers.MaxPooling2D(pool_size=(2, 2), strides=1),
        layers.Flatten(),
        layers.Dropout(0.5),
        layers.Dense(num_classes, activation="softmax"),
    ]
)
model.summary()
model.compile(loss="categorical_crossentropy", optimizer="adam", metrics=["accuracy"])


# step 3:
# train the model
model.fit(x_train, y_train, batch_size=batch_size, epochs=epochs, validation_split=0.1)
#
#
# # step 4:
# # test the model
score = model.evaluate(x_test, y_test, verbose=0)
print("Test loss:", score[0])
print("Test accuracy:", score[1])

