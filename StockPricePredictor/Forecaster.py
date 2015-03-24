#!/usr/bin/env python

__author__ = "Mithun Kumble"

import numpy
from scipy.spatial import cKDTree
import math,random,sys,bisect,time
import numpy,scipy.spatial.distance
import cProfile,pstats
import sys
import numpy as np
import pandas as pd
import numpy as np
import math
import copy
import QSTK.qstkutil.qsdateutil as du
import datetime as dt
import QSTK.qstkutil.DataAccess as da
import QSTK.qstkutil.tsutil as tsu
import QSTK.qstkstudy.EventProfiler as ep
from scipy.optimize import leastsq
import pylab as plt
from numpy import *
import numpy
from scipy.spatial import cKDTree
import math,random,sys,bisect,time
import numpy,scipy.spatial.distance
import cProfile,pstats
import sys


def getXParameters(symbol):
	"""
	The function getXParameters returns the factors affecting the stock price of a symbol. The function can return varied set of parameters affecting stock price. Here, the function returns the parameters determining the shape of a sine wave - amplitude, frequency, phase and the price of the stock.
	"""
	
	 #Retrieve stock prices for a stock
	stock_price_dataframe = array([stock_price_dataframe_actualClose[sym].values]).T
	
	#Get differences between stock prices
	stock_price_difference_dataframe = stock_price_dataframe[0:len(stock_price_dataframe)-1,:] - stock_price_dataframe[1:,:]
        stock_price_difference_dataframe = np.append(stock_price_difference_dataframe,stock_price_dataframe[len(stock_price_dataframe)-1]-stock_price_dataframe[0])
	
	#Calculate phase for the data
	pos = np.argwhere(stock_price_difference_dataframe > 0)
	neg = np.argwhere(stock_price_difference_dataframe < 0)
	a = np.argwhere(pos>neg[0])[0]
	b = np.argwhere(neg>pos[0])[0]

	span = 2*abs(pos[a][0][0] - neg[b][0][0])
	
	#mean of the values
	mean = np.mean(stock_price_dataframe[neg[b][0][0]:neg[b][0][0]+span+1,:])
	
	#calculate maximum altitude for the price
	max_alt = np.max(stock_price_dataframe[neg[b][0][0]:neg[b][0][0]+span+1,:])

	#Normalize the prices
	norm = (stock_price_dataframe - mean)/(max_alt - mean)

	#Phase calculation
	phase = np.zeros(np.shape(norm))

	for i in range(1,len(phase)):
		if norm[i] > 0:
			if norm[i]-norm[i-1] > 0:
				phase[i] = 1
			else:
				phase[i] = 2
		else:
			if norm[i]-norm[i-1] > 0:
				phase[i] = 4
			else:
				phase[i] = 3
				
		freqX2=np.zeros((len(stock_price_dataframe)-105, 1))
        freqX2.fill(span)
        j=0
		
        phaseX3=[]
        ampX1=[]
        priceX4=[]
		
	#Calculating amplitude
        for i in range(0,len(stock_price_dataframe)-105):
                
	        phaseX3.append([np.mean(phase[j:j+100,0])])
		ampX1.append([np.amax(stock_price_dataframe[j:j+100]) - np.amin(stock_price_dataframe[j:j+100])]);
                priceX4.append([np.mean(stock_price_difference_dataframe[j:j+100])])
                j=j+1

		ampX1=np.array(ampX1)
        phaseX3=np.array(phaseX3)
        priceX4=np.array(priceX4)
		
	return ampX1,freqX2,phaseX3,priceX4
	

#Initialize variables
xTrain=[]
yTrain=[]
ls_symbols = []
dt_start = dt.datetime(2001, 01, 1)
dt_end = dt.datetime(2005, 12, 31)
d_data = getData(ls_symbols, dt_start, dt_end)
stock_price_dataframe_actualClose = d_data['actual_close']


#Read stock price information
print "Reading data..."
for i in range(0, 200):
	sym="ML4T-"+str(i).zfill(3);
	ls_symbols.append(sym)

#Create training data for the learner:
# Training paramters include: 1) Phase, 2) Frequency, 3) Amplitude
print "Creating training data..."
for sym in ls_symbols:
	 

	#get training parameters for the given stock symbol
	ampX1,freqX2,phaseX3,priceX4 = getXParameters(sym)
		
		
	#Add the training parameters (X1 - Amplitude, X2 - Frequency, X3 - Phase, X4 - Price) to an array
	xTrain.append(numpy.hstack((ampX1,freqX2,phaseX3,priceX4)))
	
	#The y values for training are the differences in stock price data
	yTrain.append(stock_price_dataframe[105:len(stock_price_dataframe)]-stock_price_dataframe[104])
        npXTrain = np.vstack(xTrain)
        npYTrain = np.vstack(yTrain)
 
