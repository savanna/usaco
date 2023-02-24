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
  const char *test_input = R"(7 4 7
1 2
2 3
3 4
4 5
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

  for (int i = 0; i < N; i++) {
    cows[i] = i;
  }

  // get the initial transformation.
  for (int i = 0; i < M; i++) {
    std::reverse(cows.begin() + moves[i].first, cows.begin() + moves[i].second + 1);
  }

  // positive is repeat#, negative is parent.
  std::array<std::optional<int>, kLimit> parent;
  // for each cow, try loop through the positions we just got, and stop as soon as it repeats.
  for (int n = 0; n < N; n++) {
    int repeat = 0;
    std::optional<int>& p = parent[n];
    while (p.has_value()) {
      if(p < 0) {
        // parent
        p = parent[-1 * p.value()];
      } else {
        repeat = p.value();
        break;
      }
    }

    int current_position = n;  // n is starting position
    int i = repeat ? K - K%repeat : 0;
    for (; i < K; i++) {
      current_position = cows[current_position];
      repeat++;

      if (current_position == n) {
        // it repeats, now we can stop repeating.
        i = K - K%repeat - 1;
        parent[n] = repeat;
      } else {
        parent[current_position] = -1 * n;
      }
    }

    out << current_position + 1 << std::endl;
  }
}

