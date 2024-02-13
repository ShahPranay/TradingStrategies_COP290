#include "PAIRS2.h"
#include <iostream>
#include <cmath>
#include <set>
#include <vector>
#include <utility>

PAIRS2::PAIRS2 (std::string symbol1,
        std::string symbol2, 
        std::string start_date, 
        std::string end_date, 
        int n,
        int x,
        double threshold,
        double stop_loss_threshold) : 

  Strategy(),
  _symbol1(symbol1),
  _symbol2(symbol2),
  _start_date(start_date),
  _end_date(end_date),
  _n(n),
  _x(x),
  _threshold(threshold),
  _stop_loss_threshold(stop_loss_threshold)
{
  _stock_data1 = fetchStockData(_symbol1, _start_date, _end_date, _n-1, "data1");
  _stock_data2 = fetchStockData(_symbol2, _start_date, _end_date, _n-1, "data2");
}

void PAIRS2::runStrategy()
{
    int position = 0;
    double spread = 0.0;
    double rolling_sum = 0.0;
    double rolling_square_sum = 0.0;
    double pnl = 0.0;
    std::set<std::pair<int, std::pair<double,double>>> buy_orders;
    std::set<std::pair<int, std::pair<double,double>>> sell_orders;

    std::vector<double> spread_values;
    std::vector<double> z_score_values;
    for(int i = 0; i < _stock_data1.size(); i++){
        if(i<_n-1){
            spread = _stock_data1[i].close - _stock_data2[i].close;
            rolling_sum += spread;
            rolling_square_sum += spread*spread;
        }
        else{
            spread = _stock_data1[i].close - _stock_data2[i].close;
            spread_values.push_back(spread);
            rolling_sum += spread;
            rolling_square_sum += spread*spread;
            double rolling_mean = rolling_sum/_n;
            double rolling_variance = (rolling_square_sum - (rolling_sum*rolling_sum)/_n)/_n;
            double rolling_std_dev = sqrt(rolling_variance);
            double z_score = (spread - rolling_mean)/rolling_std_dev;
            z_score_values.push_back(z_score*100);
            int curr_position = 0;
            int flag = 0;
            if( z_score > _threshold && z_score < _stop_loss_threshold && position > -_x){ 
                curr_position = -1;
                if(buy_orders.size()>0){
                    auto it = buy_orders.begin();
                    buy_orders.erase(it);
                    flag =1;
                    std::vector<std::pair<int,std::pair<double,double>>> to_remove;
                    for(auto order: buy_orders){
                        if((spread - order.second.first) < -order.second.second*_stop_loss_threshold){
                            curr_position--;
                            to_remove.push_back(order);
                        }
                    }
                    for(auto order: to_remove){
                        buy_orders.erase(order);
                    }
                    to_remove.clear();
                }
                else if(sell_orders.size()>0){
                    std::vector<std::pair<int,std::pair<double,double>>> to_remove;
                    for(auto order: sell_orders){
                        if((spread - order.second.first) > order.second.second*_stop_loss_threshold){
                            curr_position++;
                            to_remove.push_back(order);
                        }
                    }
                    for(auto order: to_remove){
                        sell_orders.erase(order);
                    }
                    to_remove.clear();
                }

                if(curr_position < 0){
                    writeOrderStats(_stock_data1[i], false, -curr_position);
                    writeOrderStats2(_stock_data2[i], true, -curr_position);
                    pnl -= curr_position*spread;
                    position += curr_position;
                    
                }
                else if(curr_position>0){
                    writeOrderStats(_stock_data1[i], true, curr_position);
                    writeOrderStats2(_stock_data2[i], false, curr_position);
                    pnl -= curr_position*spread;
                    position += curr_position;
                }
                if(flag==0){
                    sell_orders.insert(std::make_pair(i, std::make_pair(rolling_mean , rolling_std_dev)));
                }
                
            }
            else if( z_score < -_threshold && z_score > -_stop_loss_threshold && position < _x){
                curr_position = 1;
                if(sell_orders.size()>0){
                    auto it = sell_orders.begin();
                    sell_orders.erase(it);
                    flag = 1;
                    std::vector<std::pair<int,std::pair<double,double>>> to_remove;
                    for(auto order: sell_orders){
                        if((spread - order.second.first) > order.second.second*_stop_loss_threshold){
                            curr_position++;
                            to_remove.push_back(order);
                        }
                    }
                    for(auto order: to_remove){
                        sell_orders.erase(order);
                    }
                    to_remove.clear();
                }
                else if(buy_orders.size()>0){
                    std::vector<std::pair<int,std::pair<double,double>>> to_remove;
                    for(auto order: buy_orders){
                        if((spread - order.second.first) < order.second.second*_stop_loss_threshold){
                            curr_position--;
                            to_remove.push_back(order);
                        }
                    }
                    for(auto order: to_remove){
                        buy_orders.erase(order);
                    }
                    to_remove.clear();
                }
                if(curr_position < 0){
                    writeOrderStats(_stock_data1[i], false, -curr_position);
                    writeOrderStats2(_stock_data2[i], true, -curr_position);
                    pnl -= curr_position*spread;
                    position += curr_position;
                }
                else if(curr_position>0){
                    writeOrderStats(_stock_data1[i], true, curr_position);
                    writeOrderStats2(_stock_data2[i], false, curr_position);
                    pnl -= curr_position*spread;
                    position += curr_position;
                }                    
                if(flag==0){
                    buy_orders.insert(std::make_pair(i, std::make_pair(rolling_mean , rolling_std_dev)));
                }
            }
            else{
                if(buy_orders.size()>0){
                    std::vector<std::pair<int,std::pair<double,double>>> to_remove;
                    for(auto order: buy_orders){
                        if((spread - order.second.first) < -order.second.second*_stop_loss_threshold){
                            curr_position--;
                            to_remove.push_back(order);
                        }
                    }
                    for(auto order: to_remove){
                        buy_orders.erase(order);
                    }
                    to_remove.clear();
                    if(curr_position < 0){
                        writeOrderStats(_stock_data1[i], false, -curr_position);
                        writeOrderStats2(_stock_data2[i], true, -curr_position);
                        pnl -= curr_position*spread;
                        position += curr_position;
                    }
                }
                else if(sell_orders.size()>0){
                    std::vector<std::pair<int,std::pair<double,double>>> to_remove;
                    for(auto order: sell_orders){
                        if((spread - order.second.first) > order.second.second*_stop_loss_threshold){
                            curr_position++;
                            to_remove.push_back(order);
                        }
                    }
                    for(auto order: to_remove){
                        sell_orders.erase(order);
                    }
                    to_remove.clear();
                    if(curr_position > 0){
                        writeOrderStats(_stock_data1[i], true, curr_position);
                        writeOrderStats2(_stock_data2[i], false, curr_position);
                        pnl -= curr_position*spread;
                        position += curr_position;
                    }
                }
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

    // createPlot2(spread_values, z_score_values, "Spread", "Z-Score*100", "Pairs2");
}
