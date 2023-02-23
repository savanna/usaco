#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <cmath>

//#define TESTING

#ifdef TESTING
const char *test_input = R"(4
0 2
1 0
2 3
3 5
)";

std::stringstream io(test_input);

#else
std::istream& io = std::cin;
#endif


int main() {
  constexpr std::size_t kLimit = 2599;

  int N;
  io >> N;

  std::map<int, int> x_y;
  std::map<int, int> y_x;

  std::map<int, int> x_sequence;
  std::map<int, int> y_sequence;

  for (int i = 0; i < N; i++) {
    int x, y;
    io >> x >>y;

    x_y[x] = y;
    y_x[y] = x;
  }

  int n = 0;
  for (auto i = x_y.begin(); i != x_y.end(); ++i) {
    x_sequence[i->first] = n;
    n++;
  }

  n=0;
  for (auto i = y_x.begin(); i != y_x.end(); ++i) {
    y_sequence[i->first] = n;
    n++;
  }

  // build the prefix sum matrix.
  std::array<std::array<int, kLimit>, kLimit> points;
  std::vector<std::pair<int, int>> point_list;
  auto x_y_iter = x_y.begin();
  for (int x = 0; x < N; x++) {
    auto y_x_iter = y_x.begin();
    for (int y = 0; y < N; y++) {
      points[x][y] = 0;
      if (y>0) {
        points[x][y] += points[x][y-1];
      }
      if (x > 0) {
        points[x][y] += points[x-1][y];
      }
      if (x>0 && y>0) {
        points[x][y] -= points[x-1][y-1];
      }

      if (y_x_iter->second == x_y_iter->first) {
        points[x][y] += 1;
        point_list.push_back({x, y});  // natually sorted by x;
      }

      ++y_x_iter;
    }
    ++x_y_iter;
  }

  int64_t total = 0;
  for (int x1 = 0; x1 < N; x1++) {
    for (int x2 = x1; x2 < N; x2++) {
      auto p1 = point_list[x1];
      auto p2 = point_list[x2];

      int y1 = std::min(p1.second, p2.second);
      int y2 = std::max(p1.second, p2.second);

      int a = points[x2][y1];
      if(x1 > 0) {
        a -= points[x1-1][y1];
      }

      int c = points[x2][N-1];
      if(x1 > 0) {
        c -= points[x1-1][N-1];
      }
      if (y2 > 0) {
        c -= points[x2][y2-1];
      }
      if (x1>0 && y2>0) {
        c += points[x1-1][y2-1];
      }

      total += a*c;
    }
  }

  std::cout << total + 1<< std::endl;

  return 0;
}

