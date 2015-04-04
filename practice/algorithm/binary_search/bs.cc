#include <cassert>
#include <cstdio>
#include <vector>

using std::vector;

bool bsearch(const vector<int>& values, int v) {
  int l = 0, h = values.size();

  while (l < h) {
    int m = (l + h) / 2;
    printf("%d %d: %d\n", l, h, m);
    if (values.at(m) < v) {
      l = m + 1;
    } else if (values.at(m) > v) {
      h = m;
    } else {
      return true;
    }
  }
  return false;
}

bool bsearch2(const vector<int>& values, int v) {
  int l = 0, h = values.size();

  while (l < h) {
    int m = (l + h) / 2;
    printf("%d %d: %d\n", l, h, m);
    if (values.at(m) < v) {
      l = m + 1;
    } else {
      h = m;
    }
  }
  printf("%d %d\n", l, h);
  return l < values.size() && values.at(l) == v;
}

int find_peak(const vector<int>& values) {
  int l = 0, h = values.size();

  while (l < h) {
    int m = (l + h) / 2;
    printf("%d %d: %d\n", l, h, m);

    if (m + 1 < values.size() && values.at(m + 1) > values.at(m)) {
      l = m + 1;
    } else {
      h = m;
    }
  }
  return (l < values.size()) ? values.at(l) : -1;
}

int main() {
  vector<int> values = { 1, 3, 5, 7, 9, 11, 11, 11 };

  /*
  for (int i = 0; i < 13; i++) {
    printf("try %d\n", i);
    assert(bsearch2(values, i) == (i&1 == 1 && i < 13));
  }
  */
  printf("find peak\n");
  printf("%d\n", find_peak({}));
  printf("%d\n", find_peak({11}));
  printf("%d\n", find_peak({1, 11}));
  printf("%d\n", find_peak({11, 1}));
  printf("%d\n", find_peak({1, 3, 5, 7, 9, 11, 2}));
  printf("%d\n", find_peak({1, 11, 10, 8, 6, 4, 2}));
  printf("%d\n", find_peak({1, 3, 5, 7, 9, 11}));
  printf("%d\n", find_peak({11, 10, 8, 6, 4, 2}));
  return 0;
}
