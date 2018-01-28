#!/usr/bin/env python
# coding=utf-8

import tensorflow as tf
import numpy as np

# ============ create op ===============
# creat a constant op. shape: 1*2
#matrix1 = tf.constant([[3., 3.]])
matrix1 = tf.constant([3,3], shape=[1,2], dtype=tf.float32)
# create a constant op. shape: 2*1
#matrix2 = tf.constant([[2.], [5.]])
#matrix2 = tf.constant([2,5], shape=[2,1], dtype=tf.float32)
matrix2 = tf.constant(np.array([2,5]), shape=[2,1], dtype=tf.float32)
# create a matrix multiple op 
product = tf.matmul(matrix1, matrix2)

# ============ create session ==========
with tf.Session() as sess:
  print("\nMatrixInfo type: {0}, shape: {1}, size: {2}".format(type(matrix1), tf.shape(matrix1), tf.size(matrix1)))
  result = sess.run(product)
  print(result)
