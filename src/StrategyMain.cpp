#include "LinearRegression.h"
#include "Basic.h"
#include "DMA.h"
#include "AMA.h"
#include "MACD.h"
#include "RSI.h"
#include "ADX.h"
#include "PAIRS.h"
#include "BestOfAll.h"
#include "FisherTransform.h"
#include "PAIRS2.h"
#include "TSI.h"
#include <string>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " strategy symbol ...\n";
    return 1;
  }

  // Extract command line arguments
  std::string strategy = argv[1];
  

  if (strategy == "LINEAR_REGRESSION")
  {
    if (argc < 9) {
      std::cerr << "Usage: " << argv[0] << " strategy symbol x p \
        train_start_date train_end_date start_date end_date";
      return 1;
    }
    std::string symbol = argv[2];
    int x = std::stoi(argv[3]);
    double p = std::stod(argv[4]);
    std::string train_start_date = argv[5], train_end_date = argv[6], start_date = argv[7], end_date = argv[8];

    Strategy *strat = (Strategy *) new LinearRegression(symbol, train_start_date, train_end_date, start_date, end_date, x, p);
    strat->runStrategy();
    strat->writeToFiles();
  }
  else if(strategy == "BASIC")
  {
    if (argc < 7) {
      std::cerr << "Usage: " << argv[0] << " strategy symbol n x \
        start_date end_date";
      return 1;
    }
    std::string symbol = argv[2];
    int n = std::stoi(argv[3]);
    int x = std::stoi(argv[4]);
    std::string start_date = argv[5], end_date = argv[6];
    Strategy *strat = (Strategy *) new Basic(symbol, start_date, end_date, n, x);
    strat->runStrategy();
    strat->writeToFiles();
  }
  else if(strategy == "DMA")
  {
    if (argc < 8) {
      std::cerr << "Usage: " << argv[0] << " strategy symbol n x p \
        start_date end_date";
      return 1;
    }
    std::string symbol = argv[2];
    int n = std::stoi(argv[3]);
    int x = std::stoi(argv[4]);
    int p = std::stoi(argv[5]); 
    std::string start_date = argv[6], end_date = argv[7];
    Strategy *strat = (Strategy *) new DMA(symbol, start_date, end_date, n, x, p);
    strat->runStrategy();
    strat->writeToFiles();
  }
  else if(strategy == "DMA++"){
    if (argc < 11) {
      std::cerr << "Usage: " << argv[0] << " strategy symbol n x p\
        max_hold_days start_date end_date c1 c2";
      return 1;
    }
    std::string symbol = argv[2];
    int n = std::stoi(argv[3]);
    int x = std::stoi(argv[4]);
    int p = std::stoi(argv[5]);
    int max_hold_days = std::stoi(argv[8]);
    double c1 = std::stod(argv[9]);
    double c2 = std::stod(argv[10]);
    std::string start_date = argv[6], end_date = argv[7];
    Strategy *strat = (Strategy *) new AMA(symbol, start_date, end_date, n, x, p, max_hold_days, c1, c2);
    strat->runStrategy();
    strat->writeToFiles();
  }
  else if(strategy == "MACD")
  {
    if (argc < 6) {
      std::cerr << "Usage: " << argv[0] << " strategy symbol x \
        start_date end_date";
      return 1;
    }
    std::string symbol = argv[2];
    int x = std::stoi(argv[3]);
    std::string start_date = argv[4], end_date = argv[5];
    Strategy *strat = (Strategy *) new MACD(symbol, start_date, end_date, x);
    strat->runStrategy();
    strat->writeToFiles();
  }
  else if(strategy == "RSI")
  {
    if (argc < 9) {
      std::cerr << "Usage: " << argv[0] << " strategy symbol n x \
        oversold_threshold overbought_threshold start_date end_date";
      return 1;
    }
    std::string symbol = argv[2];
    int n = std::stoi(argv[3]);
    int x = std::stoi(argv[4]);
    double oversold_threshold = std::stod(argv[7]);
    double overbought_threshold = std::stod(argv[8]);
    std::string start_date = argv[5], end_date = argv[6];
    Strategy *strat = (Strategy *) new RSI(symbol, start_date, end_date, n, x, oversold_threshold, overbought_threshold);
    strat->runStrategy();
    strat->writeToFiles();
  }
  else if(strategy == "ADX")
  {
    if (argc < 8) {
      std::cerr << "Usage: " << argv[0] << " strategy symbol n x \
        adx_threshold start_date end_date";
      return 1;
    }
    std::string symbol = argv[2];
    int n = std::stoi(argv[3]);
    int x = std::stoi(argv[4]);
    double adx_threshold = std::stod(argv[7]);
    std::string start_date = argv[5], end_date = argv[6];
    Strategy *strat = (Strategy *) new ADX(symbol, start_date, end_date, n, x, adx_threshold);
    strat->runStrategy();
    strat->writeToFiles();
  }
  else if(strategy == "PAIRS" && argc==9)
  {
    std::string symbol1 = argv[2];
    std::string symbol2 = argv[3];
    int n = std::stoi(argv[4]);
    int x = std::stoi(argv[5]);
    double threshold = std::stod(argv[8]);
    std::string start_date = argv[6], end_date = argv[7];
    Strategy *strat = (Strategy *) new PAIRS(symbol1, symbol2, start_date, end_date, n, x, threshold);
    strat->runStrategy();
    strat->writeTo2Files();
  }
  else if(strategy == "PAIRS" && argc==10){
    std::string symbol1 = argv[2];
    std::string symbol2 = argv[3];
    int n = std::stoi(argv[4]);
    int x = std::stoi(argv[5]);
    double threshold = std::stod(argv[8]);
    double stop_loss_threshold = std::stod(argv[9]);
    std::string start_date = argv[6], end_date = argv[7];
    Strategy *strat = (Strategy *) new PAIRS2(symbol1, symbol2, start_date, end_date, n, x, threshold, stop_loss_threshold);
    strat->runStrategy();
    strat->writeTo2Files();
  }
  else if(strategy == "BEST_OF_ALL")
  {
    if (argc < 5){
      std::cerr << "Usage: " << argv[0] << " strategy symbol start_date end_date";
      return 1;
    }
    std::string symbol = argv[2];
    std::string start_date = argv[3], end_date = argv[4];

    BestOfAll strat(symbol, start_date, end_date);
    strat.runStrategy();
  }
  else if(strategy == "TSI")
  {
   if (argc < 6) {
      std::cerr << "Usage: " << argv[0] << " strategy symbol x \
        start_date end_date";
      return 1;
    }
    std::string symbol = argv[2];
    int x = std::stoi(argv[3]);
    std::string start_date = argv[4], end_date = argv[5];
    Strategy *strat = (Strategy *) new TSI(symbol, start_date, end_date, x);
    strat->runStrategy();
    strat->writeToFiles(); 
  }
  else if(strategy == "FisherT")
  {
    if(argc < 7){
      std::cerr << "Usage: " << argv[0] << " strategy symbol x window_size start_date end_date";
      return 1;
    }
    std::string symbol = argv[2];
    int x = std::stoi(argv[3]), window_size = std::stoi(argv[4]);
    std::string start_date = argv[5], end_date = argv[6];

    FisherTransform strat(symbol, start_date, end_date, x, window_size);
    strat.runStrategy();
    strat.writeToFiles();
  }

  return 0;
}
