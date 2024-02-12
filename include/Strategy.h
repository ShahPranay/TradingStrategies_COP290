#ifndef STRATEGY_H
#define STRATEGY_H

#include <string>
#include <vector>
#include <sstream>

struct StockData {
  std::string date;
  double open;
  double close;
  double high;
  double low;
  double ltp;
  double volume;
  double value;
  double prevClose;
  int numTrades;
};

class Strategy
{
  public:
    Strategy(std::string symbol);
    /* ~Strategy() {  }; */

    std::string convertDateFormat(const std::string& str);
    void fetchStockData(std::string start_date, std::string end_date, int n); // rename n with a meaningfull name
                                                                              //
    void writeOrderStats(std::string date, bool is_buy, int quantity, int price);
    void writeOrderStats(const StockData &stk, bool is_buy, int quantity);
    void writeDailyCashFlow(std::string date, double cashflow);

    void writeToPNLFile(double pnl); 
    void writeToFiles();

    virtual void runStrategy() = 0; // calculates the _final_pnl and stores it.

  protected:
    std::string _symbol;
    std::ostringstream _daily_cashflow, _order_statistics;
    std::vector<StockData> _stock_data;
    double _final_pnl;
};
#endif
