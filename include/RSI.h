#ifndef RSI_H
#define RSI_H

#include "Strategy.h"

class RSI : public Strategy
{
  public:
    RSI(std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x,
        double oversold_threshold, 
        double overbought_threshold);
        
    virtual void runStrategy();

  private:
    std::string _start_date, _end_date;
    int _n, _x;
    double _oversold_threshold, _overbought_threshold;
};

#endif