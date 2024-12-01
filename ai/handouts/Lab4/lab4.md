# 人工智能试验报告-Lab4

---
学号: 2022211100 班级:2220701 姓名:郭明轩

---

## 实验目的

- 理解字嵌入方法
- 掌握利用Bert进行字嵌入，将文本转换为数据集的方法
- 理解循环神经网络
- 掌握利用早停提升模型泛化能力的方法

## 实验环境

```sh
(venv1) root@54cf208e6479:~/py1# pip list
Package                      Version
---------------------------- -----------
absl-py                      2.1.0
astunparse                   1.6.3
certifi                      2024.8.30
charset-normalizer           3.4.0
contourpy                    1.3.0
cycler                       0.12.1
flatbuffers                  24.3.25
fonttools                    4.54.1
gast                         0.6.0
google-pasta                 0.2.0
grpcio                       1.67.1
h5py                         3.12.1
idna                         3.10
importlib_metadata           8.5.0
importlib_resources          6.4.5
kagglehub                    0.3.3
keras                        3.6.0
keras-hub                    0.17.0
keras-nlp                    0.17.0
kiwisolver                   1.4.7
libclang                     18.1.1
Markdown                     3.7
markdown-it-py               3.0.0
MarkupSafe                   3.0.2
matplotlib                   3.9.2
mdurl                        0.1.2
ml-dtypes                    0.4.1
namex                        0.0.8
numpy                        2.0.2
opt_einsum                   3.4.0
optree                       0.13.0
packaging                    24.1
pillow                       11.0.0
pip                          23.0.1
protobuf                     5.28.3
Pygments                     2.18.0
pyparsing                    3.2.0
python-dateutil              2.9.0.post0
regex                        2024.9.11
requests                     2.32.3
rich                         13.9.3
setuptools                   58.1.0
six                          1.16.0
tensorboard                  2.18.0
tensorboard-data-server      0.7.2
tensorflow                   2.18.0
tensorflow-io-gcs-filesystem 0.37.1
tensorflow-text              2.18.0
termcolor                    2.5.0
tqdm                         4.66.6
typing_extensions            4.12.2
urllib3                      2.2.3
Werkzeug                     3.1.0
wheel                        0.44.0
wrapt                        1.16.0
zipp                         3.20.2
```

## 调整

### textToTFRecode

output_length 增加,内存消耗更大,速度更慢

## 关键代码

### textToTFRecode.py

```py
import tensorflow as tf
import keras_nlp


dataPath = ""
dataFile = dataPath + "sentiment.txt"
labelFile = dataPath + "sentiment_label.txt"
tfFile = dataPath + "sentiment.tfrecord"
output_length = 32
batchSize = 128
presetModel="bert_base_zh"


def encodeExample(x, y):
    dataFeatures = {
        'x': tf.train.Feature(float_list=tf.train.FloatList(value=x.flatten())),
        'y': tf.train.Feature(int64_list=tf.train.Int64List(value=[y]))
    }
    example = tf.train.Example(features=tf.train.Features(feature=dataFeatures)).SerializeToString()
    return example


if __name__ == '__main__':
    preprocessor = keras_nlp.models.BertPreprocessor.from_preset(presetModel, sequence_length=output_length)
    bert_backbone = keras_nlp.models.BertBackbone.from_preset(presetModel)

    x = tf.data.TextLineDataset(dataFile).batch(batchSize)
    x = x.map(preprocessor, num_parallel_calls=tf.data.AUTOTUNE)

    y = tf.data.TextLineDataset(labelFile)
    y = y.map(lambda e: tf.strings.to_number(e, tf.int64)).batch(batchSize)
    dataset = tf.data.Dataset.zip((x, y))

    print("Transforming ", end="")
    with tf.io.TFRecordWriter(tfFile) as tfWriter:
        for tokens, labels in dataset:
            embeddings = bert_backbone(tokens)["sequence_output"].numpy()
            ls = labels.numpy()
            for i in range(len(ls)):
                example = encodeExample(embeddings[i,:,:], ls[i])
                tfWriter.write(example)

            print("=", end="")
    tfWriter.close()
```

### textClassfication.py

因为我修改了textToTFRecode.py的参数，导致也要修改textClassfication中的参数

