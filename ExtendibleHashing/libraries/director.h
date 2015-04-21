#include<iostream.h>
#include "C:\hashing\tindbuff.cpp"

class Bucket;
class BucketBuffer;
class Directory{
   public:
	 Directory (int maxBucketKeys = -1);
	 ~Directory();
	 int Open(char * name);
	 int Create(char * name);
	 int Close();
	 int Insert(char *key,int recAddr);
	 int Remove(char *key);
	 int Search(char *key); // return recAddr for key
	 ostream & Print(ostream & stream);
   protected:
	 int Depth; // Depth of Directory
	 int NumCells; // Number of entries = 2 ** Depth
	 int *BucketAddr; // Array of Bucket Addresses

	 // protected methods
	 int DoubleSize(); // Double the size of the directory
	 int Collapse(); // Collapse half the size
	 int InsertBucket(int bucketAddr,int first,int last);
	 int RemoveBucket(int bucketIndex,int depth);
		// remove bucket form directory
	 int Find(char * key); // Return bucket address for key
	 int StoreBucket (Bucket * bucket);
	   // update or append bucket in file
	 int LoadBucket (Bucket * bucket,int bucketAddr);
		// Load Bucket from file
	 // members to support directory and bucket fiels
	 int MaxBucketKeys;
	 BufferFile * DirectoryFile;
	 LengthFieldBuffer * DirectoryBuffer;
	 Bucket * CurrentBucket; // Object to hold one bucket
	 BucketBuffer *theBucketBuffer; // buffer for bucket
	 BufferFile *BucketFile;
	 int Pack() const;
	 int Unpack();
	 Bucket *PrintBucket; // object to hold one bucket
	 friend class Bucket;
};


//hjkh hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
const int defaultMaxKeys = 100;

class Bucket : public TextIndex
{
   protected:
	 // there are no public members
	 // access to Bucket members is only through class Directory
	 Bucket (Directory & dir, int maxKeys = defaultMaxKeys);
	 int Insert (char * key, int recAddr);
	 int Remove (char * key);
	 Bucket * Split(); // Split the bucket and redistribute the keys
	 int NewRange (int & newStart, int & newEnd);
		// Calculate the range of a new (split) bucket
	 int Redistribute(Bucket & newBucket); // Redistribute keys
	 int FindBuddy(); // Find the bucket that is the buddy of this
	 int TryCombine(); // Attempt to combine buckets
	 int Combine(Bucket * buddy,int buddyIndex);
	   // Combine two buckets
	 int Depth;
	   // Number of bits used in common
	   // by the keys in this bucket
	 Directory & Dir; // directory that contains the bucket
	 int BucketAddr; // Address of file
	 ostream & Print (ostream &);
	 friend class Directory;
	 friend class BucketBuffer;
};

class BucketBuffer : public TextIndexBuffer
{
   public:
	  BucketBuffer (int keySize, int maxKeys);
	  int Pack(const Bucket & bucket);
	  int Unpack(Bucket & bucket);
};