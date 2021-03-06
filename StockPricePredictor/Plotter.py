#!/usr/bin/env python

__author__ = "Mithun Kumble"

import pylab as plt

#Creates plots based on the parameters passed to the function

def createScatterPlot(xLabel,xValue,yLabel,yValue,fname):
	""" Plot the values based on the parameters passed. 
	xlabel : Label for x axis
	xValue : Values of x
	ylabel : Label for y axis
	yValue : Values of y
	fname: File name of the pdf """
	plt.clf()
	plt.cla()
	plt.scatter(xValue, yValue, c ='blue')
	plt.xlabel(xLabel)
	plt.ylabel(yLabel)
	plt.savefig(fname+".pdf", format='pdf')
	plt.close()
	
def plotChart2YVar1XVar(xlabel, xVal, ylabel, y1Val, y2Val, fname,legend):
	""" Plot the values based on the parameters passed. 
	xlabel : Label for x axis
	ylabel : Label for y axis
	y1Val : List of y1 values
	y2Val: List of y2 values
	fname: File name of the pdf 
	legend: list of entries for the legend """
	plt.clf()
	plt.cla()
	plt.plot(xVal, y1Val, 'blue', xVal, y2Val, 'red')
	plt.xlabel(xlabel)
	plt.ylabel(ylabel)
	plt.legend(legend)
	plt.xticks(numpy.arange(0, max(xVal)+1, 20))
	plt.savefig(fname+".pdf", format="pdf")
	plt.close()
	
