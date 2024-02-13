#include "Basic.h"
#include <iostream>

Basic::Basic (std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x) : 

  Strategy(),
  _symbol(symbol),
  _start_date(start_date),
  _end_date(end_date),
  _n(n),
  _x(x)
{
  _stock_data = fetchStockData(_symbol, _start_date, _end_date, _n, "Basic");
}

void Basic::runStrategy()
{
    int position = 0;
    int dir_count = 0;
    double pnl = 0.0;
    
    for(int i = 1; i < _stock_data.size(); i++){
        if(i<_n){
            dir_count += (_stock_data[i].close - _stock_data[i-1].close > 0 ? 1 : -1);
        }
        else{
            dir_count += (_stock_data[i].close - _stock_data[i-1].close > 0 ? 1 : -1);
            if(dir_count ==_n && position <_x){
                writeOrderStats(_stock_data[i], true, 1);
                pnl -= _stock_data[i].close;
                position++;
            }
            else if(dir_count == -_n && position > -_x){
                writeOrderStats(_stock_data[i], false, 1);
                pnl += _stock_data[i].close;
                position--;
            }
            dir_count -= (_stock_data[i-_n+1].close - _stock_data[i-_n].close > 0 ? 1 : -1);
            writeDailyCashFlow(_stock_data[i].date, pnl);
        }
    }
    pnl = pnl + position*_stock_data[_stock_data.size()-1].close;
    _final_pnl = pnl;
}
