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
const char *test_input = R"(6
E 3 5
N 5 3
E 4 6
E 10 4
N 11 1
E 9 2
)";

std::stringstream io(test_input);

#else
std::istream& io = std::cin;
#endif


struct P {
  int x;
  int y;
  char d;
  int id;
  int life = -1;
  int blocked_by = -1;
};

int main() {
  constexpr std::size_t kLimit = 1099;

  int N;
  io >> N;

  std::array<P, kLimit> points;

  // second int in the pair is the index in `points` which serves as the id.
  std::map<int, std::pair<int, int>> east_y_x;
  std::map<int, std::pair<int, int>> north_x_y;

  std::array<int, kLimit> block_count;

  for (int i = 0; i < N; i++) {
    io >> points[i].d >> points[i].x >> points[i].y;
    points[i].id = i;

    if (points[i].d == 'E') {
      east_y_x[points[i].y] = {points[i].x, i};
    } else {
      assert(points[i].d == 'N');
      north_x_y[points[i].x] = {points[i].y, i};
    }
  }

  std::function<void(P&)> get_blocker = [&](P& p){
    if (p.d == 'E') {
      auto iter = north_x_y.lower_bound(p.x);

      while(iter != north_x_y.end()) {
        P& p2 = points[iter->second.second];

        int time_to_hit = p2.x - p.x;
        int time_to_setup = p.y - p2.y;
        if (time_to_setup > 0 && time_to_setup < time_to_hit) {
          // potentially, let's check the blocker's life
          if (p2.life < 0) {
            get_blocker(p2);
          }

          if (time_to_setup < p2.life) {
            // indeed blocked.
            p.blocked_by = p2.id;
            p.life = time_to_hit;
            break;
          }
        }
        iter++;
      }

      if (p.life < 0) {
        p.life = INT32_MAX;
      }
    } else { // 'N'
      auto iter = east_y_x.lower_bound(p.y);

      while(iter != east_y_x.end()) {
        P& p2 = points[iter->second.second];

        int time_to_hit = p2.y - p.y;
        int time_to_setup = p.x - p2.x;
        if (time_to_setup > 0 && time_to_setup < time_to_hit) {
          // potentially, let's check the blocker's life
          if (p2.life < 0) {
            get_blocker(p2);
          }

          if (time_to_setup < p2.life) {
            // indeed blocked.
            p.blocked_by = p2.id;
            p.life = time_to_hit;
            break;
          }
        }
        iter++;
      }
      if (p.life < 0) {
        p.life = INT32_MAX;
      }
    }
  };

  for (int i = 0; i < N; i++) {
    if (points[i].life < 0) {
      get_blocker(points[i]);
    }
  }

  for (int i = 0; i < N; i++) {
    int current = i;
    while(current >= 0) {
      if (points[current].blocked_by >=0) {
        block_count[points[current].blocked_by]++;
      }
      current = points[current].blocked_by;
    }
  }

  for (int i = 0; i < N; i++) {
    std::cout << block_count[i] << std::endl;
  }

  return 0;
}

