
#Creates plots based on the parameters based

def createScatterPlot(xLabel,xValue,yLabel,yValue,fname):
	plt.clf()
	plt.cla()
	plt.scatter(xValue, yValue, c ='blue')
	plt.xlabel(xLabel)
	plt.ylabel(yLabel)
	plt.savefig(fname+".pdf", format='pdf')
	plt.close()
	
def plotChart2YVar1XVar(xlabel, xVal, ylabel, y1Val, y2Val, fname,legend):
	plt.clf()
	plt.cla()
	plt.plot(xVal, y1Val, 'blue', xVal, y2Val, 'red')
	plt.xlabel(xlabel)
	plt.ylabel(ylabel)
	plt.legend(legend)
	plt.xticks(numpy.arange(0, max(xVal)+1, 20))
	plt.savefig(fname+".pdf", format="pdf")
	plt.close()
	
