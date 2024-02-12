#ifndef DMA_H
#define DMA_H
#include "Strategy.h"

class DMA : public Strategy
{
  public:
    DMA(std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x,
        int p);
        
    virtual void runStrategy();

  private:
    std::string _symbol, _start_date, _end_date;
    int _n, _x, _p;
    std::vector<StockData> _stock_data;
};

#endif
