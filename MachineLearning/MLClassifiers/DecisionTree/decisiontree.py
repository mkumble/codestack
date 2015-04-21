#!/usr/bin/env python

import csv
import numpy
import math
import sys
from sets import Set

class DecisionTree():
  """ Basic attributes of a decision tree """
  def __init__(self, parent=None, MAX_DEPTH = 10):
    self.parent = parent
    self.children = []
    self.splitFeature = None
    self.splitFeatureValue = None
    self.label = None
    self.MAX_DEPTH = MAX_DEPTH

def classify(tree, features):
    """ Retrieves the label for the given features by classifying the given input features based on the configuration of the given decision tree"""
    if(tree.children == []):
      return tree.label
    else:
	try:
	  treeSplitFeatureValue = float(features[tree.splitFeature])
	  childrenNumericValues = [child.splitFeatureValue for child in 
tree.children]
	  matchingChildren = [ child for child in tree.children if 
child.splitFeatureValue == min(childrenNumericValues, key=lambda x: 
abs(float(x) - treeSplitFeatureValue)) ]
	except ValueError:
	  matchingChildren = [ child for child in tree.children 
if child.splitFeatureValue == features[tree.splitFeature]]
	if(matchingChildren):
	  return classify(matchingChildren[0],features)
	else:
	  return tree.label
	  
def getLabelWithMajority(data, node):
    """ Assigns a label which has the maximum occurence in the given data to the given node """ 
    labels = [ label for (features, label) in data]
    majorityLabel = max(set(labels), key = labels.count)
    node.label = majorityLabel
    return node
  
def calculateInformationGain(data, featureIndex):
    """ Calculates the information gain of a given feature in the given dataset """
    entropyGain = getDistributionEntropy(getProbabilityDistributionOfData(data))
    for dataSubset in splitData(data,featureIndex):
	entropyGain=entropyGain-getDistributionEntropy(getProbabilityDistributionOfData(dataSubset))
    return entropyGain

def splitData(data, featureIndex):
    """ Splits the data into subsets based on the given featureIndex """	
    featureValues = [features[featureIndex] for (features, label) in data]
    for feature in set(featureValues):
      dataSubset = [(features, label) for (features,label) in data 
if features[featureIndex] == feature]
      yield dataSubset
    
def getDistributionEntropy(probabilityDistributionList):
    """ Retrieves the entropy for the probability distribution of the data """
    entropy = -sum([p * math.log(p,2) for p in probabilityDistributionList])
    return entropy

def getProbabilityDistributionOfData(data):
    """ Converts the data into a probability distribution """
    allLabels = [label for (features,label) in data]
    numEntries = len(allLabels)
    possibleLabelSet = set(allLabels)
    probabilityDistributionList = []
    for label in possibleLabelSet:
	probabilityDistributionList.append(float(allLabels.count(label))/numEntries)
    return probabilityDistributionList

def loadData(fileName,delimiter):
  """ Loads the given file and parses the input based on the given delimiter """
  print "Reading file "+str(fileName)+" ..."
  fp = open(fileName)
  lines = fp.readlines()
  data = [line.strip().split('\t') for line in lines]
  data = [(x[0:-1],x[-1]) for x in data]
  return data

def buildDecisionTree(data, root, remainingFeatures):
  """ Builds a decision tree by using features with higher information gain and also limits the tree using a maximum depth to improve accuracy"""
  if(allDataHaveSameLabel(data)):
    root.label = data[0][1]
    return root
  
  if(len(remainingFeatures) == 0):
    return root
  
  bestFeature = max(remainingFeatures, key=lambda index: 
calculateInformationGain(data,index))
  
  if(calculateInformationGain(data,bestFeature) == 0):
    return root
  
  root.splitFeature = bestFeature
  
  for dataSubset in splitData(data, bestFeature):
    child = DecisionTree(parent =root)
    child.splitFeatureValue = dataSubset[0][0][bestFeature]
    root.children.append(child)
    buildDecisionTree(dataSubset, child, remainingFeatures - set([bestFeature]))
  return root

def run_decision_tree(data):
  return buildDecisionTree(data, DecisionTree(), set(range(len(data[0][0]))))

def allDataHaveSameLabel(data):
  """ Checks if all the labels in the dataset are the same. If they are same, then no classification is required. """
  homogenous = len(set([label for features,label in data]))<=1
  return homogenous
      
def runKFoldCrossValidation(K,data):
  """ Runs a K-fold cross validaion on the given input data"""
  print "Running "+str(K)+" fold cross-validation on the input data..."
  results = []
  for k in xrange(K):
    print "Validating fold: "+str(k)+"..."
    training_set = [x for i,x in enumerate(data) if i % K !=k]
    test_set = [x for i,x in enumerate(data) if i % K ==k]
    
    # Construct a tree using training set
    tree = run_decision_tree(training_set)
    # Classify the test set using the tree we just constructed
    actualLabels = [label for features,label in test_set]
    predictedLabels = [classify(tree, features) for features,label in test_set]
    results.extend([(True if a==b else False) for a,b 
in zip(actualLabels,predictedLabels)])
  accuracy = float(results.count(True))/float(len(results))
  print "Accuracy of "+str(K)+" fold cross-validation: %.4f" % accuracy
	
    # Writing results to a file (DO NOT CHANGE)
  f = open("result.txt", "w")
  f.write("accuracy: %.4f" % accuracy)
  f.close()

if __name__ == "__main__":
  print "---- Decision Tree Classifier ----"
  fileName = "./hw4-data.tsv"
  delimiter = "\t"
  data = loadData(fileName,delimiter)
  K=10
  runKFoldCrossValidation(K,data)
