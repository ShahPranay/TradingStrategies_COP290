#include "BestOfAll.h"
#include "ADX.h"
#include "AMA.h"
#include "Basic.h"
#include "DMA.h"
#include "LinearRegression.h"
#include "MACD.h"
#include "RSI.h"

#include <omp.h>
#include <iostream>
#include <iomanip>

std::string BestOfAll::decrementYear(std::string datestr)
{
  std::istringstream iss(datestr);
  char delimiter;
  int day, month, year;

  if (!(iss >> day >> delimiter >> month >> delimiter >> year)) {
    std::cerr << "Invalid date format\n";
    return ""; // Return empty string for invalid input
  }

  // Decrement the year
  year--;

  // Check for invalid date components
  if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31) {
    std::cerr << "Invalid date components\n";
    return ""; // Return empty string for invalid input
  }

  std::ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << day << "/"
    << std::setw(2) << std::setfill('0') << month << "/"
    << std::setw(4) << std::setfill('0') << year;
  return oss.str();
}

BestOfAll::BestOfAll(std::string symbol, std::string start_date, std::string end_date) : 
  _symbol(symbol), _start_date(start_date), _end_date(end_date), _runners(7)
{  
#pragma omp parallel for default(none) shared(_runners) shared(_start_date) shared(_end_date) shared(_symbol)
  for(int i = 0; i < 7; i++){
    switch(i){
      case 0:
        _runners[i] = (Strategy *) new Basic(_symbol, _start_date, _end_date, 7, 5);
        break;
      case 1:
        _runners[i] = (Strategy *) new DMA(_symbol, _start_date, _end_date, 50, 5, 2);
        break;
      case 2:
        _runners[i] = (Strategy *) new AMA(_symbol, _start_date, _end_date, 14, 5, 5, 28, 2, 0.2);
        break;
      case 3:
        _runners[i] = (Strategy *) new MACD(_symbol, _start_date, _end_date, 5);
        break;
      case 4:
        _runners[i] = (Strategy *) new RSI(_symbol, _start_date, _end_date, 14, 5, 30, 70);
        break;
      case 5:
        _runners[i] = (Strategy *) new ADX(_symbol, _start_date, _end_date, 14, 5, 25);
        break;
      case 6:
        std::string train_start_date = decrementYear(_start_date), train_end_date = decrementYear(_end_date);
        _runners[i] = (Strategy *) new LinearRegression(_symbol, train_start_date, train_end_date, _start_date, _end_date, 5, 2);
        break;
    }
  }
}

void BestOfAll::runStrategy()
{
#pragma omp parallel for default(none) shared(_runners)
  for(int i = 0; i < 7; i++){
    if(_runners[i])
      _runners[i]->runStrategy();
  }

  int bestid = 0;
  for(int i = 0; i < 7; i++){
    std::cout << i << ": " << _runners[i]->getFinalPnl() << std::endl;
    if(_runners[i]->getFinalPnl() > _runners[bestid]->getFinalPnl())
      bestid = i;
  }
  std::cout << bestid << std::endl;
  _runners[bestid]->writeToFiles();
}
