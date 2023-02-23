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
  constexpr std::size_t kLimit = 25;

  int N;
  io >> N;

  std::map<int, int> x_y;
  std::map<int, int> y_x;

  // for each y,x, the number of points in the rect[(0,0) - (x,y)]
  std::map<int, std::map<int, int>> y_x_seen_point;

  for (int i = 0; i < N; i++) {
    int x, y;
    io >> x >>y;

    x_y[x] = y;
    y_x[y] = x;
  }

  std::set<int> seen_x;
  for (auto y : y_x) {
    seen_x.insert(y.second);

    if (y_x_seen_point.size()) {
      // copy the previous map over
      y_x_seen_point[y.first] = y_x_seen_point.rbegin()->second;
    }

    // increment all sums after this new x value
    int num_of_x = 0;
    for(auto iter = ++seen_x.find(y.second); iter != seen_x.end(); ++iter) {
      y_x_seen_point[y.first][*iter] += 1;
      num_of_x++;
    }

    // set the current sum to (total - the number bigger than current x)
    y_x_seen_point[y.first][y.second] = seen_x.size() - num_of_x;
  }

  int64_t total = 0;
  for (auto y1 = y_x.begin(); y1 != y_x.end(); ++y1) {
    for (auto y2 = y1; y2 != y_x.end(); ++y2) {

      if (y1->second <= y2->second) {
        int a = y_x_seen_point[y2->first][y1->second] - y_x_seen_point[y1->first][y1->second] + 1;

        auto y2_last = y_x_seen_point[y2->first].rbegin()->second;
        auto y1_last = y_x_seen_point[y1->first].rbegin()->second;

        auto y2_x2 = y_x_seen_point[y2->first][y2->second];
        int y1_x2 = 0;
        auto maybe_y1_x2 = y_x_seen_point[y1->first].lower_bound(y2->second);
        if (maybe_y1_x2 == y_x_seen_point[y1->first].end()) {
          y1_x2 = y_x_seen_point[y1->first].rbegin()->second;
        } else if (maybe_y1_x2->first > y2->second) {
          y1_x2 = maybe_y1_x2->second - 1;
        } else {
          y1_x2 = maybe_y1_x2->second;
        }

        int c = y2_last - y1_last - y2_x2 + y1_x2 + 1;

        total += a*c;
      } else {
        int a = y_x_seen_point[y2->first][y2->second] - y_x_seen_point[y1->first][y2->second];

        auto y2_last = y_x_seen_point[y2->first].rbegin()->second;
        auto y1_last = y_x_seen_point[y1->first].rbegin()->second;

        auto y2_x2 = y_x_seen_point[y2->first][y1->second];
        auto y1_x2 = y_x_seen_point[y1->first][y1->second];

        int c = y2_last - y1_last - y2_x2 + y1_x2 + 1;

        total += a*c;
      }
    }
  }

  std::cout << total + 1<< std::endl;

  return 0;
}

