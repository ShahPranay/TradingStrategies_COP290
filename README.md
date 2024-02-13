# COP290 Assignment 1.3: Trading Strategies

## Overall Structure

The `Strategy` base class provides the basic methods to fetch stock data and write to various streams and files. All the other strategies discused are implemented as derived classes of `Strategy`. The `runStrategy` method is a virtual function of the base class and is
implemented in all the derived classes according to the strategies. This helps have the same interface for all the strategies while also keeping their code serparate and modular.

### Strategy::fetchStockData

This method runs a python script to fetch the data using the jugaad-data library and store it in a .csv file.

## Strategies

The following strategies have been implemented. Usage instructions are provided in `StrategyMain.cpp`

### Basic

### n-Day Moving Average (DMA)
![DMA Plot](./Plots/DMA_plot.png)

### Adaptive Moving Average (DMA++)
![](./Plots/AMA_plot.png)

### MACD
![](./Plots/MACD_plot.png)

### Relative Strength Index (RSI)
![](./Plots/RSI_plot.png)

### ADX
![](./Plots/ADX_plot.png)

### Linear Regression 

The normal equation for the linear regression parameters is $$\beta = (X^T \cdot X)^{-1} \cdot X^T \cdot y$$ where $X$ is the input feature matrix and $y$ is the target feature.
To solve the above equation, we implement a `Matrix` class which has member functions to get the transpose, inverse and multiply two matrices. Once the parameters are identified, the predictions are computed as
$$y_{\text{pred}} = \beta^T \cdot x$$

The following plot shows both the predictions and the actual close prices for AXISBANK. We can see that the predictions are pretty bad and hence the strategy also generally performs badly.
![](./Plots/LR_plot.png)

### Best Of All

The best of all strategy is implemented by creating a `std::vector<Strategy *>` and calling the `runStrategy` method on each of them. This is where polymorphism plays a role. The run methods are called parallely by using omp parallel for directive. Moreover, the fetching
of data has also been parallelized to increase efficiency.

### Pairs Trading
![](./Plots/Pairs_plot.png)

### Stop-Loss in Pairs Rrading
![](./Plots/Pairs2_plot.png)
