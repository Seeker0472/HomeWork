# 人工智能试验报告-Lab3

---
学号: 2022211100 班级:2220701 姓名:郭明轩

---

## 实验目的

- 掌握构建卷积神经网络的方法
- 理解超参数对卷积神经网络结构的影响
- 掌握神经网络模型调优的方法
- 完成手写数字识别模型调优,获得性能最佳的模型

## 实验环境

```sh
(myvenv) root@54cf208e6479:~/py# pip list
Package                      Version
---------------------------- -----------
absl-py                      2.1.0
astunparse                   1.6.3
cachetools                   5.5.0
certifi                      2024.8.30
charset-normalizer           3.4.0
cycler                       0.11.0
flatbuffers                  24.3.25
fonttools                    4.38.0
gast                         0.4.0
google-auth                  2.35.0
google-auth-oauthlib         0.4.6
google-pasta                 0.2.0
grpcio                       1.62.3
h5py                         3.8.0
idna                         3.10
importlib-metadata           6.7.0
keras                        2.10.0
Keras-Preprocessing          1.1.2
keras-tuner                  1.4.7
kiwisolver                   1.4.5
kt-legacy                    1.0.5
libclang                     18.1.1
Markdown                     3.4.4
MarkupSafe                   2.1.5
matplotlib                   3.5.3
numpy                        1.21.6
oauthlib                     3.2.2
opt-einsum                   3.3.0
packaging                    24.0
Pillow                       9.5.0
pip                          23.0.1
protobuf                     3.19.6
pyasn1                       0.5.1
pyasn1-modules               0.3.0
pyparsing                    3.1.4
python-dateutil              2.9.0.post0
requests                     2.31.0
requests-oauthlib            2.0.0
rsa                          4.9
setuptools                   47.1.0
six                          1.16.0
tensorboard                  2.10.1
tensorboard-data-server      0.6.1
tensorboard-plugin-wit       1.8.1
tensorflow                   2.10.0
tensorflow-estimator         2.10.0
tensorflow-io-gcs-filesystem 0.34.0
termcolor                    2.3.0
typing_extensions            4.7.1
urllib3                      2.0.7
Werkzeug                     2.2.3
wheel                        0.42.0
wrapt                        1.16.0
zipp                         3.15.0
```

## 关键代码

### mnist.py

```py
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


```

### mnist-tuner.py

```py

import numpy as np
import keras
from keras import layers
import keras_tuner


def build_model(hp):
    activation = hp.Choice(name="activation", values=["relu", "tanh"])
    dropout = hp.Float(name="dropout", min_value=0.1, max_value=0.5, step=0.2)
    lr = hp.Float(name="learning_rate", min_value=1e-4, max_value=1e-3, sampling="log")
    optimizer = hp.Choice(name="optimizer", values=["SGD", "Adam", ])

    model = keras.Sequential()
    for i in range(hp.Int("num_layers", 1, 2)):
        model.add(
            layers.Conv2D(
                filters=hp.Int(f"filters_{i}", min_value=32, max_value=64, step=32),
                kernel_size=hp.Int(f"kernel_size_{i}", min_value=2, max_value=4, step=2),
                activation=activation, padding="same"
            )
        )
        model.add(
            layers.MaxPooling2D(
                pool_size=hp.Int(f"pool_size_{i}", min_value=2, max_value=2, step=1), padding="same"
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
epochs = 10
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
tuner = keras_tuner.GridSearch(
    hypermodel=build_model,
    objective="val_accuracy",
    max_trials=10,
    executions_per_trial=2,
    overwrite=True,
    directory="ai/",
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


```

## 结果

