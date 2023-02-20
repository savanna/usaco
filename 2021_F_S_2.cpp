#include <array>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

//#define TESTING false

#ifdef TESTING
constexpr bool is_testing = true;
const char *test_input = R"(10 5
22
22
22
22
22
22
22
22
22
8
)";

std::stringstream io(test_input);

#else
std::istream& io = std::cin;
#endif

int main() {
  int N, K;
  io >> N >> K;

  std::map<int, int> gaps;
  std::vector<int> connected_years;
  int total_connected_rounds = 0;
  std::vector<int> cows;

  for (int n = 0; n < N; ++n) {
    int year;
    io >> year;
    cows.push_back(year);
  }
  // sort list
  std::sort(cows.begin(), cows.end(), std::greater{});


  int start_round = INT32_MAX;
  int current_round = -1;
  int previous_cow_round = INT32_MAX;

  // find the connected groups
  // find the gaps, sort the gaps, maybe store in map
  for (int n = 0; n < N; ++n) {
    int y = cows[n];

    current_round = y/12 + 1;

    if (previous_cow_round == INT32_MAX) {
      start_round = current_round;
      previous_cow_round = current_round;
      continue;
    }

    int gap_round = previous_cow_round - current_round - 1;
    if(gap_round > 0) {
      // fill the gap
      gaps[gap_round] += 1;

      // record the last continuation
      int connected_round = start_round - previous_cow_round + 1;
      connected_years.push_back(connected_round);
      total_connected_rounds += connected_round;

      // start a new round
      start_round = current_round;
    }

    previous_cow_round = current_round;
  }

  bool last_jump = previous_cow_round > 1;
  if (last_jump) {
      // fill the gap
      gaps[previous_cow_round -1] += 1;
  }

  // record the last continuation
  int connected_round = start_round - previous_cow_round + 1;
  connected_years.push_back(connected_round);
  total_connected_rounds += connected_round;

  int total_jumps = connected_years.size() + (last_jump ? 1 : 0);

  int reduce_jumps = total_jumps - K;

  if(reduce_jumps > 0) {
    for(auto& gap : gaps) {
      if (reduce_jumps <= gap.second) {
        total_connected_rounds += gap.first * reduce_jumps;
        break;
      } else {
        total_connected_rounds += gap.first * gap.second;
        reduce_jumps -= gap.second;
      }
    }
  }

  std::cout << total_connected_rounds * 12 << std::endl;

  return 0;
}
