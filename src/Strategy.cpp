#include "Strategy.h"
#include <fstream>
#include <sstream>
#include <iostream>

Strategy::Strategy (std::string symbol) :
  _symbol (symbol)
{
  _daily_cashflow << "Date,Cashflow\n";
  _order_statistics << "Date,Order_dir,Quantity,Price\n";
}

std::string Strategy::convertDateFormat(const std::string& originalDate) {
  // Assuming the original format is "dd-mm-yyyy"
  // Convert it to "dd/mm/yyyy"
  std::string convertedDate = originalDate;
  size_t dashPos = convertedDate.find('-');
  while (dashPos != std::string::npos) {
    convertedDate.replace(dashPos, 1, "/");
    dashPos = convertedDate.find('-', dashPos + 1);
  }
  return convertedDate;
}

void Strategy::fetchStockData(std::string start_date, std::string end_date, int n)
{
  std::string command = "python3 main.py " + _symbol + " " + std::to_string(n) + " " + start_date + " " + end_date;
  system(command.c_str());

  std::ifstream csvFile(_symbol + ".csv");

  if (!csvFile.is_open()) {
    std::cerr << "Error: Failed to open CSV file\n";
    exit(1);
  }

  std::string line;
  std::getline(csvFile, line); // Skip the first line

  // Read each line from the CSV file
  while (getline(csvFile, line)) {
    std::istringstream iss(line);
    std::string token;

    // Create a StockData object for each line
    StockData data;

    // Parse each token in the line
    std::getline(iss, token, ',');
    data.date = convertDateFormat(token); // Convert date format
                                          //
    std::getline(iss, token, ',');
    data.open = std::stod(token);

    std::getline(iss, token, ',');
    data.close = std::stod(token);

    std::getline(iss, token, ',');
    data.high = std::stod(token);

    std::getline(iss, token, ',');
    data.low = std::stod(token);

    std::getline(iss, token, ',');
    data.ltp = std::stod(token);

    std::getline(iss, token, ',');
    data.volume = std::stod(token);

    std::getline(iss, token, ',');
    data.value = std::stod(token);

    std::getline(iss, token, ',');
    data.prevClose = std::stod(token);

    std::getline(iss, token, ',');
    data.numTrades = std::stoi(token);

    _stock_data.push_back(data);
  }
}

void Strategy::writeToPNLFile(double pnl)
{
  std::ofstream finalPnLFile("final_pnl.txt");
  if (!finalPnLFile.is_open()) {
    std::cerr << "Error: Failed to open file for writing\n";
    return;
  }

  finalPnLFile << _final_pnl << std::endl;
  finalPnLFile.close();
}

void Strategy::writeOrderStats(std::string date, bool is_buy, int quantity, int price)
{
  std::string direction = is_buy ? "BUY" : "SELL", sep = ",";

  _order_statistics << date << sep << direction << sep << quantity << sep << price << std::endl;
}

void Strategy::writeOrderStats(const StockData& stk, bool is_buy, int quantity)
{
  writeOrderStats(stk.date, is_buy, quantity, stk.close);
}

void Strategy::writeDailyCashFlow(std::string date, double cashflow)
{
  std::string sep = ",";

  _daily_cashflow << date << sep << cashflow << std::endl;
}

void Strategy::writeToFiles()
{
  std::ofstream cash_flow_file("daily_pnl.csv");
  std::ofstream order_statistics_file("order_statistics.csv");

  if (!cash_flow_file.is_open() || !order_statistics_file.is_open()) {
    std::cerr << "Error: Failed to open files for writing\n";
    return;
  }

  cash_flow_file << _daily_cashflow.str();
  order_statistics_file << _order_statistics.str();

  cash_flow_file.close();
  order_statistics_file.close();
}
