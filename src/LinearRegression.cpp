#include "LinearRegression.h"
#include <iostream>

LinearRegression::LinearRegression (std::string symbol, 
    std::string train_start_date, 
    std::string train_end_date, 
    std::string start_date,
    std::string end_date,
    double max_position,
    double percent_diff) : 

  Strategy(),
  _symbol(symbol),
  _train_start_date(train_start_date),
  _train_end_date(train_end_date),
  _test_start_date(start_date),
  _test_end_date(end_date),
  _max_position(max_position),
  _percent_diff(percent_diff)
{
  _stock_data = fetchStockData(_symbol, _train_start_date, _train_end_date, 0);
}

void LinearRegression::runStrategy()
{
  _final_pnl = 0;
  std::cout << "Hello\n";
}
