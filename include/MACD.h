#ifndef MACD_H
#define MACD_H

#include "Strategy.h"

class MACD : public Strategy
{
  public:
    MACD(std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int x);
        
    virtual void runStrategy();

  private:
    std::string _start_date, _end_date;
    int _x;
};

#endif