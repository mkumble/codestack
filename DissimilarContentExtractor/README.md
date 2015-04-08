Tool : DissimilarContentExtractor

The tool extracts dissimilar content among a given set of articles (or from files) by calculating the Cosine distance between the tf-idf(Term Frequency-Inverse Document Frequency) vectors of the articles. 

Method: getDissimilarContent(inputType,contentList, strictness=0.3)

	The method retrieves the dissimilar content from the contentList for a given threshold(strictness). The method takes either an array or a file as an inputType.
 
	Input Parameters:
	  inputType: 'array' for analyzing articles and 'file' for analyzing multiple files
	  contentList: If inputType is 'array', then the contentList must be a list of article contents. If inputType is 'file', then the contentList must contain the list of the absolute path of the files that have to be analyzed.
	  strictiness: The strictness in comparison (a threshold). Varies between -1 to 1 
	  

	Output:
	Returns the filtered set of articles based on strictness. The results are unordered.
