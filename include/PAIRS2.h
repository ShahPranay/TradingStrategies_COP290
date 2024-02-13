#ifndef PAIRS2_H
#define PAIRS2_H

#include "Strategy.h"

class PAIRS2 : public Strategy
{
  public:
    PAIRS2(std::string symbol1,
        std::string symbol2, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x,
        double threshold,
        double stop_loss_threshold);
        
    virtual void runStrategy();

  private:
    std::string _symbol1, _symbol2, _start_date, _end_date;
    int _n, _x;
    double _threshold, _stop_loss_threshold;
    std::vector<StockData> _stock_data1;
    std::vector<StockData> _stock_data2;
};

#endif