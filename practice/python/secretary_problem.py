#! /usr/bin/python
# vim:ft=python:foldmethod=marker

# http://en.wikipedia.org/wiki/Secretary_problem

import itertools
import math
import sys

def CanFindHighest(order, num_sample):
  max_value = len(order) - 1;
  max_sample = max(order[:num_sample])
  if max_sample == max_value:
    return False
  for v in order[num_sample:]:
    if v == max_value:
      return True
    elif v > max_sample:
      return False

def PickFlower(num_flowers, num_sample):
  count = 0;
  for order in itertools.permutations(range(num_flowers)):
    if CanFindHighest(order, num_sample):
      count += 1
  print "%d / %d" % (count, math.factorial(num_flowers))

def main(argv):
  N = 8
  for i in range(1, N):
    PickFlower(N, i)


if __name__ == "__main__":
  main(sys.argv)
