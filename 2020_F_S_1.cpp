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
//  const char *test_input = R"(7 4 7
//1 2
//2 3
//3 4
//4 5
//)";
  const char *test_input = R"(7 2 2
2 5
3 7
)";

  std::stringstream in(test_input);
  std::ostream& out = std::cout;

#else  //!TESTING

#define USING_FILE

#ifdef USING_FILE
  std::string input_file = "swap.in";
  std::string output_file = "swap.out";
  std::ofstream out;
  out.open(output_file);
  std::ifstream in;
  in.open(input_file);

#else
  std::istream& in = std::cin;
  std::ostream& out = std::cout;

#endif
#endif

  constexpr int kLimit = 100'999;

  // cows and their positions.
  std::array<int, kLimit> cows;

  std::vector<std::pair<int, int>> moves;

  int N, M, K;
  in >> N >> M >> K;

  for (int i = 0; i < M; i++) {
    int x, y;
    in >> x >> y;
    moves.push_back({x -1, y -1});
  }

  std::array<int, kLimit> parent;
  std::array<int, kLimit> repeat_values;
  std::unordered_map<int, std::vector<int>> repeat_map;
  for (int i = 0; i < N; i++) {
    cows[i] = i;
    parent[i] = -1;
    repeat_values[i] = -1;
  }

  // get the initial transformation.
  for (int i = 0; i < M; i++) {
    std::reverse(cows.begin() + moves[i].first, cows.begin() + moves[i].second + 1);
  }

  for (int n = 0; n < N; n++) {
    int repeat = 0;
    int p = parent[n];
    if (p>=0){
      continue;
    }

    int current_position = n;  // n is starting position
    std::vector<int> repeat_list;
    repeat_list.push_back(n);
    while(true) {
      current_position = cows[current_position];
      repeat++;

      if (current_position == n) {
        parent[n] = n;
        repeat_values[n] = repeat;
        repeat_map.emplace(n, std::move(repeat_list));
        break;
      } else {
        parent[current_position] = n;
        repeat_values[current_position] = repeat;
        repeat_list.push_back(current_position);
      }
    }
  }

  // for each cow, try loop through the positions we just got, and stop as soon as it repeats.
  for (int n = 0; n < N; n++) {
    int repeat = 0;
    int repeat_offset = 0;
    int p = parent[n];
    assert(p == parent[p]);
    repeat = repeat_values[p];
    if (p != n) {
      repeat_offset = repeat_values[n];
    }

    int i = K%repeat;
    int current_position = repeat_map[p][(i + repeat_offset)%repeat];

    out << current_position + 1 << std::endl;
  }
}

