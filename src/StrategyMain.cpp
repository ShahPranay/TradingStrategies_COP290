#include "LinearRegression.h"
#include <string>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " strategy symbol ...\n";
    return 1;
  }

  // Extract command line arguments
  std::string strategy = argv[1];
  std::string symbol = argv[2];

  if (strategy == "LinearRegression")
  {
    if (argc < 9) {
      std::cerr << "Usage: " << argv[0] << " strategy symbol x p \
        train_start_date train_end_date start_date end_date";
      return 1;
    }
    int x = std::stoi(argv[3]);
    double p = std::stod(argv[4]);
    std::string train_start_date = argv[5], train_end_date = argv[6], start_date = argv[7], end_date = argv[8];
    Strategy *strat = (Strategy *) new LinearRegression(symbol, train_start_date, train_end_date, start_date, end_date, x, p);
    strat->runStrategy();
  }

  return 0;
}
