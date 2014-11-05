// vim:ft=cpp:foldmethod=marker
#include <cstdio>

#include "common.h"

using namespace std;

int main(int argc, char *argv[]){
  double w[DIM], w_pocket[DIM];
  for (int i = 0; i < DIM; i++)
    scanf("%lf", &w[i]);
  for (int i = 0; i < DIM; i++)
    scanf("%lf", &w_pocket[i]);

  Data data[MAX_TRAINING_DATA_SIZE];
  int test_data_size = input(data);
  if (test_data_size == -1)
    return 1;

  printf("%lf %lf\n", (double)validate(data, test_data_size, w) / test_data_size,
      (double)validate(data, test_data_size, w_pocket) / test_data_size);

	return 0;
}
