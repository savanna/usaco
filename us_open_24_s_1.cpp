#include <algorithm>
#include <array>
#include <memory>
#include <bitset>
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
#include <queue>

using namespace std;


int main() {
#define TESTING
#define USING_FILE

#ifdef TESTING
#ifdef USING_FILE
  std::string input_file = "test_data/prob1_silver_open24/2.in";
  std::ifstream in;
  in.open(input_file);
#else //USING_FILE
  const char *test_input = R"(6 3
3 1 4159 2 6 5
)";
  std::stringstream in(test_input);
#endif //USING_FILE

  // always output in console when testing
  std::ostream& out = std::cout;

#else  //TESTING

#ifdef USING_FILE
  std::string input_file = "prob1_silver_open24/4.in";
  std::ifstream in;
  in.open(input_file);

  std::string output_file = "ride.out";
  std::ofstream out;
  out.open(output_file);

#else //USING_FILE
  std::istream& in = std::cin;
  std::ostream& out = std::cout;

#endif //USING_FILE
#endif //TESTING
  int n, k;
  in >> n >> k;
  priority_queue<pair<int64_t, int>, vector<pair<int64_t, int>>,
                 greater<pair<int64_t, int>>>
      interviews;
  const int kSize = 3 * 100'000;
  map<int, shared_ptr<bitset<kSize>>> is_interchangable;
  int i, counter;
  int64_t t;
  counter = 0;
  t = 0;
  int finisher = -1;

  for (int j=0; j < k; ++j) {
    in >> i;
    interviews.push(pair<int64_t, int>(i, counter));
    counter++;
  }

  while (counter <= n) {
    pair<int64_t, int> smol = interviews.top();
    interviews.pop();
    t = smol.first;
    vector<int> simults;
    simults.push_back(smol.second);
    while (interviews.top().first == smol.first) {
      simults.push_back(interviews.top().second);
      interviews.pop();
    }
    shared_ptr<bitset<kSize>> all_interchangables = make_shared<bitset<kSize>>();
    for (int simul : simults) {
      if (is_interchangable.count(simul)) {
        *all_interchangables |= *is_interchangable[simul];
      } else {
        (*all_interchangables)[simul] = 1;
      }
      if (counter < n) {
        in >> i;
        interviews.push(pair<int64_t, int>(i + t, simul));
        counter++;
      } else {
        finisher = simul;
        counter++;
      }
    }
    for (int simul : simults) {
      is_interchangable[simul] = all_interchangables;
    }
  }
  out << t << "\n";
  for (int i = 0; i < k; i++) {
    if ((*is_interchangable[finisher])[i]) {
      out << "1";
    } else {
      out << "0";
    }
  }
  cout << "\n";
}