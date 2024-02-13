#include "DMA.h"
#include <iostream>
#include <cmath>

DMA::DMA (std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x,
        int p) : 

  Strategy(),
  _symbol(symbol),
  _start_date(start_date),
  _end_date(end_date),
  _n(n),
  _x(x),
  _p(p)
{
  _stock_data = fetchStockData(_symbol, _start_date, _end_date, _n-1, "DMA");
}

void DMA::runStrategy()
{
    int position = 0;
    double price_sum = 0.0;
    double square_price_sum = 0.0;
    double pnl = 0.0;

    std::vector<double> price;
    std::vector<double> lower_bound;
    std::vector<double> upper_bound;

    for(int i = 0; i < _stock_data.size(); i++){
        if(i<_n-1){
            price_sum += _stock_data[i].close;
            square_price_sum += _stock_data[i].close*_stock_data[i].close;
        }
        else{
            price.push_back(_stock_data[i].close);
            price_sum += _stock_data[i].close;
            square_price_sum += _stock_data[i].close*_stock_data[i].close;
            double mean = price_sum/_n;
            double variance = (square_price_sum - (price_sum*price_sum)/_n)/_n;
            double std_dev = sqrt(variance);
            lower_bound.push_back(mean - _p*std_dev);
            upper_bound.push_back(mean + _p*std_dev);
            if( _stock_data[i].close >= mean + _p*std_dev && position <_x){
                writeOrderStats(_stock_data[i], true, 1);
                pnl -= _stock_data[i].close;
                position++;
            }
            else if( _stock_data[i].close <= mean - _p*std_dev && position > -_x){
                writeOrderStats(_stock_data[i], false, 1);
                pnl += _stock_data[i].close;
                position--;
            }
            price_sum -= _stock_data[i-_n+1].close;
            square_price_sum -= _stock_data[i-_n+1].close*_stock_data[i-_n+1].close;
            writeDailyCashFlow(_stock_data[i].date, pnl);
        }
    }
    
    pnl = pnl + position*_stock_data[_stock_data.size()-1].close;
    _final_pnl = pnl;

    // createPlot(price, lower_bound, upper_bound, "Price", "Mean - p*std. dev", "Mean + p*std. dev", "DMA");
}
