// direct.h
#include<iostream.h>
#include "c:\tc\hash\tindbuff.cpp"

class Bucket; // forward reference
class BucketBuffer; // forward reference

class Directory
{public:
	Directory (int maxBucketKeys = -1);
	~Directory ();
	int Open (char * name);
	int Create (char * name);
	int Close ();
	int Insert (char * key, int recAddr);
	int Remove (char * key);
	int Search (char * key); // return RecAddr for key
	int ReSize (void);
	int Reduction (void);
	void spaceutil(char * myfile);
	ostream & Print (ostream & stream);
protected:
	int Depth; // depth of directory
	int NumCells; // number of entries, = 2**Depth
	int * BucketAddr; // array of bucket addresses

	// protected methods
	int DoubleSize (); // double the size of the directory
	int Collapse (); // collapse, halve the size
	int InsertBucket (int bucketAddr, int first, int last);
	int RemoveBucket (int bucketIndex, int depth);// remove bucket from directory
	int Find (char * key); // return BucketAddr for key
	int StoreBucket (Bucket * bucket);// update or append bucket in file
	int LoadBucket (Bucket * bucket, int bucketAddr);// load bucket from file

	// methods to support Remove

	// members to support directory and bucket files
	int MaxBucketKeys;
	BufferFile * DirectoryFile;
	LengthFieldBuffer * DirectoryBuffer;
	Bucket * CurrentBucket;// object to hold one bucket
	BucketBuffer * theBucketBuffer;// buffer for buckets
	BufferFile * BucketFile;
	int Pack () const;
	int Unpack ();
	Bucket * PrintBucket;// object to hold one bucket


	friend class Bucket;
};

const int defaultMaxKeys = 100;

class Directory;

class Bucket: public TextIndex
{protected:
	// there are no public members,
	// access to Bucket members is only through class Directory
	Bucket (Directory & dir, int maxKeys = defaultMaxKeys); // constructor
	int Insert (char * key, int recAddr);
	int Remove (char * key);
	Bucket * Split ();// split the bucket and redistribute the keys
	int NewRange (int & newStart, int & newEnd);
		// calculate the range of a new (split) bucket
	int Redistribute (Bucket & newBucket); // redistribute keys
	int FindBuddy ();// find the bucket that is the buddy of this
	int TryCombine (); // attempt to combine buckets
	int Combine (Bucket * buddy, int buddyIndex); // combine two buckets
	int Depth;
		// number of bits used 'in common'
		// by the keys in this bucket
	Directory & Dir; // directory that contains the bucket
	int BucketAddr; // address of file
	ostream & Print (ostream &);
	friend class Directory;
	friend class BucketBuffer;
};

class BucketBuffer: public TextIndexBuffer
{public:
	BucketBuffer (int keySize, int maxKeys);
	int Pack (const Bucket & bucket);
	int Unpack (Bucket & bucket);
};


