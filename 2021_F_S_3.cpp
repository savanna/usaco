#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

//#define TESTING

#ifdef TESTING
const char *test_input = R"(3
57 120 87
200 100 150
2 141 135
)";

std::stringstream io(test_input);

#else
std::istream& io = std::cin;
#endif

int64_t GetNumber(int N, int grass[][1000], int limit) {
  std::map<int, std::vector<int>> x_y;

  // find all the ones that's smaller than 100
  for (int y=0; y < N; ++y) {
    for (int x = 0; x < N; ++x) {
      io >> grass[x][y];
      if (grass[x][y] < limit) {
        x_y[x].push_back(y);
      }
    }
  }
  for (int x = 0; x < N; ++x) {
    std::make_heap(x_y[x].begin(), x_y[x].end(), std::greater{});
  }

  int64_t total = 0;
  // go throw each top left point and calculate the number of subsets
  for (int y=0; y < N; ++y) {
    auto x_y_head = x_y.begin();
    for (int x = 0; x < N; ++x) {
      if (grass[x][y] < limit) {
        continue;
      }

      int start_x = x;
      int end_y = N;

      while(x_y_head != x_y.end() && start_x > x_y_head->first) {
        x_y_head++;
      }

      if (x_y_head == x_y.end()) {
        // pass the last x point, all remaining x and y are available.
        total += end_y * (N - start_x);
        continue;
      }

      auto current_x_y_head = x_y_head;
      while(current_x_y_head != x_y.end()) {
        if (start_x <= current_x_y_head->first) {
          // ignore points that have smaller y, because we won't hit it
          while (!current_x_y_head->second.empty() && current_x_y_head->second.front() < y) {
            std::pop_heap(current_x_y_head->second.begin(), current_x_y_head->second.end(), std::greater{});
            current_x_y_head->second.pop_back();
          }
          if (current_x_y_head->second.empty()) {
            current_x_y_head++;
            continue;
          }

          // new limit on x
          total += (end_y - y) * (current_x_y_head->first - start_x);
          start_x = current_x_y_head->first;

          // new limit on y
          end_y = std::min(end_y, current_x_y_head->second.front());

          current_x_y_head++;
        }
      }
      // add the last batch
      // new limit on x = N
      total += (end_y - y) * (N - start_x);
    }
  }

  return total;
}

int main() {
  int N;
  io >> N;

  int grass[1000][1000];
  int64_t number = GetNumber(N, grass, 100) - GetNumber(N, grass, 101);
  std::cout << number << std::endl;
  return 0;
}

