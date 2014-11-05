#ifndef __COMMON_H__
#define __COMMON_H__

#include <cstdio>

#define DIM 5
#define MAX_TRAINING_DATA_SIZE 800

struct Data {
  double x[DIM];
  bool y;
};

bool sign(double v) {
  return v > 0.0;
}

double product(const double a[], const double b[]) {
  double sum = a[0] * b[0];
  for (int i = 1; i < DIM; i++)
    sum += a[i] * b[i];
  return sum;
}

void add(const Data& data, double a[]) {
  if (data.y) {
    for (int i = 0; i < DIM; i++)
      a[i] += data.x[i];
  } else {
    for (int i = 0; i < DIM; i++)
      a[i] -= data.x[i];
  }
}

int input(Data data[]) {
  int data_size = 0;

  while (scanf("%lf", &data[data_size].x[1]) == 1) {
    data[data_size].x[0] = 1.0;
    for (int j = 2; j < DIM; j++)
      scanf("%lf", &data[data_size].x[j]);
    int t;
    scanf("%d", &t);
    data[data_size].y = (t == 1);
    data_size++;
    if (data_size >= MAX_TRAINING_DATA_SIZE) {
      printf("training data size is too large\n");
      return -1;
    }
  }
  return data_size;
}

int validate(const Data data[], int data_size, double w[]) {
  int right = 0;
  for (int i = 0; i < data_size; i++) {
    if (sign(product(w, data[i].x)) == data[i].y)
      right++;
  }
  return right;
}

void print(const double a[]) {
  for (int i = 0; i < DIM; i++)
    printf("%lf ", a[i]);
  printf("\n");
}

#endif
