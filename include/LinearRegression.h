#ifndef LINEAR_REGRESSION_H
#define LINEAR_REGRESSION_H

#include "Matrix.h"
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
    static Matrix<double> stockDataToMat(const StockData &prevday_stk, const StockData &today_stk);
    static double percentDifference(double prediction, double actual_price);
    void buyStock(const StockData& today_stk);
    void sellStock(const StockData& today_stk);

  private:
    std::string _symbol, _train_start_date, _train_end_date, _test_start_date, _test_end_date;
    double _percent_diff;
    int _max_position, _cur_position;
    std::vector<StockData> _train_stock_data, _stock_data;
};

#endif
