#!/usr/bin/env python

__author__ = "Mithun Kumble"

import math
import numpy


#Returns the Euclidean Distance between two arrays
def getEuclideanDistance(sourceArray, destinationArray):
	"""
	The function returns the Euclidean distance between the sourceArray and destinationArray
	"""
	return math.sqrt((sourceArray[0] - destinationArray[0])**2 + (sourceArray[1] - destinationArray[1])**2 + (sourceArray[2] - destinationArray[2])**2 + (sourceArray[3] - destinationArray[3])**2)				
    

#Calculates the Root Mean square error between two arrays
def calculateRMSError(yResult,yActual):
	"""
	The function calculates the Root Mean Square Error between the actual value of Y versus the predicted value of Y
	"""
	rmse = numpy.sqrt(numpy.mean((yResult - yActual)**2));
	return rmse
