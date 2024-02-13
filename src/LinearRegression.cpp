#include "LinearRegression.h"
#include "Matrix.h"
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
{ _train_stock_data = fetchStockData(_symbol, _train_start_date, _train_end_date, 0, "LR_train");
  _stock_data = fetchStockData(_symbol, _test_start_date, _test_end_date, 1, "LR_test");
}

Matrix<double> LinearRegression::stockDataToMat(const StockData& prevday_stk, const StockData& today_stk)
{
  Matrix<double> x_vec(1, 8);

  x_vec.set(0, 0, 1);
  x_vec.set(0, 1, prevday_stk.close);
  x_vec.set(0, 2, prevday_stk.open);
  x_vec.set(0, 3, prevday_stk.vwap);
  x_vec.set(0, 4, prevday_stk.low);
  x_vec.set(0, 5, prevday_stk.high);
  x_vec.set(0, 6, prevday_stk.numTrades);
  x_vec.set(0, 7, today_stk.open);

  return x_vec;
}

double LinearRegression::percentDifference(double prediction, double actual_price){
  /* std::cerr << "Prediction: " << prediction << ", Actual: " << actual_price << std::endl; */
  return ((prediction - actual_price) / actual_price) * 100;
}

void LinearRegression::buyStock(const StockData& today_stk){
  if(_cur_position > _max_position)
    return;
  _cur_position++;
  writeOrderStats(today_stk, true, 1);
  _final_pnl -= today_stk.close;
}

void LinearRegression::sellStock(const StockData& today_stk){
  if(_cur_position < -_max_position)
    return;
  _cur_position--;
  writeOrderStats(today_stk, false, 1);
  _final_pnl += today_stk.close;
}

void LinearRegression::runStrategy()
{
  int n_traindata = _train_stock_data.size();
  Matrix<double> X(n_traindata - 1, 8), y(n_traindata - 1, 1);

  for(int i = 0; i < n_traindata - 1; i++){
    X.setRow(i, stockDataToMat(_train_stock_data[i], _train_stock_data[i + 1]));
    y.set(i, 0, _train_stock_data[i + 1].close);
  }

  Matrix<double> X_t = X.transpose();
  Matrix<double> params = (X_t * X).inverse() * (X_t * y);

  std::cout << "Params:\n";
  params.print();
  std::cout << "\n";

  int n_testdata = _stock_data.size();

  _final_pnl = 0.0;
  _cur_position = 0;
  for(int i = 1; i < n_testdata; i++){
    Matrix<double> prediction = stockDataToMat(_stock_data[i - 1], _stock_data[i]) * params;
    std::cout << _stock_data[i].date << ", Prediction: " << prediction.get(0,0) << ", Actual val: " << _stock_data[i].close << std::endl;
    double pdiff = percentDifference(prediction.get(0,0), _stock_data[i].close);

    if(pdiff > 0 && pdiff >= _percent_diff){
      buyStock(_stock_data[i]);
    }
    else if(pdiff < 0 && -pdiff >= _percent_diff){
      sellStock(_stock_data[i]);
    }
    writeDailyCashFlow(_stock_data[i].date, _final_pnl);
  }
  _final_pnl += _cur_position * _stock_data.back().close;
}
