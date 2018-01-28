#!/usr/bin/env python
# coding=utf-8

import tensorflow as tf

# ============ graph: use method 1 ===============
c = tf.constant(value=1)
assert c.graph is tf.get_default_graph()
print(c.graph)
print(tf.get_default_graph())

# ============ graph: use method 2 ===============
with tf.Graph().as_default() as g:
  with g.name_scope("myscope") as scope:
    sess = tf.Session(target = '', graph = g, config = None)
    print("graph version: ", g.version)
    a = tf.placeholder("float")
    print("a.op: ", a.op)
    print("graph version: ", g.version)
    b = tf.placeholder("float")
    print("b.op: ", b.op)
    print("graph version: ", g.version)
    y1 = tf.multiply(a, b)
    print("y1.op: ", y1.op)
    print("graph version: ", g.version)
    operations = g.get_operations()
    for (i, op) in enumerate(operations):
      print("========= operation", i + 1, "===========")
      print("for op: ", op)
    assert y1.graph is g
    assert sess.graph is g
    print("sess.graph: ", sess.graph)
    print("sess.graph_def: ", sess.graph_def)
    print("sess.sess_str: ", sess.sess_str)
    print (sess.run(y1, feed_dict={a: 2.0, b: 3.0}))   # 5
