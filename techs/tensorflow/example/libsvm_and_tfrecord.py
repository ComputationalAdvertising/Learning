#!/usr/bin/env python
# _*_coding:utf-8_*_

import sys 
import tensorflow as tf 

class LibsvmAndTFrecord:
  def __init__(self):
    print('LibsvmAndTFrecord.__init__ libsvm and tfrecord')
    self.features = {}
    self.features['label'] = tf.FixedLenFeature([], tf.float32)   # label
    self.features['index'] = tf.VarLenFeature(tf.int64)           # id
    self.features['value'] = tf.VarLenFeature(tf.float32)         # value

  '''
  parse libsvm data at each line
  '''
  def libsvm_to_tfrecord(self, input_path, output_path):
    writer = tf.python_io.TFRecordWriter(output_path)
    
    for line in open(input_path, 'r'):
      example = self.convert_line(line)
      assert example is not None
      writer.write(example.SerializeToString())
    
    writer.close()
    print('convert successfully!')
    tf.logging.info("convert successfully!")
  
  '''
  brief parse line data libsvm -> tfrecord
  '''
  def convert_line(self, line):
    line_items = line.strip().split(' ')
    if len(line_items) < 2:
      print >> sys.stderr, 'line error. {}'.format(line)
      return None 
    label = float(line_items[0])
    feature_indexs = []
    feature_values = []
    for item in line_items[1:]:
      id, value = item.strip().split(':')
      feature_indexs.append(int(id))
      feature_values.append(float(value))

    feature = {}
    feature['label'] = tf.train.Feature(float_list=tf.train.FloatList(value=[label]))
    feature['index'] = tf.train.Feature(int64_list = tf.train.Int64List(value = feature_indexs))
    feature['value'] = tf.train.Feature(float_list = tf.train.FloatList(value = feature_values))
    
    example = tf.train.Example(features = tf.train.Features(feature = feature))
    return example

  '''
  brief get tfrecord to modeling 
  '''
  def tfrecord_batch(self, file_name, max_epoch, batch_size, num_thread, min_after_dequeue, mode):
    with tf.name_scope('input'):
      if not isinstance(file_name, list):
        file_name = file_name.strip().split(',')
      if mode == 'predict':
        max_epoch = 1
      seed = 1
      filename_queue = tf.train.string_input_producer(file_name, num_epochs=max_epoch, seed=seed)
      # decode 
      reader = tf.TFRecordReader()
      _, serialized_example = reader.read_up_to(filename_queue, 1000)
      capacity = num_thread * batch_size + min_after_dequeue 

      if mode == 'predict':
        batch_serialized_example = tf.train.batch(
          [serialized_example], batch_size = batch_size, num_threads = num_thread, 
          capacity = capacity, enqueue_many = True, allow_smaller_final_batch = True)
      else:
        batch_serialized_example = tf.train.shuffle_batch(
          [serialized_example], batch_size = batch_size, num_threads = num_thread, 
          capacity = capacity, enqueue_many = True, min_after_dequeue = min_after_dequeue, seed = seed)
      
      # examples --> number of tensors
      instances = tf.parse_example(batch_serialized_example, self.features)
      return instances['label'], instances['index'], instances['value']
