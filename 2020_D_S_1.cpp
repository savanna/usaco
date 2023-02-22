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
#include <cmath>

//#define TESTING

#ifdef TESTING
const char *test_input = R"(4
1 2
1 3
1 4
)";

std::stringstream io(test_input);

#else
std::istream& io = std::cin;
#endif


int main() {
  constexpr std::size_t kLimit = 105'000;

  int N;
  io >> N;

  // node to node set
  std::vector<int> connections[kLimit];

  for (int i = 0; i < N - 1; i++) {
    int from, to;
    io >> from >> to;

    connections[from].push_back(to);
    connections[to].push_back(from);
  }

  int total = 0;
  std::vector<int> pending;
  bool visited[kLimit];

  for (int i = 0; i <= N; i++) {
    visited[i] = false;
  }
  pending.push_back(1);
  visited[1] = true;

  while (pending.empty() == false) {
    int current = *pending.rbegin();
    pending.pop_back();

    int fan_out = 0;

    for (auto i : connections[current]) {
      if (visited[i]) {
        continue;
      }

      fan_out++;
      pending.push_back(i);
      visited[i] = true;
    }

    if (fan_out == 0) {
      continue;
    }

    total += fan_out + 1 + std::log2(fan_out);
  }

  std::cout << total << std::endl;

  return 0;
}

