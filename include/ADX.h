#ifndef ADX_H
#define ADX_H

#include "Strategy.h"

class ADX : public Strategy
{
  public:
    ADX(std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x,
        double adx_threshold);
        
    virtual void runStrategy();

  private:
    std::string _symbol, _start_date, _end_date;
    int _n, _x;
    double _adx_threshold;
    std::vector<StockData> _stock_data;
};

#endif