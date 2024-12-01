import tensorflow as tf
import keras_nlp


dataPath = ""
dataFile = dataPath + "sentiment.txt"
labelFile = dataPath + "sentiment_label.txt"
tfFile = dataPath + "sentiment.tfrecord"
output_length = 128
batchSize = 64
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