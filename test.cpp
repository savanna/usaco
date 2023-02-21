#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_set>

#define TESTING

#ifdef TESTING
const char *test_input = R"(5 4
1 3
1 2
2 3
2 4
)";

std::stringstream io(test_input);

#else
std::istream& io = std::cin;
#endif


int main() {
  int N, K;
  io >> N >> K;

  constexpr size_t kLimit = 100'000;

  std::unordered_set<int> position_set[kLimit];
  int positions[kLimit];

  std::unordered_set<int> cow_touched;
  int previous_cow_touched = 0;
  bool is_new = true;

  std::pair<int, int> moves[2*kLimit];

  // read all the moves
  for(int k = 0; k < K; ++k) {
    io >> moves[k].first >> moves[k].second;
    moves[k].first -= 1;
    moves[k].second -= 1;
  }

  // initialize the possitions
  for (int n = 0; n < N; ++n) {
    positions[n] = n;
    position_set[n].insert(n);
  }

  // go through it once
  for(int k = 0; k < K; ++k) {
    auto& m = moves[k];

    int cow_first = positions[m.first];
    int cow_second = positions[m.second];

    cow_touched.insert(cow_first);
    cow_touched.insert(cow_second);

    is_new = is_new || position_set[cow_first].count(m.second) || position_set[cow_second].count(m.first);

    position_set[cow_first].insert(m.second);
    position_set[cow_second].insert(m.first);
  }

  // continue when there are new position showing up or there are new cows being touched.
  while (is_new || previous_cow_touched < cow_touched.size()) {
    is_new = false;
    previous_cow_touched = cow_touched.size();
    for(int k = 0; k < K; ++k) {
      auto& m = moves[k];

      int cow_first = positions[m.first];
      int cow_second = positions[m.second];

      cow_touched.insert(cow_first);
      cow_touched.insert(cow_second);

      is_new = is_new || !position_set[cow_first].count(m.second) || !position_set[cow_second].count(m.first);

      position_set[cow_first].insert(m.second);
      position_set[cow_second].insert(m.first);

      std::swap(positions[m.first], positions[m.second]);
    }
  }

  for (int n = 0; n < N; ++n) {
    std::cout << position_set[n].size() << std::endl;
  }

  return 0;
}

