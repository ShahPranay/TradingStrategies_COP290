#include "RSI.h"
#include <iostream>

RSI::RSI (std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x,
        double oversold_threshold, 
        double overbought_threshold) : 

  Strategy(),
  _symbol(symbol),
  _start_date(start_date),
  _end_date(end_date),
  _n(n),
  _x(x),
  _oversold_threshold(oversold_threshold),
  _overbought_threshold(overbought_threshold)
{
  _stock_data = fetchStockData(_symbol, _start_date, _end_date, _n);
}

void RSI::runStrategy()
{
    int position = 0;
    double gain_sum = 0.0;
    double loss_sum = 0.0;
    double avg_gain = 0.0;
    double avg_loss = 0.0;
    double pnl = 0.0;

    for(int i = 1; i < _stock_data.size(); i++){
        if(i<_n){
            gain_sum += std::max(_stock_data[i].close-_stock_data[i-1].close, 0.0);
            loss_sum -= std::min(_stock_data[i].close-_stock_data[i-1].close, 0.0);
        }
        else{
            gain_sum += std::max(_stock_data[i].close-_stock_data[i-1].close, 0.0);
            loss_sum -= std::min(_stock_data[i].close-_stock_data[i-1].close, 0.0);
            avg_gain = gain_sum/_n;
            avg_loss = loss_sum/_n;
            double RS = avg_gain/avg_loss;
            double RSI = 100 - (100/(1+RS));

            if( RSI < _oversold_threshold && position <_x){
                writeOrderStats(_stock_data[i], true, 1);
                pnl -= _stock_data[i].close;
                position++;
            }
            else if( RSI > _overbought_threshold && position > -_x){
                writeOrderStats(_stock_data[i], false, 1);
                pnl += _stock_data[i].close;
                position--;
            }
            gain_sum -= std::max(_stock_data[i-_n+1].close-_stock_data[i-_n].close, 0.0);
            loss_sum += std::min(_stock_data[i-_n+1].close-_stock_data[i-_n].close, 0.0);
            writeDailyCashFlow(_stock_data[i].date, pnl);
        }
    }
    
    pnl = pnl + position*_stock_data[_stock_data.size()-1].close;
    _final_pnl = pnl;
}