```py
from tensorflow import keras
import tensorflow as tf


input_length = 32
batchSize = 128
lstmSize = 64
dropoutRate = 0.3
learningRate = 0.0001
epoches = 100
dataPath = ""
tfFile=dataPath + "sentiment.tfrecord"


def decodeExample(example):#解码
    feature_description = {
        'x': tf.io.FixedLenFeature([768 * input_length], tf.float32),
        'y': tf.io.FixedLenFeature([], tf.int64, default_value=0)
    }
    parsed = tf.io.parse_single_example(example, features=feature_description)
    x = tf.reshape(parsed['x'], (input_length, 768))
    y = parsed['y']
    return x, y

# raw_dataset = tf.data.TFRecordDataset(tfFile)
# for raw_record in raw_dataset.take(1):
#     example = tf.train.Example()
#     example.ParseFromString(raw_record.numpy())
#     print(example)

if __name__ == '__main__':
    sentiment_dataset = tf.data.TFRecordDataset(tfFile).map(decodeExample)
    train_dataset = sentiment_dataset.take(8000).batch(batchSize)
    val_dataset = sentiment_dataset.skip(8000).take(2000).batch(batchSize)
    test_dataset = sentiment_dataset.skip(10000).take(2000).batch(batchSize)

    model=keras.Sequential(
        [keras.Input(shape=(input_length, 768)),
         keras.layers.Masking(),
         keras.layers.BatchNormalization(),
         keras.layers.Dropout(dropoutRate),
         keras.layers.Bidirectional(
             keras.layers.LSTM(lstmSize, kernel_regularizer=keras.regularizers.l2(),
                               return_sequences=True)),
         keras.layers.LSTM(lstmSize, kernel_regularizer=keras.regularizers.l2()),
         keras.layers.Dropout(dropoutRate),
         keras.layers.Dense(2, activation="softmax", kernel_regularizer = keras.regularizers.l2())
        ]
    )

    model.compile(
        optimizer=keras.optimizers.Adamax(learning_rate=learningRate),
        loss=keras.losses.SparseCategoricalCrossentropy(),
        metrics=[keras.metrics.SparseCategoricalAccuracy()]
    )
    early_stopping_callback = keras.callbacks.EarlyStopping(
        monitor='val_sparse_categorical_accuracy', mode='max',
        patience=5, restore_best_weights=True)
    history = model.fit(train_dataset, epochs=epoches, validation_data=val_dataset,
                          callbacks=[early_stopping_callback], verbose=2)
    result = model.evaluate(test_dataset, verbose=0)
    print(result)

    #predictions = model.predict(test_dataset)
    #print(predictions)
```
## 实验结果

### 1

```sh
2024-11-01 10:50:26.662986: E external/local_xla/xla/stream_executor/cuda/cuda_driver.cc:152] failed call to cuInit: INTERNAL: CUDA error: Failed call to cuInit: UNKNOWN ERROR (303)
Transforming 2024-11-01 10:50:29.061816: E tensorflow/core/util/util.cc:131] oneDNN supports DT_INT64 only on platforms with AVX-512. Falling back to the default Eigen-based implementation if present.
============================================================================================================================================================================================2024-11-01 11:19:21.388634: I tensorflow/core/framework/local_rendezvous.cc:405] Local rendezvous is aborting with status: OUT_OF_RANGE: End of sequence
```

```sh
Transforming 2024-11-01 12:01:28.588694: E tensorflow/core/util/util.cc:131] oneDNN supports DT_INT64 only on platforms with AVX-512. Falling back to the default Eigen-based implementation if present.
==============================================================================================2024-11-01 12:10:51.446046: I tensorflow/core/framework/local_rendezvous.cc:405] Local rendezvous is aborting with status: OUT_OF_RANGE: End of sequence
```

### 2

