// vim:ft=cpp:foldmethod=marker
#include <cassert>
#include <cstdio>
#include <set>
#include <vector>

using namespace std;

class StampValueChecker {
 public:
  StampValueChecker(int row, int col);
  int CalculateContinuousValue(vector<int> values) const ;
  int GetTearingWaySize() const;

 private:
  void InitTearingWays();
  void FindTearingWays(unsigned int way);

  int row_, col_;
  set<unsigned int> tearing_ways_;
};

StampValueChecker::StampValueChecker(int row, int col)
  : row_(row), col_(col) {
  assert(row*col < sizeof(int) * 8);

  InitTearingWays();
}

int StampValueChecker::CalculateContinuousValue(
    vector<int> values) const {
  assert(values.size() == row_ * col_);
  set<int> combined_values;

  for (auto way : tearing_ways_) {
    int v = 0;
    for (int pos = 0; pos < row_ * col_; pos++) {
      if ((1 << pos) & way)
        v += values[pos];
    }
    combined_values.insert(v);
  }
  int previous_value = 0;
  for (auto v : combined_values) {
    if (v - previous_value > 1)
      break;
    previous_value = v;
  }
  return previous_value;
}

int StampValueChecker::GetTearingWaySize() const {
  return tearing_ways_.size();
}

void StampValueChecker::InitTearingWays() {
  for (int pos = 0; pos < row_ * col_; pos++)
    FindTearingWays(1 << pos);
}

void StampValueChecker::FindTearingWays(unsigned int way) {
  if (!tearing_ways_.insert(way).second)
    return;

  for (int pos = 0; pos < row_ * col_; pos++) {
    if ((1 << pos) & way) {
      int r = pos / col_;
      int c = pos % col_;
      if (r + 1 < row_) FindTearingWays(way | (1 << ((r + 1) * col_ + c)));
      if (r - 1 >= 0) FindTearingWays(way | (1 << ((r - 1) * col_ + c)));
      if (c + 1 < col_) FindTearingWays(way | (1 << (r * col_ + c + 1)));
      if (c - 1 >= 0) FindTearingWays(way | (1 << (r * col_ + c - 1)));
    }
  }
}

void FindMaxContinuousValue(const StampValueChecker& checker, int value_size,
    vector<int>* values, vector<bool>* used_values, int* max_continuous_value,
    vector<int>* max_values) {
  if (values->size() < value_size) {
    for (int v = 1; v <= checker.GetTearingWaySize(); v++) {
      if (!used_values->at(v)) {
        values->push_back(v);
        used_values->at(v) = true;
        FindMaxContinuousValue(checker, value_size,
            values, used_values, max_continuous_value, max_values);
        used_values->at(v) = false;
        values->pop_back();
      }
    }
  } else {
    int cv = checker.CalculateContinuousValue(*values);
    if (cv >= *max_continuous_value) {
      *max_continuous_value = cv;
      *max_values = *values;
      printf("max: %d\n", *max_continuous_value);
      for (auto v : *values)
        printf("%d ", v);
      printf("\n");
    }
  }
}

int main(int argc, char *argv[]){
  int row, col;
  scanf("%d%d", &row, &col);
  StampValueChecker checker(row, col);
  vector<int> values, max_values;
  int max_continuous_value;
  vector<bool> used_values;
  used_values.resize(checker.GetTearingWaySize() + 1, false);
  values.reserve(row * col);
  FindMaxContinuousValue(checker, row * col,
      &values, &used_values, &max_continuous_value, &values);
  printf("%d\n", max_continuous_value);
	return 0;
}
