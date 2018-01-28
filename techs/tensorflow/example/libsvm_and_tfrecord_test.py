#!/usr/bin/env python
# _*_coding:utf-8_*_

from libsvm_and_tfrecord import LibsvmAndTFrecord
import tensorflow as tf


input_path  = "../data/libsvm_tfrecord_data/libsvm_data.txt"
output_path = "../data/libsvm_tfrecord_data/libsvm_to_tfrecord_data.txt"

# tensorflow config 
flags = tf.app.flags
FLAGS = flags.FLAGS 

# config parameter 
flags.DEFINE_string('input_path', input_path, "input data path (libsvm)")
flags.DEFINE_string('output_path', output_path, "output data path (tfrecord)")

# read instances from tfrecord file
max_epoch = 1
batch_size = 4
num_thread = 1
min_after_dequeue = 1
mode = 'train'

def test_lisvm_and_tfrecord():
  data = LibsvmAndTFrecord()
  data.libsvm_to_tfrecord(FLAGS.input_path, FLAGS.output_path)
  label, index, value = data.tfrecord_batch(output_path, max_epoch, batch_size, num_thread, min_after_dequeue, mode)
  print('label info: {}'.format(label))
  return label, index, value


def main(_):
  label, index, value = test_lisvm_and_tfrecord()
  #dense_index = tf.sparse_to_dense(index, tf.stack([batch_size, 30], 1, 0))
  with tf.Session() as sess:
    print('\nsess begin ..........')
    print('shape label: {}'.format(sess.run(tf.shape(label))))
    print('label: {}'.format(sess.run(label)))

  print("\nlabel:\n{}\n{}".format(label, tf.shape(label)))
  print("\n\nindex:\n{}\n{}\t{}".format(index, tf.shape(index), tf.size(index)))
  print("\n\nvalue:\n{}".format(value))

if __name__ == '__main__':
  tf.app.run()

print("done!")
