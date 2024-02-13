#include<string>
#include<vector>
#include<Strategy.h>

class BestOfAll{
  public:
    BestOfAll(std::string symbol, std::string start_date, std::string end_date);
    static std::string decrementYear(std::string datestr);

    void runStrategy();

  private:
    std::string _symbol, _start_date, _end_date;
    std::vector<Strategy *> _runners;
};
