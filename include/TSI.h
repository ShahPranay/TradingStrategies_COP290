#ifndef TSI_H
#define TSI_H

#include "Strategy.h"

class TSI : public Strategy
{
  public:
    TSI(std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int x);

    virtual void runStrategy();

  private:
    std::string _symbol, _start_date, _end_date;
    int _x;
    std::vector<StockData> _stock_data;
};

#endif