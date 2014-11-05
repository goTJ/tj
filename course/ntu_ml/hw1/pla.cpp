// vim:ft=cpp:foldmethod=marker
#include <cstdlib>
#include <ctime>
#include <random>

#include "common.h"

using namespace std;

std::random_device rd;
std::mt19937 gen(rd());

void init_permutation(int n, int permutation[]) {
  for (int i = 0; i < n; i++) {
    std::uniform_int_distribution<> dis(0, i);
    int j = dis(gen);
    permutation[i] = permutation[j];
    permutation[j] = i;
  }
}

void calculate_w(const Data data[], int permutation[], int training_data_size,
    int max_updates, double w[], double w_pocket[]) {
  int update = 0;
  int best_result = 0;
  std::uniform_int_distribution<> dis(0, training_data_size - 1);
  while (true) {
    bool all_right = true;
    for(int i = 0; i < training_data_size; i++) {
      //int idx = permutation[i];
      int idx = dis(gen);
      if (sign(product(w, data[idx].x)) != data[idx].y) {
        //printf("%d %d\n", idx, data[idx].y);
        //print(w);
        add(data[idx], w);
        int result = validate(data, training_data_size, w);
        if (result > best_result) {
          //printf("best: %d\n", best_result);
          best_result = result;
          for (int i = 0; i < DIM; i++)
            w_pocket[i] = w[i];
        }
        update++;
        if (update == max_updates) {
          //printf("%d\n", update);
          return;
        }
        all_right = false;
      }
    }
    if (all_right) {
      //printf("%d\n", update);
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
  double w_pocket[DIM]{};
  //calculate_w(data, permutation, training_data_size, 250, w, w_pocket);
  calculate_w(data, permutation, training_data_size, 100, w, w_pocket);
  print(w);
  print(w_pocket);
	return 0;
}
