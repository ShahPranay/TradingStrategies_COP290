#include "AMA.h"
#include <iostream>
#include <queue>

AMA::AMA (std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x,
        int p,
        int max_hold_days,
        double c1, 
        double c2) : 

  Strategy(),
  _symbol(symbol),
  _start_date(start_date),
  _end_date(end_date),
  _n(n),
  _x(x),
  _p(p),
  _max_hold_days(max_hold_days),
  _c1(c1),
  _c2(c2)
{
  _stock_data = fetchStockData(_symbol, _start_date, _end_date, _n);
}

void AMA::runStrategy()
{
    int position = 0;
    double volatility = 0.0;
    double SF = 0.5;
    double AMA = 0.0;
    double pnl = 0.0;
    std::queue<int> buy_queue;
    std::queue<int> sell_queue;

    for(int i =1; i<_stock_data.size(); i++){
        if(i<_n){
            volatility += abs(_stock_data[i].close - _stock_data[i-1].close);
        }
        else{
            volatility += abs(_stock_data[i].close - _stock_data[i-1].close);
            double direction = abs(_stock_data[i].close - _stock_data[i-_n].close);
            double ER = 0;
            int hold_flag = 0;
            int AMA_flag = 0;

            if(volatility!=0){
                ER = direction/volatility;
            }
            else{
                if(!buy_queue.empty() && i-buy_queue.front()>_max_hold_days){ 
                    writeOrderStats(_stock_data[i], false, 1);
                    pnl+=_stock_data[i].close;
                    position--;
                    buy_queue.pop();
                }
                if(!sell_queue.empty() && i-sell_queue.front()>_max_hold_days){
                    writeOrderStats(_stock_data[i], true, 1);
                    pnl-=_stock_data[i].close;
                    position++;
                    sell_queue.pop();
                }
                continue;
            }
            if(i>_n){
                double c = (2*ER)/(1+_c2);
                SF = SF*(1-_c1) + ((_c1*(c-1))/(c+1));
            }

            if(i==_n){
                AMA = _stock_data[i].close;
            }
            else{
                AMA = AMA+ SF*(_stock_data[i].close - AMA);
            }

            if(_stock_data[i].close > AMA*(1+(_p/100)) && position < _x){
                AMA_flag = 1;
            }
            else if(_stock_data[i].close < AMA*(1-(_p/100)) && position > -_x){
                AMA_flag = -1;
            }
            if(!buy_queue.empty() && i-buy_queue.front()>_max_hold_days){ 
                hold_flag = -1;
            }
            if(!sell_queue.empty() && i-sell_queue.front()>_max_hold_days){
                hold_flag = 1;
            }
            
            if(AMA_flag != 0 || hold_flag != 0){
                if(AMA_flag == 0){
                    if(hold_flag == 1){
                        writeOrderStats(_stock_data[i], true, 1);
                        pnl-=_stock_data[i].close;
                        position++;
                        sell_queue.pop();
                    }
                    else if(hold_flag == -1){
                        writeOrderStats(_stock_data[i], false, 1);
                        pnl+=_stock_data[i].close;
                        position--;
                        buy_queue.pop();
                    }
                }
                else if(hold_flag==0){
                    if(AMA_flag == 1){
                        writeOrderStats(_stock_data[i], true, 1);
                        pnl-=_stock_data[i].close;
                        position++;
                        if(!sell_queue.empty()){
                            sell_queue.pop();
                        }
                        else{
                            buy_queue.push(i);
                        }
                    }
                    else if(AMA_flag == -1){
                        writeOrderStats(_stock_data[i], false, 1);
                        pnl+=_stock_data[i].close;
                        position--;
                        if(!buy_queue.empty()){
                            buy_queue.pop();
                        }
                        else{
                            sell_queue.push(i);
                        }
                    }
                }
                else if(hold_flag == AMA_flag){
                    if(hold_flag == 1){
                        writeOrderStats(_stock_data[i], true, 1);
                        pnl-=_stock_data[i].close;
                        position++;
                        sell_queue.pop();
                    }
                    else if(hold_flag == -1){
                        writeOrderStats(_stock_data[i], false, 1);
                        pnl+=_stock_data[i].close;
                        position--;
                        buy_queue.pop();
                    }
                }
                else if(hold_flag != AMA_flag){
                    if(hold_flag == 1){
                        sell_queue.pop();
                        sell_queue.push(i);
                    }
                    else if(hold_flag == -1){
                        buy_queue.pop();
                        buy_queue.push(i);
                    }
                }
            }
            writeDailyCashFlow(_stock_data[i].date, pnl);
            volatility -= abs(_stock_data[i-_n+1].close - _stock_data[i-_n].close);
        }
    }

    pnl = pnl + position*_stock_data[_stock_data.size()-1].close;
    _final_pnl = pnl;
}
