// A = { v : v = a + b * sqrt(2) }.
// Given N, get the minimun N values in A.
#include <cstdio>
#include <cmath>
#include <set>

using namespace std;

int main(int argc, char *argv[]){
  double sqrt2 = sqrt(2);
  double sqrt2_multiple = 0.0;
  int n;
  int integer = 0;
  set<double> decimals;

  scanf("%d", &n);
  while(n) {
    // Generate all possible numbers in [integer, integer + 1)
    // in the increasing order.
    if (sqrt2_multiple < integer + 1) {
      decimals.insert(sqrt2_multiple - integer);
      sqrt2_multiple += sqrt2;
    }
    for (double decimal : decimals) {
      printf("%.2lf\n", integer + decimal);
      if (--n == 0)
        break;
    }
    integer++;
  }
	return 0;
}
