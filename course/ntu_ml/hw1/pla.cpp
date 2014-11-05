// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <random>

#define DIM 5
#define MAX_TRAINING_DATA_SIZE 500

using namespace std;

struct Data {
  double x[DIM];
  bool y;
};

std::random_device rd;
std::mt19937 gen(rd());

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

void print(const double a[]) {
  for (int i = 0; i < DIM; i++)
    printf("%lf ", a[i]);
  printf("\n");
}

int input(Data data[]) {
  int training_data_size = 0;

  while (scanf("%lf", &data[training_data_size].x[1]) == 1) {
    data[training_data_size].x[0] = 0.0;
    for (int j = 2; j < DIM; j++)
      scanf("%lf", &data[training_data_size].x[j]);
    int t;
    scanf("%d", &t);
    data[training_data_size].y = (t == 1);
    training_data_size++;
    if (training_data_size >= MAX_TRAINING_DATA_SIZE) {
      printf("training data size is too large\n");
      return -1;
    }
  }
  return training_data_size;
}

void init_permutation(int n, int permutation[]) {
  for (int i = 0; i < n; i++) {
    std::uniform_int_distribution<> dis(0, i);
    int j = dis(gen);
    permutation[i] = permutation[j];
    permutation[j] = i;
  }
}

void calculate_w(const Data data[], int permutation[], int training_data_size,
    int max_updates, double w[]) {
  int update = 0;
  while (true) {
    bool all_right = true;
    for(int i = 0; i < training_data_size; i++) {
      int idx = permutation[i];
      if (sign(product(w, data[idx].x)) != data[idx].y) {
        //printf("%d %d\n", idx, data[idx].y);
        //print(w);
        add(data[idx], w);
        update++;
        if (update == max_updates) {
          printf("%d\n", update);
          return;
        }
        all_right = false;
      }
    }
    if (all_right) {
      printf("%d\n", update);
      return;
    }
  }
}

int main(int argc, char *argv[]) {
  Data data[MAX_TRAINING_DATA_SIZE];
  int training_data_size = input(data);
  if (training_data_size == -1)
    return 1;

  int permutation[MAX_TRAINING_DATA_SIZE]{};
  init_permutation(training_data_size, permutation);
  
  double w[DIM]{};
  calculate_w(data, permutation, training_data_size, 250, w);
  //print(w);
	return 0;
}
