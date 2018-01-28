#!/usr/bin/env python
# _*_coding:utf-8_*_

from libfm_and_tfrecord import LibfmAndTFrecord 

sparse_field="1,2,3"
data = LibfmAndTFrecord(sparse_field)

input_path='../example/data/libfm_data.txt'
output_path='../example/data/libfm_2tfrecord_data.txt'
data.libfm_to_tfrecord(input_path, output_path)

# read instances from tfrecord file
max_epoch = 1
batch_size = 4
num_thread = 1
min_after_dequeue = 1
mode = 'train'
label, sparse_id, sparse_val = data.tfrecord_batch(output_path, max_epoch, batch_size, num_thread, min_after_dequeue, mode)
print('type(label): {0}\n\ntype(sparse_id): {1}\n\ntype(sparse_val): {2}'.format(label, sparse_id, sparse_val))
