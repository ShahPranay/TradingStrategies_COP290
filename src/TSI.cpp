#include "TSI.h"
#include <iostream>
#include <fstream>
#include <cmath>

TSI::TSI (std::string symbol, 
        std::string start_date, 
        std::string end_date, 
        int x) : 

  Strategy(),
  _symbol(symbol),
  _start_date(start_date),
  _end_date(end_date),
  _x(x)
{
  _stock_data = fetchStockData(_symbol, _start_date, _end_date, 2, "TSI");
}

void TSI::runStrategy()
{
    int position = 0;
    double PC = 0.0;
    double PCS = 0.0;
    double PCDS = 0.0;
    double APC = 0.0;
    double APCS = 0.0;
    double APCDS = 0.0;
    double TSI = 0.0;
    double TSI_prev = 0.0;
    double signal = 0.0;
    double signal_prev = 0.0;
    double pnl = 0.0;

    std::vector<double> price;
    std::vector<double> TSI_values;
    std::vector<double> signal_values;

    for(int i = 2; i < _stock_data.size(); i++){
        price.push_back(_stock_data[i].close);
        if(i==2){
            PC = _stock_data[i].close - _stock_data[i-1].close;
            PCS = PC;
            PCDS = PCS;
            APC = abs(_stock_data[i].close - _stock_data[i-1].close);
            APCS = APC;
            APCDS = APCS;
            TSI = (PCDS/APCDS) * 100;
            signal = TSI;
        }
        else{
            PC = _stock_data[i].close - _stock_data[i-1].close;
            PCS = ((2*(PC - PCS))/(26)) + PCS;
            PCDS = ((2*(PCS - PCDS))/(14)) + PCDS;
            APC = abs(_stock_data[i].close - _stock_data[i-1].close);
            APCS = ((2*(APC - APCS))/(26)) + APCS;
            APCDS = ((2*(APCS - APCDS))/(14)) + APCDS;
            TSI_prev = TSI;
            TSI = (PCDS/APCDS) * 100;
            signal_prev = signal;
            signal = ((2*(TSI - signal))/(10)) + signal;    

            if(TSI > signal && TSI_prev < signal_prev && position < _x){
                writeOrderStats(_stock_data[i], true, 1);
                pnl -= _stock_data[i].close;
                position++;
            }
            else if(TSI < signal && TSI_prev > signal_prev && position > -_x){
                writeOrderStats(_stock_data[i], false, 1);
                pnl += _stock_data[i].close;
                position--;
            }
        }
        TSI_values.push_back(TSI);
        signal_values.push_back(signal);
        writeDailyCashFlow(_stock_data[i].date, pnl);
    }

    pnl = pnl + position*_stock_data[_stock_data.size()-1].close;
    _final_pnl = pnl;

    // createPlot(price, TSI_values, signal_values, "Price", "TSI", "Signal", "TSI");
}
