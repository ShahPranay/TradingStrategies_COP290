#include "ADX.h"
#include <iostream>

ADX::ADX (std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x,
        double adx_threshold) : 

  Strategy(),
  _symbol(symbol),
  _start_date(start_date),
  _end_date(end_date),
  _n(n),
  _x(x),
  _adx_threshold(adx_threshold)
{
  _stock_data = fetchStockData(_symbol, _start_date, _end_date, 2, "ADX");
}

void ADX::runStrategy()
{
    int position = 0;
    double TR = 0.0;
    double DM_plus = 0.0;
    double DM_minus = 0.0;
    double ATR = 0.0;
    double DI_plus = 0.0;
    double DI_minus = 0.0;
    double DX = 0.0;
    double ADX = 0.0;
    double pnl = 0.0;

    for(int i =2; i<_stock_data.size(); i++){
        TR = std::max(_stock_data[i].high - _stock_data[i].low, std::max(_stock_data[i].high - _stock_data[i-1].close, _stock_data[i-1].close - _stock_data[i].low));
        DM_plus = std::max(0.0, _stock_data[i].high - _stock_data[i-1].high);
        DM_minus = std::max(0.0, _stock_data[i].low - _stock_data[i-1].low);
        if(i==2){
            ATR = TR;
            DI_plus = DM_plus/ATR;
            DI_minus = DM_minus/ATR;
            double num = (DI_plus - DI_minus);
            double den = DI_plus + DI_minus;
            if(den==0){
                DX = 0;
            }
            else{
                DX = (num/den)*100;
            }
            ADX = DX;
        }
        else{
            ATR = ((2*(TR - ATR))/(_n+1)) + ATR;
            DI_plus = ((2*((DM_plus/ATR) - DI_plus))/(_n+1)) + DI_plus;
            DI_minus = ((2*((DM_minus/ATR) - DI_minus))/(_n+1)) + DI_minus;
            double num = (DI_plus - DI_minus);
            double den = DI_plus + DI_minus;
            if(den==0){
                DX = 0;
            }
            else{
                DX = (num/den)*100;
            }
            ADX = ((2*(DX - ADX))/(_n+1)) + ADX;
        }

        if(ADX > _adx_threshold && position <_x){
            writeOrderStats(_stock_data[i], true, 1);
            pnl -= _stock_data[i].close;
            position++;
        }
        else if(ADX < _adx_threshold && position > -_x){
            writeOrderStats(_stock_data[i], false, 1);
            pnl += _stock_data[i].close;
            position--;
        }
        writeDailyCashFlow(_stock_data[i].date, pnl);
    }

    pnl = pnl + position*_stock_data[_stock_data.size()-1].close;
    _final_pnl = pnl;
}
