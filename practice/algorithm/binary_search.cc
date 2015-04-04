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

  assert(!bsearch({}, 1));
  assert(!bsearch2({}, 1));
  for (int v = 0; v < 13; v++) {
    printf("try %d\n", v);
    printf("bsearch:\n");
    assert(bsearch(values, v) == (v&1 == 1));
    printf("bsearch2:\n");
    assert(bsearch2(values, v) == (v&1 == 1));
  }
  printf("find peak\n");
  assert(find_peak({}) == -1);
  printf("find peak\n");
  assert(find_peak({11}) == 11);
  printf("find peak\n");
  assert(find_peak({1, 11}) == 11);
  printf("find peak\n");
  assert(find_peak({11, 1}) == 11);
  printf("find peak\n");
  assert(find_peak({1, 3, 5, 7, 9, 11, 2}) == 11);
  printf("find peak\n");
  assert(find_peak({1, 11, 10, 8, 6, 4, 2}) == 11);
  printf("find peak\n");
  assert(find_peak({1, 3, 5, 7, 9, 11}) == 11);
  printf("find peak\n");
  assert(find_peak({11, 10, 8, 6, 4, 2}) == 11);
  return 0;
}
