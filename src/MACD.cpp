#include "MACD.h"
#include <iostream>

MACD::MACD (std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int x) : 

  Strategy(),
  _symbol(symbol),
  _start_date(start_date),
  _end_date(end_date),
  _x(x)
{
  _stock_data = fetchStockData(_symbol, _start_date, _end_date, 0);
}

void MACD::runStrategy()
{
    int position = 0;
    double short_EWM = 0.0;
    double long_EWM = 0.0;
    double MACD = 0.0;
    double signal = 0.0;
    double pnl = 0.0;

    for(int i = 0; i < _stock_data.size(); i++){
        if(i==0){
            short_EWM = _stock_data[i].close;
            long_EWM = _stock_data[i].close;
        }
        else{
            short_EWM = ((2*(_stock_data[i].close - short_EWM))/(13)) + short_EWM;
            long_EWM = ((2*(_stock_data[i].close - long_EWM))/(27)) + long_EWM;
            MACD = short_EWM - long_EWM;
            signal = ((2*(MACD - signal))/(10)) + signal;
            if(MACD > signal && position < _x){
                writeOrderStats(_stock_data[i], true, 1);
                pnl -= _stock_data[i].close;
                position++;
            }
            else if(MACD < signal && position > -_x){
                writeOrderStats(_stock_data[i], false, 1);
                pnl += _stock_data[i].close;
                position--;
            }
        }
        writeDailyCashFlow(_stock_data[i].date, pnl);
    }

    pnl = pnl + position*_stock_data[_stock_data.size()-1].close;
    _final_pnl = pnl;
}
