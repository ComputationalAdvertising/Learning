#!/usr/bin/env python
# __*coding:utf-8_*_

import tensorflow as tf
import numpy as np

batch_size = 5
num_class = 10
label_row = tf.constant([0,2,4,6,8])

label = tf.expand_dims(label_row, 1)
index = tf.expand_dims(tf.range(0, batch_size), 1)

# use a matrix 
concated = tf.concat([index, label],1)
onehot_labels = tf.sparse_to_dense(concated, tf.stack([batch_size, num_class]), 1.0, 0.0)

with tf.Session() as sess:
  print('label_row')
  print sess.run(label_row)
  print sess.run(tf.shape(label_row))
  print('label')
  print sess.run(label)
  print sess.run(tf.shape(label))
  print('index')
  print sess.run(index)
  print sess.run(tf.shape(index))
  print('concated')
  print sess.run(concated)
  print sess.run(tf.shape(concated))
  print('onehot_labels')
  print sess.run(onehot_labels)
  print sess.run(tf.shape(onehot_labels))
