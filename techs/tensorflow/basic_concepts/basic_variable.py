#!/usr/bin/env python
#_*_ coding:utf-8 _*_

import tensorflow as tf 
import numpy as np

# ============= tf.Variable =============
tensor1 = tf.constant(np.arange(1, 13), shape=[2,3,2], dtype=tf.float32)
a1 = tf.Variable(tensor1, dtype=tf.float32, name='a1')
a2 = tf.Variable(initial_value=tf.random_normal(shape=[2,3], mean=0, stddev=1), name='a2')
a3 = tf.Variable(tf.ones(shape=[2, 3]), name='a3')
a4 = a2.assign(tf.constant(np.arange(1,7), shape=[2,3], dtype=tf.float32))

with tf.Session() as sess:
  sess.run(tf.global_variables_initializer())
  print('var(a1) info:\n{}, type:{}, shape: {}, name: {}'.format(sess.run(a1), type(a1), sess.run(tf.shape(a1)), a1.name))
  print('var(a2) info:\n{}'.format(a2.eval(sess)))
  print('var(a3) info:\n{}'.format(sess.run(a3)))
  print('var(a4) info:\n{}'.format(sess.run(a4)))

# ============= tf.get_variable =============
gv1 = tf.get_variable('a1', shape=[2,3,2])

with tf.Session() as sess:
  sess.run(tf.global_variables_initializer())
  print('gv(gv1) info:\n{}, gv1.name: {}'.format(sess.run(gv1), gv1.name))

# ============= tf.name_scope =============
print("\n[INFO] tf.name_scope ....")
with tf.name_scope("name_scope_hello") as name_scope:
  ns1 = tf.get_variable('ns1', shape=[2, 10], dtype=tf.float32)
  print name_scope 
  print type(name_scope)
  print ns1.name
  var2 = tf.Variable(5, name='var2')
  print var2.name
  print("scope_name: %s" % tf.get_variable_scope().original_name_scope) 

# ============= tf.variable_scope =============
print("\ntest tf.variable_scope\n")
with tf.variable_scope('variable_scope_hello') as variable_scope:
  vs1 = tf.get_variable('ns1', shape=[2,10], dtype=tf.float32)
  print variable_scope 
  print type(variable_scope)
  print variable_scope.name 
  print tf.get_variable_scope().original_name_scope 
  print vs1.name
  vs2 = tf.Variable([1,2], dtype=tf.int32, name='vs2')
  print vs2.name
  with tf.variable_scope('xxxx') as x_scope:
    print tf.get_variable_scope().original_name_scope 

# ============= tf.name_scope && tf.variable_scope =============
print('\n[INFO] tf.name_scope && tf.variable_scope ....')
with tf.name_scope('name1') as name:
  with tf.variable_scope('var1') as var:
    nv1 = tf.get_variable('nv1', shape=[2])
    add = tf.add(nv1, [3])
    print tf.get_variable_scope().original_name_scope

print nv1.name
print add.name 


with tf.Session() as sess:
  sess.run(tf.global_variables_initializer())
  print sess.run(nv1)
  print sess.run(add) 

# ============= what can tf.name_scope to do? =============
print('\nwhat can tf.name_scope to do? ....')
with tf.name_scope('hiddle') as scope:
  a = tf.constant(10, name='alpha')
  W = tf.Variable(tf.random_normal([1,2], -1.0, 1.0), dtype=tf.float32, name='weights')
  b = tf.Variable(tf.zeros([1]), name='bias')
  print a.name
  print W.name
  print b.name 

# try tf.name_scope(None) 
with tf.name_scope('clear'):
  w1 = tf.Variable(1.0, dtype=tf.float32, name='w1')
  with tf.name_scope(None):
    w2 = tf.Variable(2.0, name='w2')

print w1.name
print w2.name

# ============= what can tf.variable_scope to do? =============
print('\nwhat can tf.variable_scope to do? ....')
def scope1():
  with tf.variable_scope('scope1'):
    var1 = tf.get_variable('var1', shape=[2], dtype=tf.float32)
    print var1.name 
    def scope2():
      with tf.variable_scope('scope2'):
        var2 = tf.get_variable('var2', shape=[2], dtype=tf.float32)
      return var2 
  return scope2() 
res = scope1()
print res.name 

# --------
with tf.variable_scope('scope1'):
  w = tf.get_variable('w', shape=[1])     
  print w.name        # name: 'scope/w'
  with tf.variable_scope('scope2'):
    w = tf.get_variable('w', shape=[1])   
    print w.name      # name: 'scope/scope/w'

with tf.variable_scope('scope1', reuse=True):
  w1 = tf.get_variable('w', shape=[1])   
  print w1.name       # name: 'scope/w1'
  scope = tf.get_variable_scope()
  print scope 
  print scope.reuse_variables()
  with tf.variable_scope('scope2'):
    w1 = tf.get_variable('w', shape=[1]) 
    print w1.name     # name: 'scope/w1' 
