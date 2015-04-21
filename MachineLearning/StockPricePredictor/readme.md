Stock Price Predictor:

The project Stock Price Predictor predicts the price of stocks using Machine Learning algorithms. The algorithms used for the project are Linear Regression Learner, KNN Learner, Decision Tree Learner.


I) Datasets: 

Datasets are retrieved from Yahoo Finance using QSTK. The project assumes that QSTK is installed using the steps provided in:
http://wiki.quantsoftware.org/index.php?title=QSToolKit_Installation_Guide



II) Program Structure:

The main modules in the project are:

i) Forecaster: The module deals with the business logic of predicting prices for a stock using various strategies.

ii) Learners:

    a) LinRegLearner: The module contains implementation of the Linear Regression Learner. 
    
    b) KNNLearner: The module contains implementation of the KNN Learner.
    
    c) RandomForestLearner: The module uses a combination of decision tree learners to implement the Random Forest Learner.

 The forecaster uses on of the learners in its logic to predict the stock prices.
 
  
 Miscellaneous:

  a) CommonUtils: The module provides all the utilities for simplifying the tasks.

  b) DataHandler: The DataHandler module handles all the I/O operations required such as retrieving data from a data source and writing to a data store.
  

  
  
  