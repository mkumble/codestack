#!/usr/local/bin/python2.7

import re
import numpy
import collections
import sys
import fileinput
import string
import math
import sys
from sets import Set

numpy.set_printoptions(threshold=numpy.nan)

def calculateScore(tfIdfMatrix):
    """ Predicts if a file provides 'good' or 'bad' information based on cosine similarity """
    score = 0
    for i in range(len(tfIdfMatrix)):
        if (("good" in tfIdfMatrix[i][0] and "good" in tfIdfMatrix[i][1]) or ("bad" in tfIdfMatrix[i][0] and "bad" in tfIdfMatrix[i][1])):
            score = score + 1
    scorepercent = "{0:.6%}".format(score/(float)(len(tfIdfMatrix)))
    return scorepercent

def getCosineValues(tfidf,contentIndexList):
    """ Calculates the cosine similarity between the vectors """
    #print tfidf
    vector1Vector2DotProduct = 0
    vector1SumOfSquaredComponents = 0
    vector2SumOfSquaredComponents = 0
    vector1Magnitude = 0
    vector2Magnitude = 0
    
    numOfFiles = len(tfidf[0])
    fileCosineSimilarityMatrix = numpy.zeros((numOfFiles,numOfFiles))
    fileHighestSimilarityMap = dict()
    vector1Vector2HighestCosineSimilarity_Index = 0
    vector1Vector2HighestCosineSimilarity = 0
    counter2=0

    for i in range(numOfFiles):

        for k in range(numOfFiles):		
            #Skipping comparison of same files
	    if (i==k):
                fileCosineSimilarityMatrix[i][k] = 1
                continue
		
            for j in range(len(tfidf)):
                vector1Vector2DotProduct = vector1Vector2DotProduct + tfidf[j][i]*tfidf[j][k]
                vector1SumOfSquaredComponents = vector1SumOfSquaredComponents + tfidf[j][i]*tfidf[j][i]
                vector2SumOfSquaredComponents = vector2SumOfSquaredComponents + tfidf[j][k]*tfidf[j][k]
		
            vector1Magnitude = math.sqrt(vector1SumOfSquaredComponents)
            vector2Magnitude = math.sqrt(vector2SumOfSquaredComponents)

            if ((vector1Magnitude != 0 and vector2Magnitude != 0)):
                vector1Vector2CosineSimilarity = vector1Vector2DotProduct / (float)(vector1Magnitude*vector2Magnitude)

            elif(vector1Magnitude ==0 and vector2Magnitude == 0 and vector1Vector2DotProduct ==0):
                vector1Vector2CosineSimilarity = 1
            else:
                vector1Vector2CosineSimilarity = 0

            fileCosineSimilarityMatrix[i][k] = vector1Vector2CosineSimilarity

	    #finding the most similar file
            if(vector1Vector2CosineSimilarity >= vector1Vector2HighestCosineSimilarity):
                vector1Vector2HighestCosineSimilarity = vector1Vector2CosineSimilarity
                vector1Vector2HighestCosineSimilarity_Index = k
	
	    #Reset values
            vector1Vector2DotProduct=0
            vector1SumOfSquaredComponents=0
            vector2SumOfSquaredComponents=0
            vector1Vector2CosineSimilarity = 0

        if (vector1Vector2HighestCosineSimilarity ==0):
		similarFile = "None"
	else:
		similarFile = contentIndexList[vector1Vector2HighestCosineSimilarity_Index]

        fileHighestSimilarityMap[contentIndexList[i]] = similarFile
        vector1Vector2HighestCosineSimilarity_Index = 0
	vector1Vector2HighestCosineSimilarity = 0
        vector1Vector2CosineSimilarity=0 
    #print fileCosineSimilarityMatrix
    return fileCosineSimilarityMatrix


def getWordFreqInText(word, fileContent):
	""" Retrieves the count of the given word in the given text """	
	#print word
	p = "".join(fileContent)
	#print p
	num = p.split().count(word)
	return num


def calculate_tf(wordCountMatrix):
    """ Calculates the term frequency for the given word and the word distribution in the given set of files """
    tfMatrix = numpy.zeros(shape=(len(wordCountMatrix),len(wordCountMatrix[0]))) 

    for fileIndex in range(len(wordCountMatrix[0])):
        wordCountOfMaxOccuringWord = wordCountMatrix.max(axis=0)[fileIndex]
        if(wordCountOfMaxOccuringWord == 0):
	    wordCountOfMaxOccuringWord = 1
        for wordIndex in range(len(wordCountMatrix)):
            tfMatrix[wordIndex][fileIndex]= float((wordCountMatrix[wordIndex][fileIndex])/wordCountOfMaxOccuringWord)
    return tfMatrix


def calculate_idf(wordCountMatrix):
    """ Calculates the inverse document frequency for the given word and the word distribution in the given set of files """
    idfMatrix = []
    numOfFiles = len(wordCountMatrix[0])
    for wordIndex in range(len(wordCountMatrix)):
        numOfFilesContainingWord = numpy.count_nonzero(wordCountMatrix[wordIndex])
	if(numOfFilesContainingWord == 0):
	    numOfFilesContainingWord = 1
	idfMatrix.append(numpy.log(numOfFiles/(float(numOfFilesContainingWord))))
    return idfMatrix



