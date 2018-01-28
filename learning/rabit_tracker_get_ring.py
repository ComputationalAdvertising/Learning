#!/usr/bin/env python
# coding=utf-8

space_num = 0

def find_share_ring(tree_map, parent_map, r, space_num):
  nset = set(tree_map[r])
  cset = nset - set([parent_map[r]])
  print("{4}exp parent_map[{0}]: {1}, nset: {2}, cset: {3}".format(r, parent_map[r], nset, cset, " " * space_num))
  if len(cset) == 0:
    return [r]
  space_num += 2
  rlst = [r]
  cnt = 0
  for v in cset:
    vlst = find_share_ring(tree_map, parent_map, v, space_num)
    cnt += 1
    print("{5}v: {0}, cset: {4}, vlst: {1}, rlst: {2}, cnt: {3}".format(v, vlst, rlst, cnt, cset, " " * space_num))
    if cnt == len(cset):
      vlst.reverse()
    rlst += vlst
    print("{5}v: {0}, cset: {4}, vlst: {1}, rlst: {2}, cnt: {3}".format(v, vlst, rlst, cnt, cset, " " * space_num))
  return rlst

def get_ring(tree_map, parent_map):
  assert parent_map[0] == -1
  rlst = find_share_ring(tree_map, parent_map, 0, 0)
  print("rlst: {0}".format(rlst))
  assert len(rlst) == len(tree_map)
  ring_map = {}
  nslave = len(tree_map)
  for r in range(nslave):
    rprev = (r + nslave - 1) % nslave
    rnext = (r + 1) % nslave
    ring_map[rlst[r]] = (rlst[rprev], rlst[rnext])
  return ring_map

tree_map = {0: [1, 2], 1: [0, 3, 4], 2: [0, 5, 6], 3: [1, 7, 8], 4: [1, 9], 5: [2], 6: [2], 7: [3], 8: [3], 9: [4]}
parent_map = {0: -1, 1: 0, 2: 0, 3: 1, 4: 1, 5: 2, 6: 2, 7: 3, 8: 3, 9: 4}
ring_map = get_ring(tree_map, parent_map)
print(ring_map)