```sh
Epoch 1/100
63/63 - 13s - 199ms/step - loss: 9.4942 - sparse_categorical_accuracy: 0.5935 - val_loss: 8.8959 - val_sparse_categorical_accuracy: 0.7040
Epoch 2/100
63/63 - 10s - 161ms/step - loss: 8.3946 - sparse_categorical_accuracy: 0.7181 - val_loss: 7.8772 - val_sparse_categorical_accuracy: 0.7675
Epoch 3/100
63/63 - 9s - 146ms/step - loss: 7.4499 - sparse_categorical_accuracy: 0.7625 - val_loss: 6.9895 - val_sparse_categorical_accuracy: 0.7880
Epoch 4/100
63/63 - 9s - 146ms/step - loss: 6.6269 - sparse_categorical_accuracy: 0.7954 - val_loss: 6.2352 - val_sparse_categorical_accuracy: 0.7980
Epoch 5/100
63/63 - 9s - 148ms/step - loss: 5.9268 - sparse_categorical_accuracy: 0.8094 - val_loss: 5.5994 - val_sparse_categorical_accuracy: 0.8105
Epoch 6/100
63/63 - 9s - 143ms/step - loss: 5.3298 - sparse_categorical_accuracy: 0.8257 - val_loss: 5.0604 - val_sparse_categorical_accuracy: 0.8210
Epoch 7/100
63/63 - 10s - 155ms/step - loss: 4.8260 - sparse_categorical_accuracy: 0.8326 - val_loss: 4.5996 - val_sparse_categorical_accuracy: 0.8305
Epoch 8/100
63/63 - 13s - 198ms/step - loss: 4.3884 - sparse_categorical_accuracy: 0.8466 - val_loss: 4.2016 - val_sparse_categorical_accuracy: 0.8335
Epoch 9/100
63/63 - 12s - 186ms/step - loss: 4.0146 - sparse_categorical_accuracy: 0.8521 - val_loss: 3.8576 - val_sparse_categorical_accuracy: 0.8410
Epoch 10/100
63/63 - 11s - 169ms/step - loss: 3.6879 - sparse_categorical_accuracy: 0.8587 - val_loss: 3.5579 - val_sparse_categorical_accuracy: 0.8410
Epoch 11/100
63/63 - 11s - 177ms/step - loss: 3.4034 - sparse_categorical_accuracy: 0.8634 - val_loss: 3.2963 - val_sparse_categorical_accuracy: 0.8455
Epoch 12/100
63/63 - 7s - 117ms/step - loss: 3.1512 - sparse_categorical_accuracy: 0.8666 - val_loss: 3.0669 - val_sparse_categorical_accuracy: 0.8510
Epoch 13/100
63/63 - 7s - 116ms/step - loss: 2.9320 - sparse_categorical_accuracy: 0.8734 - val_loss: 2.8654 - val_sparse_categorical_accuracy: 0.8545
Epoch 14/100
63/63 - 8s - 124ms/step - loss: 2.7396 - sparse_categorical_accuracy: 0.8783 - val_loss: 2.6898 - val_sparse_categorical_accuracy: 0.8570
Epoch 15/100
63/63 - 10s - 155ms/step - loss: 2.5740 - sparse_categorical_accuracy: 0.8821 - val_loss: 2.5358 - val_sparse_categorical_accuracy: 0.8585
Epoch 16/100
63/63 - 10s - 161ms/step - loss: 2.4231 - sparse_categorical_accuracy: 0.8852 - val_loss: 2.4011 - val_sparse_categorical_accuracy: 0.8600
Epoch 17/100
63/63 - 10s - 154ms/step - loss: 2.2888 - sparse_categorical_accuracy: 0.8936 - val_loss: 2.2842 - val_sparse_categorical_accuracy: 0.8610
Epoch 18/100
63/63 - 9s - 141ms/step - loss: 2.1752 - sparse_categorical_accuracy: 0.8944 - val_loss: 2.1810 - val_sparse_categorical_accuracy: 0.8660
Epoch 19/100
63/63 - 9s - 141ms/step - loss: 2.0752 - sparse_categorical_accuracy: 0.8980 - val_loss: 2.0932 - val_sparse_categorical_accuracy: 0.8720
Epoch 20/100
63/63 - 9s - 144ms/step - loss: 1.9826 - sparse_categorical_accuracy: 0.9028 - val_loss: 2.0137 - val_sparse_categorical_accuracy: 0.8705
Epoch 21/100
63/63 - 9s - 150ms/step - loss: 1.9055 - sparse_categorical_accuracy: 0.9047 - val_loss: 1.9433 - val_sparse_categorical_accuracy: 0.8760
Epoch 22/100
63/63 - 12s - 184ms/step - loss: 1.8325 - sparse_categorical_accuracy: 0.9129 - val_loss: 1.8828 - val_sparse_categorical_accuracy: 0.8745
Epoch 23/100
63/63 - 8s - 124ms/step - loss: 1.7673 - sparse_categorical_accuracy: 0.9141 - val_loss: 1.8281 - val_sparse_categorical_accuracy: 0.8735
Epoch 24/100
63/63 - 7s - 117ms/step - loss: 1.7070 - sparse_categorical_accuracy: 0.9190 - val_loss: 1.7792 - val_sparse_categorical_accuracy: 0.8785
Epoch 25/100
63/63 - 7s - 116ms/step - loss: 1.6599 - sparse_categorical_accuracy: 0.9214 - val_loss: 1.7350 - val_sparse_categorical_accuracy: 0.8770
Epoch 26/100
63/63 - 7s - 118ms/step - loss: 1.6068 - sparse_categorical_accuracy: 0.9269 - val_loss: 1.6963 - val_sparse_categorical_accuracy: 0.8770
Epoch 27/100
63/63 - 11s - 167ms/step - loss: 1.5654 - sparse_categorical_accuracy: 0.9283 - val_loss: 1.6630 - val_sparse_categorical_accuracy: 0.8770
Epoch 28/100
63/63 - 10s - 162ms/step - loss: 1.5204 - sparse_categorical_accuracy: 0.9334 - val_loss: 1.6339 - val_sparse_categorical_accuracy: 0.8800
Epoch 29/100
63/63 - 11s - 177ms/step - loss: 1.4877 - sparse_categorical_accuracy: 0.9327 - val_loss: 1.6047 - val_sparse_categorical_accuracy: 0.8815
Epoch 30/100
63/63 - 9s - 149ms/step - loss: 1.4460 - sparse_categorical_accuracy: 0.9411 - val_loss: 1.5821 - val_sparse_categorical_accuracy: 0.8780
Epoch 31/100
63/63 - 9s - 140ms/step - loss: 1.4148 - sparse_categorical_accuracy: 0.9402 - val_loss: 1.5618 - val_sparse_categorical_accuracy: 0.8795
Epoch 32/100
63/63 - 9s - 139ms/step - loss: 1.3921 - sparse_categorical_accuracy: 0.9430 - val_loss: 1.5333 - val_sparse_categorical_accuracy: 0.8855
Epoch 33/100
63/63 - 9s - 141ms/step - loss: 1.3594 - sparse_categorical_accuracy: 0.9476 - val_loss: 1.5164 - val_sparse_categorical_accuracy: 0.8865
Epoch 34/100
63/63 - 9s - 139ms/step - loss: 1.3406 - sparse_categorical_accuracy: 0.9477 - val_loss: 1.5001 - val_sparse_categorical_accuracy: 0.8825
Epoch 35/100
63/63 - 9s - 140ms/step - loss: 1.3079 - sparse_categorical_accuracy: 0.9524 - val_loss: 1.4820 - val_sparse_categorical_accuracy: 0.8835
Epoch 36/100
63/63 - 9s - 138ms/step - loss: 1.2842 - sparse_categorical_accuracy: 0.9549 - val_loss: 1.4633 - val_sparse_categorical_accuracy: 0.8855
Epoch 37/100
63/63 - 9s - 139ms/step - loss: 1.2661 - sparse_categorical_accuracy: 0.9556 - val_loss: 1.4492 - val_sparse_categorical_accuracy: 0.8850
Epoch 38/100
63/63 - 9s - 137ms/step - loss: 1.2438 - sparse_categorical_accuracy: 0.9566 - val_loss: 1.4342 - val_sparse_categorical_accuracy: 0.8855
[1.5111953020095825, 0.8857574462890625]
```

## 实验总结

本次实验成功实现了基于Bert字嵌入和循环神经网络的文本分类任务。实验通过Bert预处理器和backbone将文本转换为嵌入向量，并保存为TFRecord格式。在模型设计上，采用了双向LSTM和普通LSTM的组合架构，同时使用了Dropout、BatchNormalization和L2正则化来防止过拟合。训练过程中应用了早停策略，在38个epoch后停止训练，最终模型在测试集上达到了88.58%的准确率。虽然增加output_length导致了更高的内存消耗和处理时间，但可能提供了更丰富的上下文信息。实验表明，通过合理的模型设计和训练策略，可以有效地完成文本分类任务。未来可以通过调整模型架构、优化器参数等方式进一步提升模型性能。总的来说，实验达成了预期目标，成功掌握了字嵌入方法、循环神经网络的应用以及使用早停策略提升模型泛化能力的技术。

