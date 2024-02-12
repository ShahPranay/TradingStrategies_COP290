# temporary makefile
all:
	g++ -I./include src/Strategy.cpp src/StrategyMain.cpp src/LinearRegression.cpp src/Basic.cpp src/DMA.cpp src/AMA.cpp src/MACD.cpp src/RSI.cpp src/ADX.cpp src/PAIRS.cpp -o runner.o
	./runner.o $(strategy) $(symbol) $(symbol1) $(symbol2) $(x) $(start_date) $(end_date) $(n) $(p) $(max_hold_days) $(c1) $(c2) $(oversold_threshold) $(overbought_threshold) $(adx_threshold) $(threshold)
