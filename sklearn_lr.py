import pandas as pd
import numpy as np
from sklearn.linear_model import LinearRegression

# Read CSV file
df = pd.read_csv('./data/LR_train_SBIN.csv')

# Define features and target variable
X = df[['CLOSE', 'OPEN', 'VWAP', 'LOW', 'HIGH', 'NO OF TRADES']]
y = df['CLOSE']

# Lag the features
X_lagged = X.shift(1)
X_lagged.columns = [col + '_lagged' for col in X.columns]

# Include current day's open price in the features
X_lagged['OPEN'] = df['OPEN']

print(X_lagged)
print(y)
# Drop the first row as it will have NaN values after shifting
X_lagged = X_lagged.dropna()
y = y[X_lagged.index]

# Fit linear regression model
model = LinearRegression()
model.fit(X_lagged, y)

# Print learned parameters
print("Learned Parameters:")
print("Intercept (β0):", model.intercept_)
print("Coefficients (β1-β7):", model.coef_)

test_df = pd.read_csv('./data/LR_test_SBIN.csv')
X_test = test_df[['CLOSE', 'OPEN', 'VWAP', 'LOW', 'HIGH', 'NO OF TRADES']]

X_test_lagged = X_test.shift(1)
X_test_lagged.columns = [col + "_lagged" for col in X_test.columns]
X_test_lagged['OPEN'] = test_df['OPEN']
X_test_lagged = X_test_lagged.dropna()

print(X_test_lagged)
y_test = model.predict(X_test_lagged)
print(y_test)
