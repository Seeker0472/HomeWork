import numpy as np
import keras
from keras import layers
import keras_tuner


def build_model(hp):
    activation = hp.Choice(name="activation", values=["relu", "tanh"])
    dropout = hp.Float(name="dropout", min_value=0.1, max_value=0.7, step=0.2)
    lr = hp.Float(name="learning_rate", min_value=1e-4, max_value=1e-2, sampling="log")
    optimizer = hp.Choice(name="optimizer", values=["SGD", "Adam", "Adamax", "RMSprop"])

    model = keras.Sequential()
    for i in range(hp.Int("num_layers", 1, 3)):
        model.add(
            layers.Conv2D(
                filters=hp.Int(f"filters_{i}", min_value=32, max_value=128, step=32),
                kernel_size=hp.Int(f"kernel_size_{i}", min_value=2, max_value=4, step=1),
                activation=activation, padding="same"
            )
        )
        model.add(
            layers.MaxPooling2D(
                pool_size=hp.Int(f"pool_size_{i}", min_value=2, max_value=3, step=1), padding="same"
            )
        )
    model.add(layers.Flatten())
    if hp.Boolean("dropout_layer"):
        model.add(layers.Dropout(rate=dropout))
    model.add(layers.Dense(num_classes, activation="softmax"))
    model.compile(loss="categorical_crossentropy", optimizer=optimizer, metrics=["accuracy"])

    return model

# Model / data parameters
num_classes = 10
epochs = 15
batch_size = 128

# step 1:
# Load the data and split it between train and test sets
(x_train, y_train), (x_test, y_test) = keras.datasets.mnist.load_data()

# Scale images to the [0, 1] range
x_train = x_train.astype("float32") / 255
x_test = x_test.astype("float32") / 255
# Make sure images have shape (28, 28, 1)
x_train = np.expand_dims(x_train, -1)
x_test = np.expand_dims(x_test, -1)

# convert class vectors to binary class matrices
y_train = keras.utils.to_categorical(y_train, num_classes)
y_test = keras.utils.to_categorical(y_test, num_classes)


# step 2:
# build model
build_model(keras_tuner.HyperParameters())


# step 3:
# search the best model
tuner = keras_tuner.RandomSearch(
    hypermodel=build_model,
    objective="val_accuracy",
    max_trials=10,
    executions_per_trial=2,
    overwrite=True,
    directory="c:/edu/ai/",
    project_name="mnist",
)
tuner.search_space_summary()
tuner.search(x_train, y_train, batch_size=batch_size, epochs=epochs, validation_split=0.1, verbose=0)
tuner.results_summary()

# step 4:
# Get the best model.
models = tuner.get_best_models(num_models=1)
best_model = models[0]
# Build the model.
# Needed for `Sequential` without specified `input_shape`.
best_model.build(input_shape=(None, 28, 28, 1))
best_model.summary()


# step 5:
# test the model
score = best_model.evaluate(x_test, y_test, verbose=0)
print("Test loss:", score[0])
print("Test accuracy:", score[1])

