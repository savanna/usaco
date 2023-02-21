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
#include <cassert>

//#define TESTING

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

  // all the visited cows
  std::unordered_set<int> position_set[kLimit];
  // position and the cow in it.
  int positions[kLimit];
  // the end position of each cow after K moves.
  int cow_end_positions[kLimit];
  // group id is index, value is the total traveled.
  std::vector<int> groups;
  // index is cow, value is group id.
  int cow_group[kLimit];

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
    cow_group[n] = -1;  // no group
  }

  // go through it once, record all the cows' traces.
  for(int k = 0; k < K; ++k) {
    auto& m = moves[k];

    int cow_first = positions[m.first];
    int cow_second = positions[m.second];

    position_set[cow_first].insert(m.second);
    position_set[cow_second].insert(m.first);

    std::swap(positions[m.first], positions[m.second]);
  }

  // find end positions
  for (int n = 0; n < N; ++n) {
    cow_end_positions[positions[n]] = n;
  }

  // for each cow find all the connected groups
  for (int n = 0; n < N; ++n) {
    if (cow_group[n] < 0) {
      cow_group[n] = groups.size();
      groups.push_back(0);
    } else {
      // group is already computed.
      continue;
    }

    std::unordered_set<int> visited{n};
    std::unordered_set<int> all_visited{position_set[n]};

    int current_cow = n;
    int next_cow = cow_end_positions[current_cow];
    while(visited.count(next_cow) == 0) {
      // new position
      visited.insert(next_cow);
      all_visited.merge(position_set[next_cow]);
      assert(cow_group[next_cow] < 0);
      cow_group[next_cow] = cow_group[n];

      current_cow = next_cow;
      next_cow = cow_end_positions[current_cow];
    }

    groups[cow_group[n]] = all_visited.size();
  }

  for (int n = 0; n < N; ++n) {
    std::cout << groups[cow_group[n]] << std::endl;
  }

  return 0;
}