```sh
To enable them in other operations, rebuild TensorFlow with the appropriate compiler flags.
Search space summary
Default search space size: 9
activation (Choice)
{'default': 'relu', 'conditions': [], 'values': ['relu', 'tanh'], 'ordered': False}
dropout (Float)
{'default': 0.1, 'conditions': [], 'min_value': 0.1, 'max_value': 0.5, 'step': 0.2, 'sampling': 'linear'}
learning_rate (Float)
{'default': 0.0001, 'conditions': [], 'min_value': 0.0001, 'max_value': 0.001, 'step': None, 'sampling': 'log'}
optimizer (Choice)
{'default': 'SGD', 'conditions': [], 'values': ['SGD', 'Adam'], 'ordered': False}
num_layers (Int)
{'default': None, 'conditions': [], 'min_value': 1, 'max_value': 2, 'step': 1, 'sampling': 'linear'}
filters_0 (Int)
{'default': None, 'conditions': [], 'min_value': 32, 'max_value': 64, 'step': 32, 'sampling': 'linear'}
kernel_size_0 (Int)
{'default': None, 'conditions': [], 'min_value': 2, 'max_value': 4, 'step': 2, 'sampling': 'linear'}
pool_size_0 (Int)
{'default': None, 'conditions': [], 'min_value': 2, 'max_value': 2, 'step': 1, 'sampling': 'linear'}
dropout_layer (Boolean)
{'default': False, 'conditions': []}
Results summary
Results in ai/mnist
Showing 10 best trials
Objective(name="val_accuracy", direction="max")

Trial 09 summary
Hyperparameters:
activation: relu
dropout: 0.1
learning_rate: 0.0001
optimizer: SGD
num_layers: 2
filters_0: 32
kernel_size_0: 2
pool_size_0: 2
dropout_layer: False
filters_1: 32
kernel_size_1: 4
pool_size_1: 2
Score: 0.9729166626930237

Trial 08 summary
Hyperparameters:
activation: relu
dropout: 0.1
learning_rate: 0.0001
optimizer: SGD
num_layers: 2
filters_0: 32
kernel_size_0: 2
pool_size_0: 2
dropout_layer: False
filters_1: 32
kernel_size_1: 2
pool_size_1: 2
Score: 0.9597499966621399

Trial 02 summary
Hyperparameters:
activation: relu
dropout: 0.1
learning_rate: 0.0001
optimizer: SGD
num_layers: 1
filters_0: 32
kernel_size_0: 4
pool_size_0: 2
dropout_layer: False
Score: 0.9448333382606506

Trial 06 summary
Hyperparameters:
activation: relu
dropout: 0.1
learning_rate: 0.0001
optimizer: SGD
num_layers: 1
filters_0: 64
kernel_size_0: 4
pool_size_0: 2
dropout_layer: False
Score: 0.94200000166893

Trial 03 summary
Hyperparameters:
activation: relu
dropout: 0.1
learning_rate: 0.0001
optimizer: SGD
num_layers: 1
filters_0: 32
kernel_size_0: 4
pool_size_0: 2
dropout_layer: True
Score: 0.940750002861023

Trial 07 summary
Hyperparameters:
activation: relu
dropout: 0.1
learning_rate: 0.0001
optimizer: SGD
num_layers: 1
filters_0: 64
kernel_size_0: 4
pool_size_0: 2
dropout_layer: True
Score: 0.940750002861023

Trial 01 summary
Hyperparameters:
activation: relu
dropout: 0.1
learning_rate: 0.0001
optimizer: SGD
num_layers: 1
filters_0: 32
kernel_size_0: 2
pool_size_0: 2
dropout_layer: True
Score: 0.9320000112056732

Trial 00 summary
Hyperparameters:
activation: relu
dropout: 0.1
learning_rate: 0.0001
optimizer: SGD
num_layers: 1
filters_0: 32
kernel_size_0: 2
pool_size_0: 2
dropout_layer: False
Score: 0.9317500293254852

Trial 04 summary
Hyperparameters:
activation: relu
dropout: 0.1
learning_rate: 0.0001
optimizer: SGD
num_layers: 1
filters_0: 64
kernel_size_0: 2
pool_size_0: 2
dropout_layer: False
Score: 0.9305000007152557

Trial 05 summary
Hyperparameters:
activation: relu
dropout: 0.1
learning_rate: 0.0001
optimizer: SGD
num_layers: 1
filters_0: 64
kernel_size_0: 2
pool_size_0: 2
dropout_layer: True
Score: 0.9302500188350677
Model: "sequential"
_________________________________________________________________
 Layer (type)                Output Shape              Param #   
=================================================================
 conv2d (Conv2D)             (None, 28, 28, 32)        160       
                                                                 
 max_pooling2d (MaxPooling2D  (None, 14, 14, 32)       0         
 )                                                               
                                                                 
 conv2d_1 (Conv2D)           (None, 14, 14, 32)        16416     
                                                                 
 max_pooling2d_1 (MaxPooling  (None, 7, 7, 32)         0         
 2D)                                                             
                                                                 
 flatten (Flatten)           (None, 1568)              0         
                                                                 
 dense (Dense)               (None, 10)                15690     
                                                                 
=================================================================
Total params: 32,266
Trainable params: 32,266
Non-trainable params: 0
_________________________________________________________________
Test loss: 0.096678227186203
Test accuracy: 0.9707000255584717
WARNING:tensorflow:Detecting that an object or model or tf.train.Checkpoint is being deleted with unrestored values. See the following logs for the specific values in question. To silence these warnings, use `status.expect_partial()`. See https://www.tensorflow.org/api_docs/python/tf/train/Checkpoint#restorefor details about the status object returned by the restore function.
WARNING:tensorflow:Value in checkpoint could not be found in the restored object: (root).optimizer.iter
WARNING:tensorflow:Value in checkpoint could not be found in the restored object: (root).optimizer.decay
WARNING:tensorflow:Value in checkpoint could not be found in the restored object: (root).optimizer.learning_rate
WARNING:tensorflow:Value in checkpoint could not be found in the restored object: (root).optimizer.momentum
```


