from tensorflow import keras
import tensorflow as tf


input_length = 64
batchSize = 64
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