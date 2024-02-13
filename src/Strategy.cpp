#include "Strategy.h"
#include <fstream>
#include <sstream>
#include <iostream>

Strategy::Strategy ()
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

std::vector<StockData> Strategy::fetchStockData(std::string symbol, std::string start_date, std::string end_date, int n, std::string filename = "data")
{
  std::string filepath = "./data/" + filename + "_" + symbol;
  std::string command = "python3 main.py " + symbol + " " + std::to_string(n) + " " + start_date + " " + end_date + " " + filepath;
  system(command.c_str());

  std::ifstream csvFile(filepath + ".csv");

  if (!csvFile.is_open()) {
    std::cerr << "Error: Failed to open CSV file\n";
    exit(1);
  }

  std::string line;
  std::getline(csvFile, line); // Skip the first line
  std::vector<StockData> stock_data;
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
    data.vwap = std::stod(token);

    std::getline(iss, token, ',');
    data.numTrades = std::stoi(token);

    stock_data.push_back(data);
  }
  return stock_data;
}

void Strategy::writeToPNLFile(double pnl)
{
  std::ofstream finalPnLFile("final_pnl.txt", std::ofstream::trunc);
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

void Strategy::writeOrderStats2(std::string date, bool is_buy, int quantity, int price)
{
  std::string direction = is_buy ? "BUY" : "SELL", sep = ",";

  _order_statistics_2 << date << sep << direction << sep << quantity << sep << price << std::endl;
}

void Strategy::writeOrderStats2(const StockData& stk, bool is_buy, int quantity)
{
  writeOrderStats2(stk.date, is_buy, quantity, stk.close);
}

void Strategy::writeDailyCashFlow(std::string date, double cashflow)
{
  std::string sep = ",";

  _daily_cashflow << date << sep << cashflow << std::endl;
}

void Strategy::writeToFiles()
{
  std::ofstream cash_flow_file("daily_pnl.csv", std::ofstream::trunc);
  std::ofstream order_statistics_file("order_statistics.csv", std::ofstream::trunc);

  if (!cash_flow_file.is_open() || !order_statistics_file.is_open()) {
    std::cerr << "Error: Failed to open files for writing\n";
    return;
  }

  cash_flow_file << _daily_cashflow.str();
  order_statistics_file << _order_statistics.str();

  cash_flow_file.close();
  order_statistics_file.close();

  writeToPNLFile(_final_pnl);
}

void Strategy::writeTo2Files()
{
  std::ofstream cash_flow_file("daily_pnl.csv");
  std::ofstream order_statistics_file1("order_statistics1.csv");
  std::ofstream order_statistics_file2("order_statistics2.csv");

  if (!cash_flow_file.is_open() || !order_statistics_file1.is_open() || !order_statistics_file2.is_open()) {
    std::cerr << "Error: Failed to open files for writing\n";
    return;
  }

  cash_flow_file << _daily_cashflow.str();
  order_statistics_file1 << _order_statistics.str();
  order_statistics_file2 << _order_statistics_2.str();

  cash_flow_file.close();
  order_statistics_file1.close();
  order_statistics_file2.close();

  writeToPNLFile(_final_pnl);
}

double Strategy::getFinalPnl()
{
  return _final_pnl;
}

void Strategy::createPlot(std::vector<double> &data1, std::vector<double> &data2, std::vector<double> &data3, 
                          std::string label1, std::string label2, std::string label3, std::string filename = "data")
{
  FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    
    std::ofstream dataFile1("data/data1.txt");
    std::ofstream dataFile2("data/data2.txt");
    std::ofstream dataFile3("data/data3.txt");

    for (size_t i = 0; i < data1.size(); ++i) {
        dataFile1 << i << " " << data1[i] << std::endl;
        dataFile2 << i << " " << data2[i] << std::endl;
        dataFile3 << i << " " << data3[i] << std::endl;
    }

    dataFile1.close();
    dataFile2.close();
    dataFile3.close();

    fprintf(gnuplotPipe, "set terminal pngcairo enhanced color size 1200,900\n");
    fprintf(gnuplotPipe, "set output 'Plots/%s_plot.png'\n", filename.c_str());

    // Plot the data using Gnuplot
    fprintf(gnuplotPipe, "plot 'data/data1.txt' with lines title '%s', \
                          'data/data2.txt' with lines title '%s', \
                          'data/data3.txt' with lines title '%s'\n", label1.c_str(), label2.c_str(), label3.c_str());

    fflush(gnuplotPipe);

    // Close the Gnuplot pipe
    pclose(gnuplotPipe);
}

void Strategy::createPlot2(std::vector<double> &data1, std::vector<double> &data2, 
                          std::string label1, std::string label2, std::string filename = "data")
{
  FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    
    std::ofstream dataFile1("data/data1.txt");
    std::ofstream dataFile2("data/data2.txt");

    for (size_t i = 0; i < data1.size(); ++i) {
        dataFile1 << i << " " << data1[i] << std::endl;
        dataFile2 << i << " " << data2[i] << std::endl;
    }

    dataFile1.close();
    dataFile2.close();

    fprintf(gnuplotPipe, "set terminal pngcairo enhanced color size 1200,900\n");
    fprintf(gnuplotPipe, "set output 'Plots/%s_plot.png'\n", filename.c_str());

    // Plot the data using Gnuplot
    fprintf(gnuplotPipe, "plot 'data/data1.txt' with lines title '%s', \
                          'data/data2.txt' with lines title '%s'\n", label1.c_str(), label2.c_str());

    fflush(gnuplotPipe);

    // Close the Gnuplot pipe
    pclose(gnuplotPipe);
}