## 解释输出

### `tuner.search_space_summary()` 的输出及解释

```plaintext
Search space summary
Default search space size: 9
activation (Choice)
{'default': 'relu', 'conditions': [], 'values': ['relu', 'tanh'], 'ordered': False}
dropout (Float)
{'default': 0.1, 'conditions': [], 'min_value': 0.1, 'max_value': 0.5, 'step': 0.2, 'sampling': 'linear'}
learning_rate (Float)
{'default': 0.0001, 'conditions': [], 'min_value': 0.0001, 'max_value': 0.001, 'step': None, 'sampling': 'log'}
optimizer (Choice)
{'default': 'SGD', 'conditions': [], 'values': ['SGD', 'Adam'], 'ordered': False}
num_layers (Int)
{'default': None, 'conditions': [], 'min_value': 1, 'max_value': 2, 'step': 1, 'sampling': 'linear'}
filters_0 (Int)
{'default': None, 'conditions': [], 'min_value': 32, 'max_value': 64, 'step': 32, 'sampling': 'linear'}
kernel_size_0 (Int)
{'default': None, 'conditions': [], 'min_value': 2, 'max_value': 4, 'step': 2, 'sampling': 'linear'}
pool_size_0 (Int)
{'default': None, 'conditions': [], 'min_value': 2, 'max_value': 2, 'step': 1, 'sampling': 'linear'}
dropout_layer (Boolean)
{'default': False, 'conditions': []}
```

**解释：**
- **Search space summary** 显示了超参数调优的范围和默认值。
- **activation**: 可以选择的激活函数有 `relu` 和 `tanh`，默认是 `relu`。
- **dropout**: 丢弃率（dropout rate）的范围是从 0.1 到 0.5，步长为 0.2，默认是 0.1。
- **learning_rate**: 学习率的范围是从 0.0001 到 0.001，采用对数采样，默认是 0.0001。
- **optimizer**: 可以选择的优化器有 `SGD` 和 `Adam`，默认是 `SGD`。
- **num_layers**: 网络层数的范围是从 1 到 2，默认是 None。
- **filters_0**: 第一层卷积层的滤波器数量范围是从 32 到 64，步长为 32，默认是 None。
- **kernel_size_0**: 第一层卷积层的核大小范围是从 2 到 4，步长为 2，默认是 None。
- **pool_size_0**: 第一层池化层的池化大小范围是从 2 到 2，步长为 1，默认是 None。
- **dropout_layer**: 是否添加丢弃层，默认是 False。

### `tuner.results_summary()` 的输出及解释

