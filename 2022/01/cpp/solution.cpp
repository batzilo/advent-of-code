#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

std::string infile = "input.txt";

int input_to_queue(std::priority_queue<int>& q) {
  std::ifstream input(infile);
  if (!input.is_open()) {
    std::cout << "Failed to open file: " << infile << std::endl;
    return -1;
  }

  std::string line;
  int curr = 0;
  while (getline(input, line)) {
    if (line.empty()) {
      q.push(curr);
      curr = 0;
    } else {
      int i;
      std::stringstream(line) >> i;
      curr += i;
    }
  }

  input.close();
  return 0;
}

int main() {
  std::priority_queue<int> q;
  int res = input_to_queue(q);
  if (res) {
    return res;
  }

  int max = q.top();
  q.pop();
  int max2 = q.top();
  q.pop();
  int max3 = q.top();

  std::cout << max << std::endl;
  std::cout << max + max2 + max3 << std::endl;
  return 0;
}
