#!/usr/bin/env python
# coding=utf-8

import tensorflow as tf 
print("tf version: {0}".format(tf.__version__))
# ========= session use method 1 ===========
# create a session
sess = tf.Session()
# use a session to run task
hello = tf.constant("[method1] Hello TensorFlow")
#print(sess.run(hello))
print(hello.eval(session = sess))
# close session 
sess.close()

# ========= session use method 2 ===========
hello = tf.constant("Hello TensorFlow")
a = tf.constant(2)
b = tf.constant(3)
m = tf.multiply(a, b)
with tf.Session() as sess:
  result = sess.run(hello)
  print(result)
  print(sess.run(m))

# ========= session use method 3 ===========
sess = tf.Session()
with sess.as_default():
  # hello as tensor
  print(hello.eval())

# ========= session use method 4 ===========
conf = tf.ConfigProto(allow_soft_placement = True,
                       log_device_placement = True)
sess = tf.Session(config = conf)
print(hello.eval(session = sess))
print(m.eval(session = sess))