```plaintext
Results summary
Results in ai/mnist
Showing 10 best trials
Objective(name="val_accuracy", direction="max")

Trial 09 summary
Hyperparameters:
activation: relu
dropout: 0.1
learning_rate: 0.0001
optimizer: SGD
num_layers: 2
filters_0: 32
kernel_size_0: 2
pool_size_0: 2
dropout_layer: False
filters_1: 32
kernel_size_1: 4
pool_size_1: 2
Score: 0.9729166626930237

... (其他试验的摘要)
```

**解释：**
- **Results summary** 显示了在 `ai/mnist` 目录下的结果，展示了前 10 个最佳试验。
- **Objective**: 优化的目标是验证集的准确率（`val_accuracy`），方向是最大化。
- **Trial 09 summary**: 显示了第 9 次试验的超参数配置及其得分（`Score`），得分越高表示模型性能越好。
- **Hyperparameters**: 列出了每个试验的具体超参数配置。

### `best_model.summary()` 的输出及解释

```plaintext
Model: "sequential"
_________________________________________________________________
 Layer (type)                Output Shape              Param #   
=================================================================
 conv2d (Conv2D)             (None, 28, 28, 32)        160       
                                                                 
 max_pooling2d (MaxPooling2D  (None, 14, 14, 32)       0         
 )                                                               
                                                                 
 conv2d_1 (Conv2D)           (None, 14, 14, 32)        16416     
                                                                 
 max_pooling2d_1 (MaxPooling  (None, 7, 7, 32)         0         
 2D)                                                             
                                                                 
 flatten (Flatten)           (None, 1568)              0         
                                                                 
 dense (Dense)               (None, 10)                15690     
                                                                 
=================================================================
Total params: 32,266
Trainable params: 32,266
Non-trainable params: 0
_________________________________________________________________
Test loss: 0.096678227186203
Test accuracy: 0.9707000255584717
```

**解释：**
- **Model summary**: 显示了最佳模型的结构摘要。
- **Layer**: 列出了模型的每一层及其类型、输出形状和参数数量。
- **Total params**: 模型的总参数数量。
- **Trainable params**: 可训练的参数数量。
- **Non-trainable params**: 不可训练的参数数量。
- **Test loss**: 测试集上的损失值。
- **Test accuracy**: 测试集上的准确率。

### 警告信息解释

```plaintext
WARNING:tensorflow:Detecting that an object or model or tf.train.Checkpoint is being deleted with unrestored values. See the following logs for the specific values in question. To silence these warnings, use `status.expect_partial()`. See https://www.tensorflow.org/api_docs/python/tf/train/Checkpoint#restorefor details about the status object returned by the restore function.
WARNING:tensorflow:Value in checkpoint could not be found in the restored object: (root).optimizer.iter
WARNING:tensorflow:Value in checkpoint could not be found in the restored object: (root).optimizer.decay
WARNING:tensorflow:Value in checkpoint could not be found in the restored object: (root).optimizer.learning_rate
WARNING:tensorflow:Value in checkpoint could not be found in the restored object: (root).optimizer.momentum
```

**解释：**
- 这些警告信息表明在恢复模型时，某些值（如优化器的迭代次数、衰减率、学习率和动量）没有被正确恢复。
- 可以通过使用 `status.expect_partial()` 来忽略这些警告，或者检查恢复过程是否正确。

## 实验总结

通过本次实验，我们成功掌握了构建和调优卷积神经网络的方法，深入理解了超参数对模型性能的影响，并使用 Keras Tuner 进行系统化的超参数搜索，最终获得了在手写数字识别任务上准确率高达 97.07% 的最佳模型。实验过程中，我们通过分析 `tuner.search_space_summary()` 和 `tuner.results_summary()` 的输出，了解了超参数的搜索范围和最佳组合，并通过 `best_model.summary()` 确认了最佳模型的结构和性能。尽管在恢复模型时遇到了一些优化器参数未正确恢复的警告，但通过使用 `status.expect_partial()` 或检查恢复过程，我们能够顺利完成实验。本次实验不仅为我们提供了宝贵的经验和知识，也为今后在深度学习领域的进一步研究和应用打下了坚实的基础。
