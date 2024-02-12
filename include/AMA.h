#ifndef AMA_H
#define AMA_H
#include "Strategy.h"

class AMA : public Strategy
{
  public:
    AMA(std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x,
        int p,
        int max_hold_days,
        double c1, 
        double c2);
        
    virtual void runStrategy();

  private:
    std::string _start_date, _end_date;
    int _n, _x, _p, _max_hold_days;
    double _c1, _c2;
};

#endif
