#ifndef BASIC_H
#define BASIC_H

#include "Strategy.h"

class Basic : public Strategy
{
  public:
    Basic(std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x);
        
    virtual void runStrategy();

  private:
    std::string _start_date, _end_date;
    int _n, _x;
};

#endif