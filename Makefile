# temporary makefile
all:
	g++ -fopenmp -I./include src/Strategy.cpp src/StrategyMain.cpp src/LinearRegression.cpp src/Basic.cpp src/DMA.cpp src/AMA.cpp src/MACD.cpp src/RSI.cpp src/ADX.cpp src/BestOfAll.cpp -o ./bin/runner.o
	./bin/runner.o $(strategy) $(symbol) $(symbol1) $(symbol2) $(n) $(x) $(p) $(train_start_date) $(train_end_date) $(start_date) $(end_date) $(max_hold_days) $(c1) $(c2) $(oversold_threshold) $(overbought_threshold) $(adx_threshold)

clean:
	rm ./bin/*.o ./results/*.csv ./results/*.txt
