#include "Strategy.h"
#include <queue>

class FisherTransform : public Strategy {
  public:
    FisherTransform(std::string symbol, std::string start_date, std::string end_date, int max_position, int window_size);
    bool isPositiveCrossover();
    bool isNegativeCrossover();
    void addFisherTransform(const StockData &data);
    void buyStock(const StockData &data);
    void sellStock(const StockData &data);

    virtual void runStrategy();
  private:
    int _max_position, _cur_position, _ma_window_size;
    std::vector<StockData> _stock_data;
    std::queue<double> _fisher_values;
    double _cur_ma;
    bool _is_above_ma;
};


