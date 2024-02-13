#include "PAIRS.h"
#include <iostream>
#include <cmath>

PAIRS::PAIRS (std::string symbol1,
        std::string symbol2, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x,
        double threshold) : 

  Strategy(),
  _symbol1(symbol1),
  _symbol2(symbol2),
  _start_date(start_date),
  _end_date(end_date),
  _n(n),
  _x(x),
  _threshold(threshold)
{
  _stock_data1 = fetchStockData(_symbol1, _start_date, _end_date, _n-1, "Pairs_1");
  _stock_data2 = fetchStockData(_symbol2, _start_date, _end_date, _n-1, "Pairs_2");
}

void PAIRS::runStrategy()
{
    int position = 0;
    double spread = 0.0;
    double rolling_sum = 0.0;
    double rolling_square_sum = 0.0;
    double pnl = 0.0;

    std::vector<double> spread_values;
    std::vector<double> z_score_values;

    for(int i = 0; i < _stock_data1.size(); i++){
        if(i<_n-1){
            spread = _stock_data1[i].close - _stock_data2[i].close;
            rolling_sum += spread;
            rolling_square_sum += spread*spread;
        }
        else{
            spread_values.push_back(_stock_data1[i].close - _stock_data2[i].close);
            spread = _stock_data1[i].close - _stock_data2[i].close;
            rolling_sum += spread;
            rolling_square_sum += spread*spread;
            double rolling_mean = rolling_sum/_n;
            double rolling_variance = (rolling_square_sum - (rolling_sum*rolling_sum)/_n)/_n;
            double rolling_std_dev = sqrt(rolling_variance);
            double z_score = (spread - rolling_mean)/rolling_std_dev;
            z_score_values.push_back(z_score*100);
            if( z_score > _threshold && position > -_x){
                writeOrderStats(_stock_data1[i], false, 1);
                writeOrderStats2(_stock_data2[i], true, 1);
                pnl += spread;
                position--;
            }
            else if( z_score < -_threshold && position < _x){
                writeOrderStats(_stock_data1[i], true, 1);
                writeOrderStats2(_stock_data2[i], false, 1);
                pnl -= spread;
                position++;
            }
            double spread_to_remove = _stock_data1[i-_n+1].close - _stock_data2[i-_n+1].close;
            rolling_sum -= spread_to_remove;
            rolling_square_sum -= spread_to_remove*spread_to_remove;
            writeDailyCashFlow(_stock_data1[i].date, pnl);
        }
    }
    spread = _stock_data1[_stock_data1.size()-1].close - _stock_data2[_stock_data2.size()-1].close;
    pnl = pnl + position*spread;
    _final_pnl = pnl;

    createPlot2(spread_values, z_score_values, "Spread", "Z-Score*100", "Pairs");
}
