#!/usr/bin/env python

__author__ = "Mithun Kumble"

import numpy
from scipy.spatial import cKDTree
import math,random,sys,bisect,time
import numpy,scipy.spatial.distance
import cProfile,pstats
import sys
from CommonUtils import calculateRMSError
from DataHandler import getflatcsv

 
class LinRegLearner:
	def __init__(self):
	  """ 
	  Initialize the variables
	  """
		self.Xtrain = None
		self.Ytrain = None
		self.coeff = None
		self.res = None
		

	def addEvidence(self,Xtrain,Ytrain=None):
	  """
	  Trains the Linear Regression Learner from the XTrain values
	  """
		self.Xtrain = Xtrain
		self.Ytrain = Ytrain
		xTrainIdentityMatrix = numpy.hstack([self.Xtrain, numpy.ones((len(self.Xtrain), 1))])	
		self.coeff = numpy.zeros(2)
		self.coeff[0] = numpy.linalg.lstsq(xTrainIdentityMatrix, Ytrain)[0][0]
		self.coeff[1] = numpy.linalg.lstsq(xTrainIdentityMatrix, Ytrain)[0][1]
		self.res = numpy.linalg.lstsq(xTrainIdentityMatrix, Ytrain)[0][2]
	
    	def query(self,XTest):
	  """
	  Retrieves the predicted Y values based on the input XTest values
	  """
		Y = numpy.dot(XTest, self.coeff) + self.res
		return Y			

	


def testLinRegLearner(fname):
	"""
	The function testLinRegLearner does the following things:
	i) Creates a Linear Regression  Learner
	ii) Trains the learner using about 60% of the data
	iii Tests the learner using 40% of the data - Calculates the Root Mean Square Error, Correlation Coefficient for the predicted values.
	"""
	learner = LinRegLearner()
	data = getflatcsv(fname)
	xTrainData = data[0:0.6*len(data),0:2]
	yTrainData = data[0:0.6*len(data),2:3]
	xTest = data[0.6*len(data):len(data),0:2]
	learner.addEvidence(xTrainData,yTrainData)
	yResult = learner.query(xTest)
	yActual = data[0.6*len(data):len(data),2]
	rmse = calculateRMSError(yResult,yActual)
	corrCoeff= numpy.corrcoef(yResult, yActual)[0,1]
	return rmse,corrCoeff,yActual,yResult


	
fname = "data-classification-prob.csv"
rmse,corrCoeff,yActual,yResult = testLinRegLearner(fname)
print "\n\tLearner: Linear Regression Learner"
print "\t\tFile Name: "+fname
print "\t\tRMS Error = "+str(rmse)
print "\t\tCorrelation Coefficient = "+str(corrCoeff)+"\n\n"



fname = "data-ripple-prob.csv"
rmse,corrCoeff,yActual,yResult = testLinRegLearner(fname)
print "\n\tLearner: Linear Regression Learner"
print "\t\tFile Name: "+fname
print "\t\tRMS Error = "+str(rmse)
print "\t\tCorrelation Coefficient = "+str(corrCoeff)+"\n\n"


