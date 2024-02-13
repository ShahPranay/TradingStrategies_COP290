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
  double vwap;
  int numTrades;
};

class Strategy
{
  public:
    Strategy();
    /* ~Strategy() {  }; */

    static std::string convertDateFormat(const std::string& str);
    static std::vector<StockData> fetchStockData(std::string symbol, std::string start_date, std::string end_date, int n, std::string filename); // rename n with a meaningfull name
                                                                              //
    void createPlot(std::vector<double> &data1, std::vector<double> &data2, std::vector<double> &data3, 
                          std::string label1, std::string label2, std::string label3, std::string filename);
    void createPlot2(std::vector<double> &data1, std::vector<double> &data2, std::string label1, std::string label2, std::string filename);
    void writeOrderStats(std::string date, bool is_buy, int quantity, int price);
    void writeOrderStats(const StockData &stk, bool is_buy, int quantity);
    void writeOrderStats2(std::string date, bool is_buy, int quantity, int price);
    void writeOrderStats2(const StockData &stk, bool is_buy, int quantity);
    void writeDailyCashFlow(std::string date, double cashflow);

    void writeToPNLFile(double pnl); 
    void writeToFiles();
    void writeTo2Files();

    double getFinalPnl();

    virtual void runStrategy() = 0; // calculates the _final_pnl and stores it.

  protected:
    std::ostringstream _daily_cashflow, _order_statistics, _order_statistics_2;
    double _final_pnl;
};
#endif