def calculate_tfidf(tfMatrix,idfMatrix):
    """ Calculates the tf-idf vector matrix """
    tfidfMatrix = numpy.zeros(shape=(len(tfMatrix),len(tfMatrix[0])))
    for i in range(len(tfMatrix)):
        for j in range(len(tfMatrix[0])):
            tfidfMatrix[i][j] = tfMatrix[i][j] * idfMatrix[i]
    return tfidfMatrix

def getIndexOfDissimilarFiles(fileCosineSimilarityMatrix,fileNameList, theta):
	excludedFileIndexSet = Set()
	dissimilarFileIndexSet = Set(x for x in fileNameList)
	if (theta >1 or theta < -1):
		print "Invalid threshold..please enter values between -1 and 1"
		return
	else:
		for i in range(len(fileCosineSimilarityMatrix)):
			#print i
			for j in range(i+1,len(fileCosineSimilarityMatrix[0])):
					

					if(fileCosineSimilarityMatrix[i][j] > theta):
					  
						excludedFileIndexSet.add(fileNameList[j])
						if fileNameList[j] in dissimilarFileIndexSet:
						  dissimilarFileIndexSet.remove(fileNameList[j])		
	#print dissimilarFileIndexSet
	return dissimilarFileIndexSet

def getContentFromIndex(dissmilarFilesIndexSet,docs):
  """ Retrives the content for the given index. If files are given as input, then dissimilar content from all the files is returned as a list """
  dissimilarContentList = []
  for i in dissmilarFilesIndexSet:
    dissimilarContentList.append(docs[i])
  return dissimilarContentList



def getDissimilarContent(inputType,contentList, strictness=0.3):
  """ The method retrieves the dissimilar content from the contentList for a given threshold(strictness). The method takes either an array or a file as an inputType.
  inputType: 'array' for analyzing articles and 'file' for analyzing multiple files
  contentList: If inputType is 'array', then the contentList must be a list of article contents. If inputType is 'file', then the contentList must contain the list of the absolute path of the files that have to be analyzed.
  strictiness: The strictness in comparison (a threshold). Varies between -1 to 1 """
  counter = 0
  docs = [[] for _ in range(10000)]
  files = []
  #Removing the duplicate contents
  contentList = list(set(contentList))
  unique_words_set = set()
  files.append("term")
  theta = strictness

  if(inputType == "file"):
  #i) Read text from input file, ii) Remove punctuations from the text, iii) store each file content in the array, iv) store unique words in a set
    for line in fileinput.input():
      if fileinput.filename() not in files:
	files.append(fileinput.filename())
     
      line = "".join(l for l in line if l not in string.punctuation).lower().strip()	
      docs[counter].append(line)
      counter = counter + 1
      for word in line.split(" "):
        if word:
            unique_words_set.add(word)

  elif(inputType == "array"):
    for article in contentList:
      files.append(counter)
      article = "".join(l for l in article if l not in string.punctuation).lower().strip().translate(None, string.digits)	
      docs[counter] = article
      counter = counter+1
      for word in article.split(" "):
	if word:
            unique_words_set.add(word)
            
  else:
    print "Invalid input type. Possible types: array/file"
    return
    
  unique_words = list(unique_words_set)
  wordCount = numpy.zeros(shape=(len(unique_words),len(files) - 1))

  for fileIndex in range(len(files)-1):
    for wordIndex in range(len(unique_words)):
        wordCount[wordIndex][fileIndex] = getWordFreqInText(unique_words[wordIndex], docs[fileIndex])

  print "Calculating Term Frequency for the content"
  tfMatrix = calculate_tf(wordCount)
  print "Calculating Inverse Document Frequency for the content"
  idfMatrix = calculate_idf(wordCount)	
  print "Calculating tf-idf for the content"
  tfidfMatrix = calculate_tfidf(tfMatrix,idfMatrix)

  uniqueWords_verticalList = numpy.asarray(unique_words).reshape(-1,1)
  fileCosineSimilarityMatrix=getCosineValues(tfidfMatrix,files[1:])

  dissmilarFilesIndexSet = getIndexOfDissimilarFiles(fileCosineSimilarityMatrix,files[1:], theta) 
  dissimilarTextContent = getContentFromIndex(dissmilarFilesIndexSet,contentList)
  return dissimilarTextContent

textContentList = []
#Sample input
textContent1 = 'This is a test'
textContent2 = 'This is a similar test'
textContent3 = 'Hi'
print "Sample Input: "
print "Text Content 1: "+str(textContent1)
print "Text Content 2: "+str(textContent2)
print "Text Content 3: "+str(textContent3)

inputType = 'array'
textContentList.append(textContent1)
textContentList.append(textContent2)
textContentList.append(textContent3)
strictness = 0.5
dissimilarContent = getDissimilarContent('array',textContentList, strictness)
print "The dissimilar content are "+str(dissimilarContent)