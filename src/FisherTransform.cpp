#include "FisherTransform.h"
#include <cmath>
#include <iostream>

FisherTransform::FisherTransform(std::string symbol, std::string start_date, std::string end_date, int max_position, int window_size):
  Strategy(),
  _max_position(max_position),
  _cur_position(0),
  _ma_window_size(window_size),
  _stock_data(fetchStockData(symbol, start_date, end_date, 0, "FisherT")),
  _cur_ma(0.0)
{
}

bool FisherTransform::isPositiveCrossover()
{
  if(_fisher_values.size() < _ma_window_size)
    return false;
  if(_fisher_values.front() > _cur_ma && !_is_above_ma)
    return true;
  else return false;
}

bool FisherTransform::isNegativeCrossover()
{
  if(_fisher_values.size() < _ma_window_size)
    return false;
  if(_fisher_values.front() < _cur_ma && _is_above_ma)
    return true;
  else return false;
}

void FisherTransform::addFisherTransform(const StockData &data)
{
  const double MAX_VALUE = 0.99999;
  const double MIN_VALUE = -0.99999;

  // Calculate X value using the close price of the day
  double X = 0.5 * std::log((1 + (data.close - data.low) / (1 + (data.high - data.close))));

  // Apply Fisher Transform formula
  double value = 0.5 * std::log((1 + X) / (1 - X));
  value = std::max(std::min(value, MAX_VALUE), MIN_VALUE);

  std::cout << value << std::endl;

  _cur_ma = _cur_ma + ((value - _cur_ma) / _fisher_values.size());
  _fisher_values.push(value);
  
  if(_fisher_values.size() > _ma_window_size){
    double sval = _fisher_values.back();
    _cur_ma = ((_cur_ma * _fisher_values.size()) - sval) / (_fisher_values.size() - 1);
    _fisher_values.pop();
  }
}

void FisherTransform::buyStock(const StockData &data)
{
  if(_cur_position > _max_position)
    return;
  _cur_position++;
  _final_pnl -= data.close;
  writeOrderStats(data, true, 1);
}

void FisherTransform::sellStock(const StockData &data)
{
  if(_cur_position < -_max_position)
    return;
  _cur_position--;
  _final_pnl += data.close;
  writeOrderStats(data, false, 1);
}

void FisherTransform::runStrategy()
{
  int n_data = _stock_data.size(); 
  for(int i = 0; i < n_data; i++){
    if(isPositiveCrossover())
      buyStock(_stock_data[i]);
    else if(isNegativeCrossover())
      sellStock(_stock_data[i]);

    if(_fisher_values.size() > 0 && _fisher_values.back() >= _cur_ma)
      _is_above_ma = true;
    else _is_above_ma = false;

    addFisherTransform(_stock_data[i]);
    writeDailyCashFlow(_stock_data[i].date, _final_pnl);
  }

  _final_pnl += _cur_position * _stock_data.back().close;
}