npXTrain = np.vstack(xTrain)
npYTrain = np.vstack(yTrain)

#Select an appropriate learner for training
k=7
learner = KNNLearner(k)

#Training the learner with the data
print "Training data using KNN Learner with k = "+str(k);
learner.addEvidence(npXTrain,npYTrain[:,0])
ls_symbols = []


#Read test data
print "Reading data from ML4T-292 and ML4T-324 for testing"; 
ls_symbols.append("ML4T-292")
ls_symbols.append("ML4T-324")
dt_start = dt.datetime(2006, 01, 1)
dt_end = dt.datetime(2007, 12, 31)
d_data = getData(ls_symbols,dt_start, dt_end)
stock_price_dataframe_actualClose = d_data['actual_close']


#Initializing parameters
xTest=[]


print "Creating testing data parameters..."
for sym in ls_symbols:
	
	#Retrieve parameters for querying
	ampX1,freqX2,phaseX3,priceX4=getXParameters(sym)
	
	#Query the learner
	yTest = learner.query(numpy.hstack((ampX1,freqX2,phaseX3,priceX4)))
	
	#Add the price difference (normalize)
    yTest = yTest[:]+stock_price_dataframe[104]
	
	if(sym == "ML4T-324"):
		yTest=yTest[60:]
		test= np.zeros(60)
		test[:] = numpy.NAN
		yTest = np.append(yTest,test)	
	yTestAllDays= np.zeros(105)
	yTestAllDays[:] = numpy.NAN
	yTestAllDays = np.append(yTestAllDays,yTest)

        yActual = stock_price_dataframe
		
	print "Calculating Correlation Coefficient for the data set "+str(sym)+".csv...";
	corrCoeff= numpy.corrcoef(yTestAllDays[105:len(yTestAllDays)-105], yActual[105:len(yActual),0])[0,1]
	print "Correlation Coefficient = "+str(corrCoeff);
	
	print "\nCalculating RMS Error for the data set "+str(sym)+".csv...";
	rmse = calculateRMSError(yActual[105:len(yActual),0],yTestAllDays[105:len(yTestAllDays)-105])
	print "RMS Error = "+str(rmse);
       
        if(sym == "ML4T-292"):
			print "\nCreating timeseries plot of Days vs Features(Amplitude, Frequency, Phase, Price difference) for the first 200 days of dataset "+str(sym)+".csv...";
                	plt.clf()
	                plt.cla()
                        xVal=range(0,200)
	                plt.plot(xVal, ampX1[0:200], 'blue', xVal, freqX2[0:200], 'red',xVal,phaseX3[0:200],'green',xVal,priceX4[0:200],'yellow')
	                plt.xlabel("Days")
	                plt.ylabel("Features")
                        l=["Amplitude","Frequency","Phase"]
	                plt.legend(l)
	                plt.xticks(numpy.arange(min(xVal), max(xVal)+1, 50))
	                plt.savefig(sym+"_FeaturesVsDays.pdf", format="pdf")
	                plt.close()


	legend = ["YActual" , "YPredicted"]
	print "\nCreating time series plot of Days vs (YPredict,YActual) for the first 200 days of the dataset "+str(sym)+".csv...";
	plotChart2YVar1XVar("Days", range(0,200), "YActual", yActual[0:200], yTestAllDays[0:200],sym+"_First200DaysYPredictedYActualVsDays",legend)


	yActual = stock_price_dataframe
        legend = ["YActual" , "YPredicted"]
        yActualEnd= np.zeros(105)
	yActualEnd[:] = numpy.NAN
	yActualWithNaN = np.append(yActual,yActualEnd)
        temp = np.zeros(5)
        temp[:]=numpy.NAN
        yActualFiveNaN= np.append(yActual,temp)
        print "\nCreating time series plot of Days vs (YPredict,YActual) for the last 200 days of the dataset "+str(sym)+".csv...";
        plotChart2YVar1XVar("Dates", range(0,200), "YActual", yActual[len(yActual)-200:len(yActual)], yTestAllDays[len(yTestAllDays)-105-200:len(yTestAllDays)-105],sym+"_Last200DaysYPredictedYActualVsDays",legend)

	print "\nCreating scatterplot of YPredict vs YActual for the dataset "+str(sym)+".csv...";
	createScatterPlot("PredictedY",yTestAllDays[105:len(yTestAllDays)-105],"ActualY",yActual[105:len(yActual),0],sym+"_PredictedYVsActualY")

print "Done!"


