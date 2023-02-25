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
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <cmath>
#include <optional>

using namespace std;


int main() {
//#define TESTING

#ifdef TESTING
  const char *test_input = R"(11
1 -3
0 -2
0 -1
0 1
0 0
1 0
-1 0
2 0
-2 0
3 0
-3 0
)";

  std::stringstream in(test_input);
  std::ostream& out = std::cout;

#else  //!TESTING

#define USING_FILE

#ifdef USING_FILE
  std::string input_file = "triangles.in";
  std::string output_file = "triangles.out";
  std::ofstream out;
  out.open(output_file);
  std::ifstream in;
  in.open(input_file);

#else
  std::istream& in = std::cin;
  std::ostream& out = std::cout;

#endif
#endif

  struct P {
    P(int x, int y): x(x), y(y) {};

    int x, y;
    // prefix sum for all 4 directions.
    int64_t top = 0;
    int64_t right = 0;
    int64_t bottom = 0;
    int64_t left = 0;
  };

  constexpr int kLimit = 10;

  std::vector<P> points;
  std::map<int, std::vector<int>> same_x_points;
  std::map<int, std::vector<int>> same_y_points;

  int N;
  in >> N;

  for (int i = 0; i < N; i++) {
    int x,y;
    in >> x >> y;
    points.push_back({x, y});

    same_x_points[x].push_back(i);
    same_y_points[y].push_back(i);
  }

  for (auto xi : same_x_points) {
    std::sort(xi.second.begin(), xi.second.end(), [&](int l, int r) {
        return points[l].y < points[r].y;
        });

    // skip the first point, it has prefix sum 0
    for (int i = 1; i < xi.second.size(); i++) {
      auto& current = points[xi.second[i]];
      auto& previous = points[xi.second[i-1]];

      current.bottom += previous.bottom + i*(current.y - previous.y);
    }

    for (int i = xi.second.size() - 2; i >= 0; i--) {
      auto& current = points[xi.second[i]];
      auto& previous = points[xi.second[i+1]];

      current.top += previous.top + (xi.second.size() - 1 - i)*(previous.y - current.y);
    }
  }

  for (auto xi : same_y_points) {
    std::sort(xi.second.begin(), xi.second.end(), [&](int l, int r) {
        return points[l].x < points[r].x;
        });

    // skip the first point, it has prefix sum 0
    for (int i = 1; i < xi.second.size(); i++) {
      auto& current = points[xi.second[i]];
      auto& previous = points[xi.second[i-1]];

      current.left += previous.left + i*(current.x - previous.x);
    }

    for (int i = xi.second.size() - 2; i >= 0; i--) {
      auto& current = points[xi.second[i]];
      auto& previous = points[xi.second[i+1]];

      current.right += previous.right + (xi.second.size() - 1 - i)*(previous.x - current.x);
    }
  }

  int64_t total = 0;
  for (auto p : points) {
    total += (p.left + p.right) * (p.top + p.bottom);
    total = total % (1000'000'007);
  }

  out << total % (1000'000'007) << std::endl;
}

