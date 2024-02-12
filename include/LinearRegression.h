#include "Strategy.h"

class LinearRegression : public Strategy
{
  public:
    LinearRegression(std::string symbol, 
        std::string train_start_date, 
        std::string train_end_date, 
        std::string start_date,
        std::string end_date,
        double max_position,
        double percent_diff);

    virtual void runStrategy();

  private:
    std::string _train_start_date, _train_end_date, _test_start_date, _test_end_date;
    double _max_position, _percent_diff;
};

