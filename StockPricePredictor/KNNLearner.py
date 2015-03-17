class KNNLearner:
	def __init__(self, k=3,method='mean',leafsize=10):
		self.k= k;
		self.data = None;
		self.rebuild_tree = True
		self.method = method
		self.leafsize = leafsize

	def addEvidence(self,dataX,dataY=None):
	        if not dataY == None:
	            data = numpy.zeros([dataX.shape[0],dataX.shape[1]+1])
	            data[:,0:dataX.shape[1]]=dataX
	            data[:,(dataX.shape[1])]=dataY
	        else:
	            data = dataX
	        
	        self.rebuild_tree = True
	        if self.data is None:
	            self.data = data
	        else:
	            self.data = numpy.append(self.data,data,axis=0)
	
    	def query(self,XTest):
		i=0
		j=0
		distanceYMatrix = numpy.zeros((len(self.data),2))
		yResult =numpy.zeros(len(XTest))
      		for xTest in XTest:
			for storedDataRow in self.data:
				distanceYMatrix[i][0] = getEuclideanDistance(xTest,storedDataRow)
				distanceYMatrix[i][1] = storedDataRow[4]
				i=i+1
			distanceYMatrix = distanceYMatrix[distanceYMatrix[:,0].argsort()]
			i=0
			yResult[j] = numpy.mean(distanceYMatrix[0:self.k,1])
       			j=j+1
		j=0
		return yResult	
