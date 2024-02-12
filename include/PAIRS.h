#ifndef PAIRS_H
#define PAIRS_H

#include "Strategy.h"

class PAIRS : public Strategy
{
  public:
    PAIRS(std::string symbol1,
        std::string symbol2, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x,
        double threshold);
        
    virtual void runStrategy();

  private:
    std::string _symbol1, _symbol2, _start_date, _end_date;
    int _n, _x;
    double _threshold;
    std::vector<StockData> _stock_data1;
    std::vector<StockData> _stock_data2;
};

#endif