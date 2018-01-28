#!/usr/bin/env python
# _*_coding:utf-8_*_

import sys 
import tensorflow as tf 

class LibfmAndTFrecord:
  '''
  brief: converts libfm-format data to tfrecord-format data
  param sparse_fields [string type] '1,2,3'
  param linear fields [string type] '4,5,6'
  param output_path output path of tfrecord
  '''
  #def __init__(self, sparse_fields, continuous_fields):
  def __init__(self, sparse_fields):
    self.sparse_fields = sparse_fields.strip().split(',')
    #self.continuous_fields = sparse_fields.strip().split(',')
    
    # map<int, map<string, string>> map[field]["id"] = "sparse_field_id_" + field 
    self.field2name = {}
    
    self.features = {}
    self.features['label'] = tf.FixedLenFeature([], tf.float32)
    # sparse fields
    for field in self.sparse_fields:
      if field not in self.field2name:
        type2name = {'id':'sparse_field_id_' + field, 'val':'sparse_field_val_' + field}
        self.field2name[field] = type2name
      self.features[self.field2name[field]['id']] = tf.VarLenFeature(tf.int64)    # id
      self.features[self.field2name[field]['val']] = tf.VarLenFeature(tf.float32) # val 
    # other fields ....
    '''
    for field in self.continuous_fields:
      if field not in self.field2name:
        type2name = {'id':'continuous_field_id_' + str(field), 'val':'continuous_field_val_' + str(field)}
        self.field2nameame[field] = type2name 
      self.features[self.field2name[field]['id']] = tf.VarLenFeature(tf.int64)    # id 
      self.features[self.field2name[field]['val']] = tf.VarLenFeature(tf.float32) # val 
    '''
  
############################# libfm --> tfrecords ##################################
  '''
  parse libfm data at each line 
  '''
  def libfm_to_tfrecord(self, input_path, output_path):
    writer = tf.python_io.TFRecordWriter(output_path)
    
    for line in open(input_path, 'r'):
      example = self.convert_line(line)
      assert example is not None
      writer.write(example.SerializeToString())
    
    writer.close()
    tf.logging.info("convert successfully!")

  '''
  brief parse line data libfm -> tfrecord
  input line: 
    0 1:10:1.2 3:20:1.4 9:17:0.8 ...
  output example
  '''
  def convert_line(self, line):
    line_items = line.strip().split(' ')
    if len(line_items) < 2:
      print >> sys.stderr, "line error. {0}".format(line)
      return None
    label = float(line_items[0])
    field2feature = {}    # two-loop map. map<int, map<int, float>>
    # parse item. '1:10:1.2'
    for item in line_items[1:]:
      fieldid, featureid, value = item.split(':')
      fieldid = int(fieldid)
      featureid = int(featureid)
      value = float(value)
      if int(fieldid) not in field2feature:
        feature2field = {}
        feature2field[featureid] = value
        field2feature[fieldid] = feature2field
      else:
        field2feature[fieldid][featureid] = value

    #### define feature of line and fill info ####
    feature = {}
    feature['label'] = tf.train.Feature(float_list=tf.train.FloatList(value=[label]))
    # sparse fields 
    self.save_field_info(field2feature, self.sparse_fields, feature)
    # continuous fields
    # picture fields 
    # text fields
    
    example = tf.train.Example(features = tf.train.Features(feature = feature))
    return example 

  '''
  brief save field info 
  '''
  def save_field_info(self, field2feature, fields, feature):
    for fieldid in fields:
      feature_id_list = []
      feature_val_list = []
      if fieldid in field2feature:
        for featureid,value in field2feature[fieldid].items():
          feature_id_list.append(featureid)
          feature_val_list.append(value)
      else:
        feature_id_list.append(0)
        feature_val_list.append(0.0)
          
      feature[self.field2name[fieldid]['id']] = tf.train.Feature(int64_list=tf.train.Int64List(value = feature_id_list))
      feature[self.field2name[fieldid]['val']] = tf.train.Feature(float_list = tf.train.FloatList(value = feature_val_list))

############################# tfrecords --> modeling ##################################

  '''
  brief get tfrecord to modeling
  '''
  def tfrecord_batch(self, file_name, max_epoch, batch_size, num_thread, min_after_dequeue, mode):
    with tf.name_scope('input'):
      if not isinstance(file_name, list):
        file_name = file_name.strip().split(',')
      
      if mode == 'predict':
        max_epoch = 1

      seed = 1;
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
      
      # brief field related data
      def get_field_data(instances, fields):
        ids = []
        vals = []
        for fieldid in fields:
          ids.append(instances[self.field2name[fieldid]['id']])
          vals.append(instances[self.field2name[fieldid]['val']])
        return ids, vals 

      # sparse field info got from instances
      sparse_ids, sparse_vals = get_field_data(instances, self.sparse_fields)
      # continuous field info got from instances
      #continuous_ids, continuous_vals = self.get_field_data(instances, self.continuous_fields)

      return instances['label'], sparse_ids, sparse_vals 


