#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//#define TESTING

#ifdef TESTING
const char *test_input = R"(4
3 3 1 1
1 1 3 1
3 3 1 1
1 1 3 3
)";

std::stringstream io(test_input);

#else
std::istream& io = std::cin;
#endif


int main() {
  int N;
  io >> N;

  int values[N][N];

  for (int i = 0; i < N; i++) {
    for (int n = 0; n < N; n++) {
      io >> values[i][n];
    }
  }

  // alternating, choose either the even or odd max for each row.
  int row_max_total = 0;
  for (int i = 0; i < N; i++) {
    int row_even = 0;
    int row_odd = 0;
    for (int n = 0; n < N; n++) {
      if (n%2) {
        row_odd += values[i][n];
      } else {
        row_even += values[i][n];
      }
    }
    row_max_total += std::max(row_even, row_odd);
  }

  // alternating, choose either the even or odd max for each row.
  int col_max_total = 0;
  for (int n = 0; n < N; n++) {
    int col_even = 0;
    int col_odd = 0;
    for (int i = 0; i < N; i++) {
      if (i%2) {
        col_odd += values[i][n];
      } else {
        col_even += values[i][n];
      }
    }
    col_max_total += std::max(col_even, col_odd);
  }

  std::cout << std::max(row_max_total, col_max_total) << std::endl;

  return 0;
}

