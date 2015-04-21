
/******************<EXTENDIBLE HASHING>/*****************/


// buffile.cpp

#include "c:\tc\bin\hash\buffile.h"

BufferFile::BufferFile (IOBuffer & from)
 // create with a buffer
	: Buffer (from) {}

int BufferFile::Open (char * filename, int mode)
// open an existing file and check the header
// a correct header must be on the file
// use ios::nocreate to ensure that a file exists
{
	// these modes are not allowed when opening an existing file
	if (mode&ios::noreplace||mode&ios::trunc) return FALSE;

	File . open (filename, mode|ios::in|ios::nocreate| ios::binary);
	if (! File.good()) return FALSE;
	File . seekg(0, ios::beg); File . seekp(0, ios::beg);
	HeaderSize = ReadHeader();
	if (!HeaderSize) // no header and file opened for output
		return FALSE;
	File . seekp (HeaderSize, ios::beg);
	File . seekg (HeaderSize, ios::beg);
	return File . good();
}

int BufferFile::Create (char * filename, int mode)
// create a new file and write a header on it.
// use ios::nocreate to ensure that no file exists
{
	if (!(mode & ios::out)) return FALSE; // must include ios::out
	File . open (filename, mode|ios::in|ios::out|ios::noreplace|ios::binary);
	if (!File . good())
	{
		File . close();
		return FALSE;
	}
	HeaderSize = WriteHeader ();
	return HeaderSize != 0;
}

int BufferFile::Close ()
{
	File . close();
	return TRUE;
}

int BufferFile::Rewind ()
{
	File . seekg (HeaderSize, ios::beg);
	File . seekp (HeaderSize, ios::beg);
	return 1;
}

// Input and Output operations
int BufferFile::Read (int recaddr)
// read a record into the buffer
// return the record address
// return <0 if read failed
// if recaddr == -1, read the next record in the File
// if recaddr != -1, read the record at that address
{
	if (recaddr == -1)
		return Buffer . Read (File);
	else
		return Buffer . DRead (File, recaddr);
}

int BufferFile::Write (int recaddr)
 // write the current buffer contents
{
	if (recaddr == -1)
		return Buffer . Write (File);
	else
		return Buffer . DWrite (File, recaddr);
}

int BufferFile::Append ()
// write the current buffer at the end of File
{
	File . seekp (0, ios::end);
	return Buffer . Write (File);
}

// Access to IOBuffer
IOBuffer & BufferFile::GetBuffer ()
{ return Buffer;}

// protected methods
int BufferFile::ReadHeader ()
{
	return Buffer . ReadHeader (File);
}

int BufferFile::WriteHeader ()
{
	return Buffer . WriteHeader (File); //write header defined in iobuffer

}


//buffile.h

#include"C:\tc\bin\hash\length.cpp"
#include<fstream.h>

class BufferFile
{
   public:
   BufferFile (IOBuffer &); // create with a buffer
   int Open (char * filename, int MODE); // open an existing file
   int Create (char *filename, int MODE); // create a new file
   int Close();
   int Rewind (); // reset to the fiest data record
   // Input and Output operations
   int Read (int recaddr = -1);
     // read a record into the buffer
     // return the record address
     // return <0 if read failed
     // if recaddr == -1, read the next record in the file
     // if recaddr != -1, read the record at that address
   int Write (int recaddr = -1); // write the buffer contents
   int Append (); // write the current buffer at hte end of file
   // Access to IOBuffer
   IOBuffer & GetBuffer ();

   protected:
   IOBuffer & Buffer;
   fstream File;
   int HeaderSize; // size of header
   int ReadHeader ();
   int WriteHeader ();
};


#include <string.h>
#include "c:\tc\bin\HASH\delim.h"

DelimFieldBuffer :: DelimFieldBuffer (char delim, int maxBytes)
  : VariableLengthBuffer (maxBytes)
{
     Init (delim);
}

void DelimFieldBuffer :: Clear()
{
    VariableLengthBuffer:: Clear();
}

int DelimFieldBuffer :: Pack (const void * field, int size)
// set the value of the next field of the buffer;
// if size = -1 (default) use strlen(field) as the Delim of field
// return number of bytes packed,-1 if error
{
    // look for delimeters in field
    short len;
    if (size >= 0) len = size;
    else len = strlen ((char *) field);

    if(len > (short) strlen ((char*) field))
      return -1;
    int start = NextByte;
    NextByte += len+1;
     if(NextByte > MaxBytes) return -1;
    memcpy(&Buffer[start],field,len);
    Buffer[start+len] = Delim;
    BufferSize = NextByte;
    return len;
}

int DelimFieldBuffer :: Unpack(void * field, int maxBytes)
// extract the value of the next field of the buffer
// return the number of bytes extracted, -1 if error
{
   int len = -1;
   int start = NextByte;
   for(int i = start; i < BufferSize; i++)
      if(Buffer[i] == Delim)
      {
	 len = i - start;
	 break;
      }
   if (len == -1) return -1;
   NextByte += len+1;
  // if (NextByte > BufferSize) return -1;
   memcpy(field, &Buffer[start], len);
   if (maxBytes > len || maxBytes == -1)
     ((char *)field)[len] = 0;
   return len;
}


//delim.cpp


void DelimFieldBuffer :: Print (ostream & stream) const
{
     VariableLengthBuffer :: Print(stream);
     stream << "Delimeter'" << Delim << "'"<< endl;
}

int DelimFieldBuffer :: Init(char delim)
{
   Initialized = TRUE;
   Clear ();
   if (delim == -1)
     Delim = DefaultDelim;
   else
     Delim = delim;
   return TRUE;
}

char  DelimFieldBuffer :: getfirstchar(void)
{
   return Buffer[0];
}
void DelimFieldBuffer :: SetDefaultDelim (char delim)
{
  DefaultDelim = delim;
}

char DelimFieldBuffer :: DefaultDelim = 0;



//delim.h
#include "C:\tc\bin\hash\direct.cpp"

class DelimFieldBuffer : public VariableLengthBuffer{
   public:
     DelimFieldBuffer (char Delim = -1, int maxBytes =1000);
	 // construct with a maximum of mexBytes
	 // construct with fields with delimeters
     DelimFieldBuffer (const DelimFieldBuffer & buffer);
	 // copy constructor
     void Clear();
     int Pack (const void*, int size = -1);
	 // set the value of the next field of the buffer
     int Unpack ( void * field, int maxBytes = -1);
	 // extract the value of the next field of the buffer
     void Print (ostream &) const;
     int Init(char delim = 0);
     static void SetDefaultDelim(char delim);
     char getfirstchar(void);
   protected:
     char Delim;
     static char DefaultDelim;
};

inline DelimFieldBuffer :: DelimFieldBuffer (const DelimFieldBuffer & buffer)
   : VariableLengthBuffer (buffer)
{
    Init (buffer.Delim);
}

//direct.h
// direct.cpp

#include "c:\tc\bin\hash\global.h">
#include "c:\tc\bin\hash\direct.h"
#include<math.h>
#include "c:\tc\bin\hash\hash.cpp"
#include <strstream.h>
#include <string.h>
#include<conio.h>
const int MaxKeySize = 12;

Directory::Directory (int maxBucketKeys)
{
	Depth = 0; // depth of directory
	NumCells = 1; // number of entries, = 2**Depth
	BucketAddr = new int [NumCells]; // array of bucket addresses

	MaxBucketKeys = maxBucketKeys;
	//directory file is of lengthfeildbuffer
	DirectoryBuffer = new LengthFieldBuffer; // default size
	DirectoryFile = new BufferFile(*DirectoryBuffer);
	CurrentBucket = new Bucket (*this, MaxBucketKeys);
	theBucketBuffer = new BucketBuffer (MaxKeySize, MaxBucketKeys);
 //bucketbuffer=>textindexbuffer=>fixedfeildbuffer=>fixedlengthbuffer=>iobuf
	BucketFile = new BufferFile (*theBucketBuffer);
	PrintBucket = new Bucket (*this, MaxBucketKeys);
}

Directory::~Directory ()
{
	Close();
}

void makeNames(char * name, char *& dirName, char *& bktName)
{
	ostrstream directoryName;// Derived From: strstreambase, ostream
	directoryName<<name<<".dir"<<ends;
	dirName = strdup(directoryName.str());
	//cout<<dirName;
	ostrstream bucketName;
	bucketName <<name<<".bkt"<<ends;
	bktName = strdup(bucketName.str());
}

int Directory::Open (char * name)    //reads directory file contents to buffer
{                                   //and unpacks to depth,recaddr using unpack func
	int result;
	char * directoryName, * bucketName;
	makeNames(name, directoryName, bucketName);
	result = DirectoryFile->Open(directoryName, ios::in|ios::out);
	if (!result) return 0;
	result = DirectoryFile->Read();//read=>lengthfeildbuffer<=varbuf
	if (result==-1) return 0;
	result = Unpack();
	if (!result==-1) return 0;
	result = BucketFile->Open(bucketName,ios::in|ios::out);
	return result;
}

int Directory::Create (char * name)
{
	// create the two files, clear the directory, create a single bucket
	// and add it to the directory and the bucket file
	int result;
	char * directoryName, * bucketName;
	makeNames(name, directoryName, bucketName);
	result = DirectoryFile->Create(directoryName, ios::in|ios::out);
	if (!result) return 0;
	result = BucketFile->Create(bucketName,ios::in|ios::out);
	if (!result) return 0;
	// store the empty CurrentBucket in the BucketFile and add to Directory
	BucketAddr[0] = StoreBucket (CurrentBucket);
	return result;
}

int Directory::Close ()
{// write the directory and close. error occurs on buffer overflow
	int result;
	result = Pack();
	if (result==-1) return 0;
	DirectoryFile -> Rewind();//sets put and get pointer to begining
	result = DirectoryFile->Write();//write buffer contents back to file
	if (result==-1) return 0;
	return DirectoryFile->Close() && BucketFile->Close(); //performs file close
}

int Directory::Insert (char * key, int recAddr)
{
       //->//if(key[0]=='*')
       //->//   return 0;
	int found = Search (key);
	if (found == -1) return CurrentBucket->Insert(key, recAddr);
	return 0;// key already in directory
}

int Directory::Remove (char * key)
{// remove the key and return its RecAddr
	int bucketAddr = Find(key);
	LoadBucket (CurrentBucket, bucketAddr);
	return CurrentBucket -> Remove (key);
}

int Directory::Search (char * key)
{// return RecAddr for key, also put current bucket into variable
	int bucketAddr = Find(key);
	LoadBucket (CurrentBucket, bucketAddr);
	return CurrentBucket->Search(key);
}

int Directory::ReSize ()
// double the size of the directory
{
	int newSize = 2 * NumCells;
	int * newBucketAddr = new int[newSize];
	for (int i = 0; i < NumCells; i++)
	{
		newBucketAddr[2*i] = BucketAddr[i];
		newBucketAddr[2*i+1] = BucketAddr[i];
	}
	delete BucketAddr;
	BucketAddr = newBucketAddr;
	Depth ++;
	NumCells = newSize;
	return 1;
}


int Directory :: Collapse()
{
   // if collapse is possible reduce size by half
   if (Depth == 0) return 0;// only one bucket
   // look for buddies that are different, if found return
   for (int i=0;i<NumCells;i+=2)
	  if(BucketAddr[i] != BucketAddr[i+1])
		return 0;
   int newSize = NumCells / 2;
   int * newAddrs = new int [newSize];
   for(int j =0; j<newSize;j++)
	 newAddrs[j] = BucketAddr[j*2];
   delete BucketAddr;
   BucketAddr = newAddrs;
   Depth --;
   collapsetrue=1;
   NumCells = newSize;
   return 1;
}


int Directory:: DoubleSize ()
{
  // double the size of the directory
  int newSize = 2 * NumCells;
  int *newBucketAddr = new int[newSize];
  for(int i=0;i<NumCells;i++){
	newBucketAddr[2*i] = BucketAddr[i];
	newBucketAddr[2*i+1] = BucketAddr[i];
  }

  delete BucketAddr;
  BucketAddr = newBucketAddr;
  Depth++;
  NumCells = newSize;
  return 1;
}




int Directory::Reduction ()
{// if collapse is possible, reduce size by half
	if (Depth == 0) return 0; // only 1 bucket
	// look for buddies that are different, if found return
	for (int i = 0; i < NumCells; i += 2)
		if (BucketAddr[i] != BucketAddr[i+1]) return 0;
	int newSize = NumCells / 2;
	int * newAddrs = new int [newSize];
	for (int j = 0; j < newSize; j++)
		newAddrs[j] = BucketAddr[j*2];
	delete BucketAddr;
	BucketAddr = newAddrs;
	Depth --;
	NumCells = newSize;
	return 1;
}

int Directory::InsertBucket (int bucketAddr, int first, int last)
{// change cells to refer to this bucket
	for (int i = first; i <= last; i++)
		BucketAddr[i] = bucketAddr;
	return 1;
}

int Directory::RemoveBucket (int bucketIndex, int bucketDepth)
{
// set all cells for this bucket to its buddy bucket
	int fillBits = Depth-bucketDepth; // number of bits to fill
	int buddyIndex = bucketIndex ^ ( 1 << (fillBits-1) ); // flip low bit
	int newBucketAddr = BucketAddr[buddyIndex];
	int lowIndex = bucketIndex >> fillBits << fillBits; // zero low bits
	int highIndex = lowIndex + (1<<fillBits) - 1; // set low bits to 1
	for (int i = lowIndex; i <= highIndex; i++)
		BucketAddr[i] = newBucketAddr;
	return 0;
}

int Directory::Find (char * key)
// return BucketAddr for key
{
	return BucketAddr[MakeAddress (key, Depth)];
}

int Directory::Pack ()  const
{// pack the buffer and return the number of bytes packed
	int result, packsize;
	DirectoryBuffer -> Clear();
	packsize = DirectoryBuffer -> Pack (&Depth, sizeof(int));
	if (packsize == -1) return -1;
	for (int i = 0; i<NumCells; i++)
	{
		result = DirectoryBuffer -> Pack (&BucketAddr[i], sizeof(int));
		if (result == -1) return -1;
		packsize += result;
	}
	return packsize;
}

int Directory::Unpack ()
{
	int result;
	result = DirectoryBuffer -> Unpack (&Depth, sizeof(int));
	if (result == -1) return -1;
	NumCells = 1 << Depth; //gives number of cells
	if (BucketAddr != 0) delete BucketAddr;
	BucketAddr = new int[NumCells];
	for (int i = 0; i<NumCells; i++)
	{
		result = DirectoryBuffer -> Unpack (&BucketAddr[i], sizeof(int));
		if (result == -1) return -1;
	}
	return 0;
}

int Directory::StoreBucket (Bucket * bucket)
{// update or append the bucket to the bucket file
	int result;
	result = theBucketBuffer -> Pack (*bucket);  //copys entire bucket
							//to buffer
	if (result == -1) return -1;
	int addr = bucket->BucketAddr;
	if (addr != 0) return BucketFile->Write (addr);
	addr = BucketFile -> Append (); //append()=>buf.write()
//during BucketFile declaration is
//thebuckbuf is passed as constructer parameter, hence iobuf::buf of class
//bufferfile is intialized with thebuckbuf object
//thebuckbuf contain write function inherited as shown
//bufferfile<=textindexbuffer<=fixedfeildbuffer<=fixedlengthbuffer
//hence write func of fixedlength buffer writes the content of buf to file
//and returns it's the address which is then assigned to bucket address

	bucket -> BucketAddr = addr;
	return addr;
}

int Directory::LoadBucket(Bucket * bucket, int bucketAddr)
{// read bucket from file, and set BucketAddr field
	int result;
	result = BucketFile -> Read (bucketAddr);
	if (result == -1) return 0;
	result = theBucketBuffer -> Unpack (*bucket);
	if (result == -1) return 0;
	bucket->BucketAddr = bucketAddr;
	return 1;
}

ostream & Directory::Print (ostream & stream)
{
	stream <<"Directory Depth "<<Depth<<" size "<<NumCells<<endl;
	for (int i = 0; i<NumCells; i++)
	{
		stream <<"bucket address is "<<BucketAddr[i]<<" in bucket file\n";
		//<<" addr "<<(void *) i<<endl;
		LoadBucket (PrintBucket, BucketAddr[i]);
		PrintBucket->Print(stream);
		cout << endl << "Press a key..."<<endl;
		getch();
	}
	stream <<"end directory"<<endl;
	return stream;
}


Bucket::Bucket (Directory & dir, int maxKeys) // constructor
:TextIndex (maxKeys), Dir (dir)  //Dir(dir) acts member intialization list
{
	BucketAddr = 0;
	Depth = 0;
}

int Bucket::Insert (char * key, int recAddr)
{
	if (NumKeys < MaxKeys)
	{
		int result = TextIndex::Insert (key, recAddr);
		Dir.StoreBucket (this);
		return result;
	}
	else
	{
		Split ();
		return Dir.Insert (key, recAddr);
	}
}

int Bucket::Remove (char * key)
{// remove the key, return its RecAddr
	int result = TextIndex::Remove (key);
	if (!result) return 0; // key not in bucket
	TryCombine (); // attempt to combine with buddy
	// make the changes permanent
	Dir.StoreBucket(this);
	return 1;
}

Bucket * Bucket::Split ()
{
	int newStart, newEnd;
	if (Depth == Dir.Depth || Dir.NumCells==1)
       {        doublesizetrue=1;
		Dir.DoubleSize();
       }

	Bucket * newBucket = new Bucket (Dir, MaxKeys);
	Dir.StoreBucket (newBucket);
	NewRange (newStart, newEnd);
	Dir.InsertBucket(newBucket->BucketAddr, newStart, newEnd);
	Depth ++;
	newBucket->Depth = Depth;
	Redistribute (*newBucket);
	Dir.StoreBucket (this);
	Dir.StoreBucket (newBucket);
	return newBucket;
}

int Bucket::FindBuddy ()
{// find the directory address of the bucket that is paired with this
	if (Dir.Depth == 0) return -1; // no buddy, empty directory
	// unless bucket depth == directory depth, there is no single
	// bucket to pair with
	if (Depth < Dir.Depth) return -1;
	int sharedAddress = MakeAddress(Keys[0], Depth); // address of any key
	return sharedAddress ^ 1; // exclusive or with low bit
}

int Bucket::TryCombine ()
{// called after removal of key to combine buddies, if possible
	int result;
	int buddyIndex = FindBuddy ();
	if (buddyIndex == -1) return 0;// no combination possible
	// load buddy bucket into memory
	int buddyAddr = Dir.BucketAddr[buddyIndex];
	//allocate a memory to load buddy bucket
	Bucket * buddyBucket = new Bucket (Dir, MaxKeys);
	Dir . LoadBucket (buddyBucket, buddyAddr);
	// if the sum of the sizes of the buckets is too big, return
	if (NumKeys + buddyBucket->NumKeys > MaxKeys) return 0;
	Combine (buddyBucket, buddyIndex);// collapse the 2 buckets
	result = Dir.Collapse ();
	if (result) TryCombine(); //if collapse, may be able to combine
	return 1;
}

int Bucket::Combine (Bucket * buddy, int buddyIndex)
{// collapse this and buddy into a single bucket
	int result;
	// move keys from buddy to this
	for (int i = 0; i < buddy->NumKeys; i++)
	{// insert the key of the buddy into this
		result = Insert (buddy->Keys[i],buddy->RecAddrs[i]);
		if (!result) return 0;
	}
	Depth --;// reduce the depth of the bucket
	Dir . RemoveBucket (buddyIndex, Depth);
	return 1;
}

int Bucket::NewRange (int & newStart, int & newEnd)
{// make a range for the new split bucket
	int sharedAddr = MakeAddress(Keys[0], Depth);
	int bitsToFill = Dir.Depth - (Depth + 1);
	newStart = (sharedAddr << 1) | 1;
	newEnd = newStart;
	for (int j = 0; j < bitsToFill; j++)
	{
		newStart = newStart << 1;
		newEnd = (newEnd << 1) | 1;
	}
	return 1;
}

int Bucket::Redistribute (Bucket & newBucket)
{
	// check each key in this bucket
	for (int i = NumKeys - 1; i >= 0; i--)
	{
		int bucketAddr = Dir.Find (Keys[i]); // look up the bucket
		if (bucketAddr != BucketAddr)// key belongs in the new bucket
		{
			newBucket.TextIndex::Insert (Keys[i], RecAddrs[i]);
			TextIndex::Remove (Keys[i]); // delete key from this bucket
		}
	}
	return 1;
}

ostream & Bucket::Print(ostream & stream)
{
	stream <<"Bucket depth: "<<Depth<<endl;
	TextIndex::Print (stream);
	return stream;
}

BucketBuffer::BucketBuffer (int keySize, int maxKeys)
: TextIndexBuffer(keySize,maxKeys,1,sizeof(int))
{
	AddField (sizeof(int));//textindexbuffer<=fixedfeildbuffer
			      //addfeild is present in fixedfeildbuffer
}

int BucketBuffer::Pack (const Bucket & bucket)
{
	int result;
	TextIndexBuffer::Pack(bucket);
	if (result == -1) return -1;
	return FixedFieldBuffer::Pack (&bucket.Depth);
}

int BucketBuffer::Unpack (Bucket & bucket)
{
	int result;
	result = TextIndexBuffer::Unpack(bucket);
	if (result == -1) return -1;
	return FixedFieldBuffer::Unpack (&bucket.Depth);
}


void Directory::spaceutil(char * myfile)
{
	fstream file(myfile,ios::in);
	float numrecs=0,util;
	char ch;
	while(1)
	{
		file>>ch;
		if(file.fail())
			break;
		else if(ch=='#')
			numrecs++;
	}
	file.close();
	int cnt=1;
	for(int i=0;i<NumCells-1;i++)//counts number of buckets
	{
		if(BucketAddr[i+1]==BucketAddr[i])
			continue;
		cnt++;
	}

	util=(numrecs/(cnt*4))*100;//utilization=r/(bN)

	cout<<"\nTHE NUMBER OF RECORDS IN THE FILE  --> "<<numrecs<<"\n";
	cout<<"\n\nTHE NUMBER OF BUCKETS USED BY THE RECORDS -->"<<cnt++;
	cout<<"\n\nTHE SIZE OF THE DIRECTORY IS -->"<<NumCells;
	cout<<"\n\nSPACE UTILIZATION = "<<util<<"%";
	//for directory
	float x;
	x=pow(numrecs,1.25);
	x=x*0.98;
	cout<<"\n\n THE SPACE OCCUPIED BY THE DIRECTORY -->"<<x<<"bytes(approx)";

	getch();
}


//direct.h
// direct.h
#include<iostream.h>
#include "C:\tc\bin\hash\tindbuff.cpp"

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

//director.h

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

//draw.cpp#include <stdio.h>
#include <iostream.h>
#include<graphics.h>
#include<dos.h>
#include<ctype.h>
#include<conio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>


#define TOPLT 218
#define HORI 196
#define TOPRT 191
#define VERT 179
#define BOTLT 192
#define BOTRT 217


void box(int x1, int y1, int x2, int y2)
{
    int i;


    gotoxy(x1,y1);
    cprintf("%c",TOPLT);
    for(i=x1+1;i<x2;i++)
    cprintf("%c",HORI);

    cprintf("%c",TOPRT);
    for(i=y1+1;i<y2;i++)
    {
	gotoxy(x1,i);	cprintf("%c",VERT);
	gotoxy(x2,i);	cprintf("%c",VERT);
    }
    gotoxy(x1,y2); cprintf("%c",BOTLT);
    for(i=x1+1;i<x2;i++)
    cprintf("%c",HORI);

    cprintf("%c",BOTRT);
    return;
}/* box */



//fixfld.cpp

#include "c:\tc\bin\hash\fixfld.h"

FixedFieldBuffer::FixedFieldBuffer(int maxFields, int maxBytes)
   :FixedLengthBuffer(maxBytes)
{
      Init(maxFields);
}

static int SumFieldSizes (int numFields, int * fieldSize)
{
    int sum = 0;
    for (int i=0; i < numFields; i++)
       sum += fieldSize[i];
    return sum;
}

FixedFieldBuffer & FixedFieldBuffer :: operator = (const FixedFieldBuffer & buffer)
{
   if (NumFields != buffer.NumFields) return *this;
   for(int i=0;i<NumFields; i++)
     if (FieldSize[i] != buffer.FieldSize[i]) return *this;
   NextField = buffer.NextField;
   FixedFieldBuffer :: operator = (buffer);
   return *this;
}

FixedFieldBuffer :: FixedFieldBuffer (int numFields, int *fieldSize)
  :FixedLengthBuffer (SumFieldSizes (numFields, fieldSize))
{
   Init(numFields,fieldSize);
}


int FixedFieldBuffer:: NumberOfFields () const
{
   return NumFields;
}

void FixedFieldBuffer :: Clear()
{
     FixedLengthBuffer :: Clear();
     NextField = 0;
     Buffer[0] = 0;
     Packing = TRUE;
}

int FixedFieldBuffer :: AddField (int fieldSize)
{
   Initialized = TRUE;
   if (NumFields == MaxFields) return FALSE;
   if (BufferSize + fieldSize > MaxBytes) return FALSE;
   FieldSize[NumFields] = fieldSize;
   NumFields ++;
   BufferSize += fieldSize;
   return TRUE;
}

static const char *headerStr3 = "Field";
static const int headerStrSize3 = strlen(headerStr3);

int FixedFieldBuffer :: ReadHeader(istream & stream)
{
   char str[6];
   int numFields, *fieldSize;
   int result;

   result = FixedLengthBuffer::ReadHeader(stream);

   if (result < 0) return -1;
   stream.read(str,headerStrSize3);
   if (!stream.good()) return -1;
   if (strncmp (str,headerStr3,headerStrSize3)!=0) return -1;
   stream.read((char *)&numFields, sizeof(numFields));
   if (!stream) return -1;
   fieldSize = new int [numFields];
   for(int i =0; i < numFields;i++)
     stream.read((char *)&fieldSize[i], sizeof(fieldSize[i]));

   if (Initialized){
     if (numFields != NumFields) return -1;
     for(int j = 0; j < numFields; j++)
       if (fieldSize[j] != FieldSize[j] ) return -1;
     return stream.tellg();
   }
   Init(numFields,fieldSize);
   return stream.tellg();
}

int FixedFieldBuffer :: WriteHeader(ostream & stream) const
{
   int result;
   if (!Initialized) return -1;
   result = FixedLengthBuffer::WriteHeader(stream);
   if(!result) return -1;
   stream.write(headerStr3,headerStrSize3);
   if (!stream.good()) return -1;
   stream.write((char *)&NumFields, sizeof(NumFields));
   for (int i = 0; i < NumFields;i++){
      stream.write((char *)&FieldSize[i], sizeof(FieldSize[i]));
   }
   if (!stream) return -1;
   return stream.tellp();
}

int FixedFieldBuffer :: Pack (const void * field, int size)
{
   if (NextField == NumFields || !Packing)
     return -1;
   int start = NextByte;
   int packSize = FieldSize[NextField];

   if (size != -1 && packSize != size) return -1;
   memcpy(&Buffer[start], field, packSize);
   NextByte += packSize;
   NextField ++;
   if (NextField == NumFields){
     Packing = -1;
     NextField = NextByte = 0;
   }
   return packSize;
}

int FixedFieldBuffer :: Unpack (void *field, int maxBytes)
{
   Packing = FALSE;
   if (NextField == NumFields)
     return -1;
   int start = NextByte;
   int packSize = FieldSize[NextField];
   memcpy(field, &Buffer[start], packSize);
   NextByte += packSize;
   NextField++;
   if (NextField == NumFields) Clear();
   return packSize;
}


void FixedFieldBuffer :: Print(ostream & stream) const
{
   FixedFieldBuffer :: Print (stream);
   stream << endl;
   stream << "\t max fields " << MaxFields<< " and actual "
	  << NumFields << endl;
   for (int i = 0;i < NumFields; i++)
      stream<< "\tfield " << i << " size "<< FieldSize[i] << endl;
   Buffer[BufferSize]=0;
   stream << "NextByte "<<NextByte<<endl;
   stream << "Buffer '" << Buffer<< "'"<<endl;
}

int FixedFieldBuffer :: Init(int maxFields)
{
    Clear();
    if (maxFields < 0) maxFields = 0;
    MaxFields = maxFields;
    FieldSize = new int [MaxFields];
    BufferSize = 0;
    NumFields = 0;
    return 1;
}

int FixedFieldBuffer :: Init(int numFields, int * fieldSize)
{
   Initialized = TRUE;
   Init (numFields);
   for (int j =0; j < numFields; j++)
      AddField(FieldSize[j]);
   return TRUE;
}




/*FixedFieldBuffer :: FixedFieldBuffer (int maxFields, int maxBytes)
 // construct with a maximum of maxFields
: FixedLengthBuffer(maxBytes)
{
	Init (maxFields);
}

// private function to calculate the record size from the field sizes
static int SumFieldSizes (int numFields, int * fieldSize)
{
	int sum = 0;
	for (int i = 0; i < numFields; i++)
		sum += fieldSize[i];
	return sum;
}

FixedFieldBuffer & FixedFieldBuffer :: operator =
	(const FixedFieldBuffer & buffer)
{
	// disallow copy unless fields are identical
	if (NumFields != buffer . NumFields) return *this;
	for (int i = 0; i < NumFields; i++)
		if (FieldSize[i] != buffer . FieldSize[i]) return *this;
	NextField = buffer . NextField;
	FixedLengthBuffer :: operator = (buffer);
	return *this;
}

FixedFieldBuffer :: FixedFieldBuffer (int numFields, int * fieldSize)
// construct with fields of specific size
: FixedLengthBuffer(SumFieldSizes(numFields, fieldSize))
{
	Init (numFields, fieldSize);
}

int FixedFieldBuffer :: NumberOfFields () const
// return number of fields
{
	return NumFields;
}

void FixedFieldBuffer :: Clear ()
// clear fields from buffer
{
	FixedLengthBuffer::Clear ();
	NextField = 0;
	Buffer[0]=0;
	Packing = TRUE;
}

int FixedFieldBuffer :: AddField (int fieldSize)
{
	Initialized = TRUE;
	if (NumFields == MaxFields) return FALSE;
	if (BufferSize + fieldSize > MaxBytes) return FALSE;
	FieldSize[NumFields] = fieldSize;
	NumFields ++;
	BufferSize += fieldSize;
	return TRUE;
}


static const char * headerStr3 = "Field";
static const int headerStrSize3 = strlen (headerStr3);

int FixedFieldBuffer :: ReadHeader (istream & stream)
// read the header and check for consistency
// see WriteHeader for header record structure
{
	char * str = new char[headerStrSize3+1];
	int numFields, *fieldSize;
	int result;
	// read the FixedLengthBufferheader
	result = FixedLengthBuffer::ReadHeader (stream);
	if (result < 0) return -1;
	// read the header string
	stream . read (str, headerStrSize3);
	if (!stream.good()) return -1;
	if (strncmp (str, headerStr3, headerStrSize3) != 0) return -1;
	// read the record description
	stream . read ((char*)&numFields, sizeof(numFields));
	if (!stream) return -1; // failed to read numFields
	fieldSize = new int[numFields];
	for (int i = 0; i < numFields; i ++)
	{
		stream . read ((char*)&fieldSize[i], sizeof(fieldSize[i]));
	}

	if (Initialized) // check header for consistency
	{
		if (numFields != NumFields) return -1;
		for (int j = 0; j < numFields; j ++)
			if (fieldSize[j] != FieldSize[j]) return -1;
		return 1;//stream . tellg (); // everything matches
	}
	// else initialize the buffer from the header
	Init (numFields, fieldSize);
	return 1;//stream.tellg();
}

int FixedFieldBuffer :: WriteHeader (ostream & stream) const
// write a buffer header to the beginning of the stream
// A header consists of the
//	FixedLengthBufferheader
//	FIXED			5 bytes
//	Variable sized record of length fields
//	that describes the file records
//	Header record size	2 bytes
//	number of fields		4 bytes
//	field sizes			4 bytes per field
{
	int result;
	if (!Initialized) return -1; // cannot write unitialized buffer
	// write the parent (FixedLengthBuffer) header
	result = FixedLengthBuffer::WriteHeader (stream);
	if (!result) return -1;
	// write the header string
	stream . write (headerStr3, headerStrSize3);
	if (!stream.good()) return -1;
	// write the record description
//cout << "packing numfields "<<NumFields<<endl;
	stream . write ((char*)&NumFields, sizeof(NumFields));
	for (int i = 0; i < NumFields; i ++)
	{
//cout << "packing fieldsize "<<FieldSize[i]<<endl;
		stream . write ((char*)&FieldSize[i], sizeof(FieldSize[i]));
	}
	if (!stream) return -1;
	return stream . tellp ();
}

int FixedFieldBuffer :: Pack (const void * field, int size)
// set the value of the next field of the buffer;
//    if size != -1, it must be the same as the packSize
// return number of bytes packed, -1 if error
{
//cout<<"Pack NumFields "<<NumFields<<" field "<<(char *)field<<endl;
	if (NextField == NumFields || !Packing) // buffer is full or not packing mode
		return -1;
	int start = NextByte; // first byte to be packed
	int packSize = FieldSize[NextField]; // number bytes to be packed
	if (size != -1 && packSize != size) return -1;
	memcpy (&Buffer[start], field, packSize); // move bytes to buffer
	NextByte += packSize;
	NextField ++;
	if (NextField == NumFields) // all fields packed
	{
		Packing = -1;
		NextField = NextByte = 0;
	}
	return packSize;
}

int FixedFieldBuffer :: Unpack (void * field, int maxBytes)
// extract the value of the next field of the buffer
// return the number of bytes extracted, -1 if error
{
	Packing = FALSE;
	if (NextField == NumFields) // buffer is full
		return -1;
	int start = NextByte; // first byte to be unpacked
	int packSize = FieldSize[NextField]; // number bytes to be unpacked
	memcpy (field, &Buffer[start], packSize);
	NextByte += packSize;
	NextField ++;
	if (NextField == NumFields) Clear (); // all fields unpacked
	return packSize;
}

void FixedFieldBuffer :: Print (ostream & stream) const
{
	FixedLengthBuffer::Print (stream);
	stream << endl;
	stream << "\t max fields "<<MaxFields<<" and actual "<<NumFields<<endl;
	for (int i = 0; i < NumFields; i++)
		stream <<"\tfield "<<i<<" size "<<FieldSize[i]<<endl;
	Buffer[BufferSize]=0;
	stream <<"NextByte "<<NextByte<<endl;
	stream <<"Buffer '"<<Buffer<<"'"<<endl;
}

int FixedFieldBuffer :: Init (int maxFields)
 // construct with a maximum of maxFields
{
	Clear ();
	if (maxFields < 0) maxFields = 0;
	MaxFields = maxFields;
	FieldSize = new int[MaxFields];
	BufferSize = 0;
	NumFields = 0;
	return 1;
}

int FixedFieldBuffer :: Init (int numFields, int * fieldSize)
// construct with fields of specific size
{
	// initialize
	Initialized = TRUE;
	Init (numFields);

	// add fields
	for (int j = 0; j < numFields; j++)
		AddField (FieldSize[j]);
	return TRUE;
} */

//fixfld.h

#include "C:\tc\bin\hash\fixlen.cpp"

class FixedFieldBuffer : public FixedLengthBuffer
{
  public:
   FixedFieldBuffer (int maxFields, int RecoredSize = 1000);
   FixedFieldBuffer (int maxFields,int * fieldSize);
   FixedFieldBuffer (const FixedFieldBuffer &);
   FixedFieldBuffer & operator = (const FixedFieldBuffer &);
   void Clear();
   int AddField(int fieldSize);
   int ReadHeader(istream &);
   int WriteHeader(ostream &) const;
   int Pack (const void * field, int size = -1);
   int Unpack (void * field, int maxBytes = -1);
   void Print(ostream &) const;
   int NumberOfFields () const;
   int Init (int maxFields);
   int Init (int numFields, int * fieldSize);

  protected:
   int *FieldSize;
   int MaxFields;
   int NumFields;
   int NextField;
};

inline FixedFieldBuffer :: FixedFieldBuffer (const FixedFieldBuffer & buffer)
  :FixedLengthBuffer (buffer)
{
   Init(buffer.NumFields, buffer.FieldSize);
}

//fixlen.cpp

#include "c:\tc\bin\hash\fixlen.h"

//class FixedLengthBuffer

// public members

FixedLengthBuffer :: FixedLengthBuffer (int recordSize)
 // construct with a maximum of maxFields
: IOBuffer (recordSize)
{
	Init (recordSize);
}

void FixedLengthBuffer :: Clear ()
// clear fields from buffer
{
	IOBuffer::Clear ();
	Buffer[0]=0;
	Packing = TRUE;
}

int FixedLengthBuffer :: Read (istream & stream)
// write the number of bytes in the buffer field definitions
{
	int recaddr = stream . tellg (); stream.clear();
	Clear ();
	Packing = FALSE;
	stream . read (Buffer, BufferSize);
	if (! stream . good ()){stream.clear(); return recaddr;}
	return recaddr;
}


int FixedLengthBuffer :: Write (ostream & stream) const
// read the number of bytes in the buffer field definitions
// return the location of the record in the file
{
	int recaddr = stream . tellp ();
	stream . write (Buffer, BufferSize);
	if (! stream . good ()) return -1;
	return recaddr;
}

static const char * headerStr4 = "Fixed";
static const int headerStrSize2 = strlen (headerStr4);

int FixedLengthBuffer :: ReadHeader (istream & stream)
// read the header and check for consistency
// see WriteHeader for header record structure
{
	char str[6];
	int recordSize;
	int result;
	// read the IOBuffer header
	result = IOBuffer::ReadHeader (stream);
	if (result < 0) return -1;
	// read the string "Fixed"
	stream . read (str, headerStrSize2);
	if (!stream.good()) return -1;
	if (strncmp (str, headerStr4, headerStrSize2) != 0) return -1;
	stream . read ((char*)&recordSize, sizeof(recordSize));
	if (Initialized) // check header for consistency
	{
		if (recordSize != BufferSize) return -1;
	}
	// else initialize the buffer from the header
	ChangeRecordSize (recordSize);
	return stream.tellg();
}

int FixedLengthBuffer :: WriteHeader (ostream & stream) const
// write a buffer header to the beginning of the stream
// A header consists of the
//	IOBUFFER header
//	FIXED			5 bytes
//	record size		2 bytes
{
	int result;
	if (!Initialized) return -1; // cannot write unitialized buffer
	// write the parent (IOBuffer) header
	result = IOBuffer::WriteHeader (stream);
	if (!result) return -1;
	// write the string "Fixed"
	stream . write (headerStr4, headerStrSize2);
	if (!stream . good ()) return -1;
	// write the record size
	stream . write ((char *)&BufferSize, sizeof(BufferSize));
	if (!stream . good ()) return -1;
	return stream . tellp ();
}

void FixedLengthBuffer :: Print (ostream & stream) const
{
	IOBuffer::Print (stream);
	stream <<  "Fixed ";
}

int FixedLengthBuffer :: Init (int recordSize)
 // construct with a maximum of maxFields
{
	Clear ();
	BufferSize = recordSize;
	return 1;
}

int FixedLengthBuffer :: ChangeRecordSize (int recordSize)
 // construct with a maximum of maxFields
{
	BufferSize = recordSize;
	return 1;
}


//fixlen.h

#include "C:\tc\bin\hash\buffile.cpp"
//#include "e:\hash\iobuffer.cpp"

class FixedLengthBuffer:public IOBuffer
{
   public:
    FixedLengthBuffer (int recordSize = 1000);
    FixedLengthBuffer (const FixedLengthBuffer & buffer);
    void Clear();
    int Read (istream &);
    int Write (ostream &) const;
    int ReadHeader (istream &);
    int WriteHeader (ostream &) const;
    void Print (ostream &) const;
    int SizoOfBuffer () const;
   protected:
    int Init(int recordSize);
    int ChangeRecordSize (int recordSize);
};

inline FixedLengthBuffer::FixedLengthBuffer (const FixedLengthBuffer& buffer)
	   : IOBuffer(buffer)
{
    Init(buffer.BufferSize);
}


//global.h

extern int collapsetrue;
extern int doublesizetrue;

//hash.cpp

#include "c:\tc\bin\hash\hash.h"
#include<string.h>
int Hash (char * key)
{
   int sum = 0;
   int len = strlen(key);
   if (len % 2 == 1) len++; // make len even
   // for an odd length string, use the trailiung 0 as part of key
   for(int j=0; j < len; j+=2)
	 sum = ( sum +100 * key[j] + key[j+1]) %19937;
   return sum;
}

int MakeAddress (char *key, int depth)
{
   int retval = 0;
   int mask = 1;
   int hashVal = Hash(key);

   for ( int j = 0; j < depth; j++){
	  retval = retval << 1;
	  int lowbit = hashVal & mask;
	  retval = retval | lowbit;
	  hashVal = hashVal >> 1;
   }
   return retval;
}




//hash.h

int Hash (char *key);
// create the primary hash value from a string

int MakeAddress (char *key, int depth);
 // get the primary hash, reverse the bits
 // return an address of depth bits

//iobuffer.cpp

#include "c:\tc\bin\hash\iobuffer.h"
#include <string.h>
#define TRUE 1
#define FALSE 0

IOBuffer::IOBuffer (int maxBytes)
// construct with a maximum of maxFields
{
	Init (maxBytes);
}

IOBuffer & IOBuffer :: operator = (const IOBuffer & buffer)
{
	if (MaxBytes < buffer . BufferSize) return *this; // fails
	Initialized = buffer . Initialized;
	BufferSize = buffer . BufferSize;
	memcpy (Buffer, buffer.Buffer, buffer . BufferSize);
	NextByte = buffer . NextByte;
	Packing = Packing;
	return *this;
}

void IOBuffer::Clear ()
// clear field values from buffer
{
	NextByte = 0;
	Packing = TRUE;
}

void IOBuffer::Print (ostream & stream) const
{
	stream<<"MaxBytes "<<MaxBytes<<" BufferSize "<<BufferSize;
}

int IOBuffer::Init (int maxBytes)
{
	Initialized = FALSE;
	if (maxBytes < 0) maxBytes = 0;
	MaxBytes = maxBytes;
	Buffer = new char[MaxBytes];
	BufferSize = 0;
	Clear ();
	return 1;
}
int IOBuffer::DRead (istream & stream, int recref)
// read specified record
{
	stream . seekg (recref, ios::beg);
	if (stream . tellg () != recref) return -1;
	return Read (stream);
}

int IOBuffer::DWrite (ostream & stream, int recref) const
// write specified record
{
	stream . seekp (recref, ios::beg);
	if (stream . tellp () != recref) return -1;
	return Write (stream);
}

static const char * headerStr = "IOBuffer";
static const int headerSize = strlen (headerStr);

int IOBuffer::ReadHeader (istream & stream) 
{
	char str[9];
	stream . seekg (0, ios::beg);
	stream . read (str, headerSize);
	if (! stream . good ()) return -1;
	if (strncmp (str, headerStr, headerSize)==0) return headerSize;
	else return -1;
}

int IOBuffer::WriteHeader (ostream & stream) const
{
	stream . seekp (0, ios::beg);
	stream . write (headerStr, headerSize);
	if (! stream . good ()) return -1;
	return headerSize;
}

//iobuffer.h

#include<iostream.h>
class IOBuffer{
   public:
      IOBuffer (int maxBytes = 1000); // a maximum of maxBytes
      IOBuffer & operator = (const IOBuffer &);
      virtual void Clear (); // clear fields from buffer
      virtual int Pack (const void * field, int size = -1) = 0;
	 // set the value of the next field of the buffer;
      virtual int Unpack (void * field, int maxbytes = -1) = 0;
	 // extract the value of the next field of the buffer
      virtual void Print (ostream &) const;
      int Init (int maxBytes);
	 // the read and write methods return the addreess of the record
	 // sequential read and write operations
      virtual int Read (istream &) = 0;
	 // read a buffer from the stream
      virtual int Write (ostream &) const = 0;
	 // write a buffer to the stream
      // these are the direct access read and write operations
      virtual int DRead (istream &, int recref);
	 // read specified record
      virtual int DWrite (ostream &, int recref) const;
      // these header operations return the size of the header
      virtual int ReadHeader (istream &);
      virtual int WriteHeader (ostream &) const;
   protected:
      int Initialized; // TRUE if buffer is initialized
      char* Buffer;   // character array to hold field values
      int BufferSize; // sum of the sizes of packed fields
      int MaxBytes;   // maximum number of characters in the buffer
      int NextByte;   // index of next byte to be packed/unpacked
      int Packing;    // TRUE if in packing maod, FALSE, if unpacking
};


//length.cpp

#include "c:\tc\bin\hash\length.h"
LengthFieldBuffer :: LengthFieldBuffer (int maxBytes)
 // construct with a maximum of maxFields
: VariableLengthBuffer (maxBytes)
{
	Init ();
}

void LengthFieldBuffer :: Clear ()
// clear fields from buffer
{
	VariableLengthBuffer::Clear ();
}

int LengthFieldBuffer :: Pack (const void* field, int size)
// set the value of the next field of the buffer;
// if size = -1 (default) use strlen(str) as length of field
// return number of bytes packed, -1 if error
{
	short len; // length of string to be packed
	if (size >= 0) len = size;
	else len = strlen ((char *) field);
	int start = NextByte; // first character to be packed
	NextByte += (len + sizeof(len));
	if (NextByte > MaxBytes) return -1;
	memcpy (&Buffer[start], &len, sizeof(len));
	memcpy (&Buffer[start+sizeof(len)], field, len);
	BufferSize = NextByte;
	return len;
}

int LengthFieldBuffer :: Unpack (void* field, int maxBytes)
// extract the value of the next field of the buffer
// return the number of bytes extracted, -1 if error
{
	short len; // length of packed string
	if (NextByte >= BufferSize) return -1; // no more fields
	int start = NextByte; // first character to be unpacked
	memcpy (&len, &Buffer[start], sizeof(len));
	if (maxBytes != -1 && len > maxBytes) return -1; // field too long
	NextByte += len + sizeof(len);
	if (NextByte > BufferSize) return -1;
	memcpy (field, &Buffer[start+sizeof(len)], len);
	if (maxBytes > len || maxBytes == -1)
		((char *)field) [len] = 0; // zero termination for string
	return len;
}

void LengthFieldBuffer :: Print (ostream & stream) const
{
	stream << "Buffer has characters "<<MaxBytes<<" and Buffer Size "<<BufferSize<<endl;
}

// Protected members

int LengthFieldBuffer :: Init ()
 // construct with a maximum of maxFields
{
	Initialized = TRUE;
	Clear ();
	return TRUE;
}


//length.h

#include "C:\tc\bin\hash\varlen.cpp"

class LengthFieldBuffer : public VariableLengthBuffer
// class that supports length plus value fields
{
   public:
     LengthFieldBuffer ( int maxBytes = 1000 );
	// construct with maximum of maxFields
	// construct with fields of specific size
     LengthFieldBuffer (const LengthFieldBuffer & buffer)
       : VariableLengthBuffer (buffer) {}
     void Clear(); // clear fields from buffer
     int Pack (const void * field, int size = -1);
	// set the value of the next field of the buffer;
     int Unpack ( void * field, int maxBytes = -1);
	// extract the value of the next field of the buffer
     void Print ( ostream &) const;
     int Init ();
};



//main.cpp


#include<stdio.h>
#include<conio.h>
#include<process.h>
#include "c:\tc\bin\hash\student.cpp"
#include <iostream.h>
#include<string.h>
#include"c:\tc\bin\hash\draw.cpp"
#include<graphics.h>
#include<ctype.h>
#include<math.h>
#include<stdlib.h>
#include<dos.h>


Directory Dir (4);
int collapsetrue=0,doublesizetrue=0;
void deleterecord (char *myfile,char *m);
void deleterecord1 (char *myfile,char *m);

char Branch[5][4] = {	"ISE",
			"CSE",
			"ECE",
			"IT"  };


char *toupp(void)
{
     char *string = " ";// "this is a string";

     cin >>string;
     for (int i=0; i<strlen(string); i++)
     {
	string[i] = toupper(string[i]);
     }
     return string;
}

char *toupp1(char * string)
{
     for (int i=0; i<strlen(string); i++)
     {
	string[i] = toupper(string[i]);
     }
     return string;
}


void append(char *myfile)
{
     int x=17,y=4;

     Student s;
     char *Branch1,*branch;
     clrscr();

     box(2,1,77,25);
     gotoxy(4,y-1);
     cout<<"                                                                ";
     gotoxy(x,y);


     gotoxy(x,y+1);
     cout <<"            ENTER THE  DATA FOR THE STUDENT :          ";
     gotoxy(x,y+2);


     gotoxy(x,y+4);
     cout << "   REG-NO      : ";
     gotoxy(x,y+6);
     cout << "   LAST NAME   : ";
     gotoxy(x,y+8);
     cout << "   FIRST NAME  : ";
     gotoxy(x,y+10);
     cout << "   ADDRESS     : ";
     gotoxy(x,y+12);
     cout << "   SEMESTER    : ";
     gotoxy(x,y+14);
     cout << "   BRANCH      : ";
     gotoxy(x,y+16);
     cout << "   COLLEGE     : ";
     gotoxy(x,y+18);


     gotoxy(4,y-1);
     cout<<"                                                                ";

     gotoxy(x+20,y+4);
     strcpy(s.URN , toupp());

     while(strlen(s.URN) != 10 )
     {
	 gotoxy(x+18,y+20);
	 textcolor(WHITE+BLINK);
	 cprintf("URN MUST BE 10 CHARACTERS");

	 gotoxy(x+20,y+4);
	 cout <<"                                 ";
	 gotoxy(x+20,y+4);
	 strcpy(s.URN,toupp());
	 gotoxy(x+18,y+20);
	 cout <<"                         ";
	 textcolor(15);

     }
     int res = Dir.Search(s.URN);
     if(res!=-1) {
	 gotoxy(x+18,y+20);
	 textcolor(WHITE+BLINK);
	 cprintf("REG-NO ALREADY EXISTS!");
	 textcolor(15);
	 getch();
	  return;
     }

     fflush(stdin);
     fflush(stdout);
     gotoxy(x+20,y+6);
     gets(s.Lname);

     gotoxy(x+20,y+8);
     gets(s.Fname);

     gotoxy(x+20,y+10);
     gets(s.Address);

     gotoxy(x+20,y+12);
     gets(s.Semester);

     int s1 = atoi(s.Semester);

     while( s1 <= 0 || s1 > 8 || s1=='\0' )
     {
	 gotoxy(x+18,y+20);
	 textcolor(WHITE+BLINK);
	 cprintf("ENTER SEMESTER 1 - 8 !");
	 gotoxy(x+20, y+12);
	 cout<<"                                ";
	 gotoxy(x+20,y+12);
	 gets(s.Semester);
	 s1 = atoi(s.Semester);
	 gotoxy(x+18,y+20);
	 cout <<"                                 ";
	 textcolor(15);
     }


     while(1) {
	 gotoxy(x+20,y+14);
	 textcolor(15);
	 gets(Branch1);
	 strcpy(s.Branch,toupp1(Branch1));

	 if(!((strcmp(s.Branch,"CSE"))&&(strcmp(s.Branch,"ISE"))&&(strcmp(s.Branch,"ECE"))&&(strcmp(s.Branch,"IT"))&&(strcmp(s.Branch,'\0'))))
	 {
	    gotoxy(x+18,y+20);
	 /*if(strcmp(s.Branch,"ISE")==0 || strcmp(s.Branch,"CSE")==0
		 || strcmp(s.Branch,"ELE")==0|| strcmp(s.Branch,"MECH")==0 || strcmp(s.Branch,"TELE")==0  || strcmp(s.Branch,"ARCH")==0 || strcmp(s.Branch,"IP")==0   ||  strcmp(s.Branch,"IT")==0 || strcmp(s.Branch,"ELEC")==0 || strcmp(s.Branch,"CHEM")==0)*/

	     cout<<"                       ";
	     break;
	 }
	 else
	 {

	     gotoxy(x+18,y+20);
	     textcolor(WHITE+BLINK);
	     cprintf("INVALID BRANCH!!");
	 }
	 gotoxy(x+20,y+14);
	 cout<<"                               ";
     }
     gotoxy(x+20,y+16);
     gets(s.College);
     /*while(s.College=="")
     {
	gotoxy(x+18,y+20);
	textcolor(WHITE+BLINK);
	cprintf("INCOMPLETE DATA");
	gotoxy(x+20, y+16);
	cout<<"                                ";
	gotoxy(x+20,y+16);
	gets(s.College);
	gotoxy(x+18,y+20);
	cout <<"                                 ";
	textcolor(15);
     }
       */
     gotoxy(32,22);
     textcolor(WHITE + BLINK);
     cprintf("UPDATED SUCCESSFULLY");
     textcolor(WHITE);
     int recaddr=s.Append(myfile);
     Dir.Insert(s.URN,recaddr);
     getch();
     if(doublesizetrue)
     {
		clrscr();
     cout<<"The Directory Has Doubled\n";
     doublesizetrue=0;
     Dir.Print(cout);
     return;
    }
}



void modify(char * myfile)
{
   Student s;
   cout << "\n\t\t ENTER THE REG-NO OF THE RECORD TO BE MODIFIED : ";
  strcpy(s.URN,toupp());
  int addr=Dir.Search(s.URN);
  if(addr==-1){
  gotoxy(20,15);
      textcolor(WHITE + BLINK);
      cprintf("THE RECORD DOES NOT EXIST");
      textcolor(WHITE);
   getch();
   return;
  }
  clrscr();
  cout << "\n\t\tTHE CURRENT CONTENTS OF THE RECORD IS : ";
  DelimFieldBuffer :: SetDefaultDelim('|');
  DelimFieldBuffer Buff;
  ifstream file(myfile,ios::in);

  Buff.DRead(file,addr);
  s.Unpack(Buff);
  s.Print(cout);
  getch();
  clrscr();

  cout <<"\n";

  cout 	<<"    1. FNAME"<<endl
	<<"    2. LNAME"<<endl
	<<"    3. ADDRESS"<<endl
	<<"    4. SEMESTER"<<endl
	<<"    5. BRANCH"<<endl
	<<"    6. COLLEGE"<<endl
	<<"    0. IGNORE"<<endl;
  fflush(stdin);
  cout <<endl<< "\n\t\tENTER THE FIELD NUMBER TO BE MODIFIED : ";
  int choice;
  cin >> choice;
  char oldurn[11];
  strcpy(oldurn,s.URN);
  cout <<endl;

  switch(choice){

    case 0:return;


    case 1: cout << " \n\t\t   Enter the new First Name: ";
	  gets(s.Fname);
	  break;

    case 2: cout << "\n\t\t    Enter the new Last Name: ";
	  gets(s.Lname);
	  break;

    case 3: cout << "\n\t\t    Enter the new Address: ";
	  gets(s.Address);
	  break;

    case 4: cout << "\n\t\t    Enter the new semester: ";
	  gets(s.Semester);
	   int x,y;
	  int s1 = atoi(s.Semester);

	    while( s1 <= 0 || s1 > 8 || s1=='\0'){
		 gotoxy(x+10,y+18);
		 textcolor(WHITE + BLINK);
		 cprintf("INVALID SEMESTER!");
		 gotoxy(x+22, y+10);
		 cout <<"                                 ";
		 gotoxy(x+22, y+10);
		 gets(s.Semester);
		 s1 = atoi(s.Semester);
		 gotoxy(x+10,y+18);
		 cout <<"                                 ";
		 textcolor(15);
	    }

	  break;

    case 5: cout << "\n\t\t    Enter the new Branch: ";

	  char *Branch1;
	    while(1) {
		gotoxy(x+22,y+10);
		cout<<"                        ";
		textcolor(15);
		gotoxy(x+22,y+10);
		gets(Branch1);
		strcpy(s.Branch,toupp1(Branch1));

		if(!((strcmp(s.Branch,'\0'))&&(strcmp(s.Branch,"CSE"))&&(strcmp(s.Branch,"ISE"))&&(strcmp(s.Branch,"ECE"))&&(strcmp(s.Branch,"IT"))))  {
			gotoxy(x+12,y+20);
			cout<<"                         ";
			gotoxy(x+12,y+20);
			break;
		getch();
		}


		 else{
		    gotoxy(x+12,y+20);
		    textcolor(WHITE+BLINK);
		    cprintf("INVALID BRANCH!!");
		}
	      }
	       break;

	     case 6 : cout << "\n\t\t    Enter the new College: ";
	  gets(s.College);
	  break;

    default:    gotoxy(x+12,y+20);
		textcolor(WHITE + BLINK);
		cprintf("INVALID CHOICE");
		textcolor(WHITE);
		return;
  }
   s.Pack(Buff);
   int newaddr=s.Append(myfile);
   fstream ofile(myfile,ios::in|ios::out);
   ofile.seekp(addr,ios::beg);
   Dir.Remove(oldurn);
   ofile.write("*",1);
   gotoxy(28,20);
     textcolor(WHITE + BLINK);
     cprintf("UPDATED SUCCESSFULLY");
     textcolor(WHITE);

   Dir.Insert(s.URN,newaddr);
   ofile.close();
}


void display(char *myfile)
{
   Student s;
   cout << "\n\t\tENTER THE REG-NO OF THE RECORD TO BE DISPLAYED : ";
   strcpy(s.URN,toupp());
   int addr;
   if((addr = Dir.Search(s.URN))==-1){
      gotoxy(20,15);
      textcolor(WHITE + BLINK);
      cprintf("RECORD NOT FOUND!");
      textcolor(WHITE);
      getch();
      return;
   }
   clrscr();
   DelimFieldBuffer :: SetDefaultDelim('|');
   DelimFieldBuffer Buff;
   fstream file(myfile,ios::in);
   Buff.DRead(file,addr);
   s.Unpack(Buff);
   clrscr();
   //gotoxy(10,3);
   //cout <<"\n\n\t\tTHE CONTENTS OF THE FILE ARE : ";
   s.Print(cout);
   box(2,1,77,25);
   file.close();
}

void deleterecord (char *myfile)
{
    Student s;
    cout << "\n\tENTER THE REG-NO OF THE RECORD TO BE DELETED : ";
    strcpy(s.URN,toupp());
    int addr=Dir.Search(s.URN);
    if(addr==-1){
      gotoxy(20,15);
      textcolor(WHITE + BLINK);

      cprintf("THE RECORD DOES NOT EXIST");
      textcolor(WHITE);
      getch();return;
    }
   fstream ofile(myfile,ios::in|ios::out);
   ofile.seekp(addr,ios::beg);
   ofile.write("*",1);
   ofile.close();
   Dir.Remove(s.URN);
   //Dir.Print(cout);
   cout << "\n\n----- RECORD DELETED SUCCESSFULLY --------- ";

   cout<<" \n\npress any key to see the directory collapse\n";
	 getch();
    if(collapsetrue)
  {         collapsetrue=0;
	cout<<"The Directory has collapsed.";
  }
   else
	cout<<"The Directory has not collapsed.";
   cout<<"\n\n\nThe Directory information...\n";
   Dir.Print(cout);
   getch();


}


void loaddir(char *myfile)
{
  Student s;
  fstream ifile(myfile,ios::in);
  DelimFieldBuffer :: SetDefaultDelim('|');
  DelimFieldBuffer Buff;
  int recaddr;
  while(1){
      if((recaddr=Buff.Read(ifile))==-1)break;
       s.Unpack(Buff);
       Dir.Insert(s.URN,recaddr);
     }
    ifile.close();
}

void updatefile(char *myfile)
{
   fstream ifile(myfile,ios::in);
   fstream ofile("demo4.bak",ios::out);
   char ch;
   while(1){
       ifile >> ch;
       if(ifile.fail()) break;
       ofile << ch;
  }
  ifile.close();
  ofile.close();
  ifile.open("demo4.bak",ios::in);
  ofile.open(myfile,ios::out);
  DelimFieldBuffer :: SetDefaultDelim('|');
  DelimFieldBuffer Buff;
  while(1){
      if(Buff.Read(ifile)==-1)break;
       if(Buff.getfirstchar()=='*')continue;
       else
	Buff.Write(ofile);
     }
    ifile.close();
    ofile.close();
}



void displayall(char *myfile)
{
   int x=22,y=7,i=1;
   DelimFieldBuffer :: SetDefaultDelim('|');
   DelimFieldBuffer Buff;
   fstream file(myfile,ios::in);
   Student s;
   while(1){
      clrscr();

      int res = Buff.Read(file);
      if(res==-1){
	 file.close();
	 return;
      }
      if( Buff.getfirstchar() == '*') continue;
	 s.Unpack(Buff);
	 box(2,1,77,25);
	 gotoxy(x+5,4);
	 textcolor(WHITE);
	 cprintf("STUDENT DATABASE DETAILS");
	 textcolor(15);
	 gotoxy(66,1); cout<<"RECORD : ";
	 textcolor(WHITE);
	 cprintf("#");cout<<i++;
	 textcolor(WHITE);
	 gotoxy(x,y);
	 cprintf("REG-NO       :");

	 gotoxy(x,y+2);
	 cprintf("FIRST NAME   :");

	 gotoxy(x,y+4);
	 cprintf("LAST NAME    :");

	 gotoxy(x,y+6);
	 cprintf("SEMESTER     :");

	 gotoxy(x,y+8);
	 cprintf("BRANCH       :");

	 gotoxy(x,y+10);
	 cprintf("COLLEGE      :");

	 gotoxy(x,y+12);
	 cprintf("ADDRESS    :");

	 textcolor(15);
	 gotoxy(x+20,y);
	 cout << s.URN;

	 gotoxy(x+20,y+2);
	 cout << s.Fname;

	 gotoxy(x+20,y+4);
	 cout << s.Lname;

	 gotoxy(x+20,y+6);
	 cout << s.Semester;

	 gotoxy(x+20,y+8);
	 cout << s.Branch;

	 gotoxy(x+20,y+10);
	 cout << s.College<<endl;

	 gotoxy(x+20,y+12);
	 cout <<s.Address<<endl;
	 getch();
   }
}

void Open()
{      // setbkcolor();
	setcolor(WHITE);
	rectangle(4,3,638,478);
	rectangle(6,4,645,485);
	line(4,60,638,60);
	line(4,438,638,438);
	setcolor(15);
		settextjustify(CENTER_TEXT,CENTER_TEXT);
	settextstyle(7,0,4);
	outtextxy(300,27,"    R.N.S.I.T ");
	settextstyle(3,0,2);
	outtextxy(300,90, " PROJECT ON ");
	outtextxy(300,120, " FILE STRUCTURES ");
	setcolor(WHITE);
	settextstyle(1,0,6);
	outtextxy(300,250," HASHING ");
	setcolor(15);
	settextstyle(2,0,6);
	outtextxy(300,400, "IMPLEMENTED BY ");
	settextstyle(2,0,7);
	outtextxy(150,457,"   DEEPIKA S ");
	outtextxy(460,457,"   DEEPTI MADHU ");
	getch();
	cleardevice();
}


int main()
{
   clrscr();
   int x=20,y=2;
   int flag=0;
   int result;
   remove("hash.bkt");
   remove("hash.dir");

   result = Dir . Create ("hash");
   char *myfile="DEMO4.txt";
   loaddir(myfile);

     int gdriver =  DETECT, gmode;

	initgraph(&gdriver,&gmode,"c:\\tc\\bgi");

	Open();
	cleardevice();
	//getch();
	closegraph();

   while(1){

     getch();
     clrscr();
     box(1,1,78,24);

     textcolor(15);
     gotoxy(x,y);

     gotoxy(x,y+1);
     textcolor(15);
     cprintf(" ----------------- MAIN MENU --------------------     ");
     textcolor(15);
     gotoxy(x,y+2);

     textcolor(15);
     gotoxy(x+5,y+3);
     cprintf("1.INSERT A RECORD");
     gotoxy(x+5,y+5);
     cprintf("2.MODIFY A RECORD");
     gotoxy(x+5,y+7);
     cprintf("3.DISPLAY A RECORD");
     gotoxy(x+5,y+9);
     cprintf("4.DISPLAY ALL RECORDS");
     gotoxy(x+5,y+11);
     cprintf("5.DELETE A RECORD");
     gotoxy(x+5,y+13);
     cprintf("6.DISPLAY DIRECTORY");
     gotoxy(x+5,y+15);
     cprintf("7.SPACE UTILIZATION");
     gotoxy(x+5,y+17);
     cprintf("8.EXIT");
     gotoxy(x,y+19);
     textcolor(15);
     cprintf("--------------------------------------");
     textcolor(WHITE);
     int choice;
     gotoxy(x+3,y+20);
     cprintf(" ENTER YOUR CHOICE :");
     cin >> choice;
     clrscr();
     switch(choice){
       case 1:
	   append(myfile);
	   break;
       case 2:
	   modify(myfile);flag=1;
	   break;
       case 3:
	   display(myfile);getch();
	   break;
       case 4:
	   displayall(myfile);getch();
	   break;
       case 5:
	   deleterecord(myfile);flag=1;
	   break;
       case 6:
	   Dir.Print(cout);
	   break;
	   case 7:
			//Dir.spaceutil(myfile);
			 if(flag)
		  updatefile(myfile);  //update the file if any delletions or updations are made
		  Dir.spaceutil(myfile);
			getch();
			break;
       case 8:
	   cout << "Updating the file"<<endl;
	   if(flag)
	     updatefile(myfile);
	   exit(0);
       default:
		 cout <<"\n\n\n\n\n\n\n\n\n\t\t\t INVALID CHOICE";
	     break;

     }
  }
}




void deleterecord1 (char *myfile,char *m)
{
	 Student s;
	 strcpy(s.URN,m);
	 int addr=Dir.Search(s.URN);
	 if(addr==-1){
	  // cout << "THE RECORD DOES NOT EXIST"<< endl;
	 //  getch();
	 return;
	 }
	fstream ofile(myfile,ios::in|ios::out);
	ofile.seekp(addr,ios::beg);
	ofile.write("*",1);
	ofile.close();
	Dir.Remove(s.URN);

}


//student.cpp

#include<iostream.h>
#include "c:\tc\bin\hash\student.h"
#define TRUE 1
#define FALSE 0

Student :: Student () { Clear();}

void Student :: Clear()
{  // Set each field to an empty string
   URN[0] = 0;Lname[0] = 0; Fname[0] = 0; Address[0] = 0;
   Semester[0] = 0; Branch[0] = 0; College[0] = 0;
}

int Student :: Pack (IOBuffer & Buffer) const
{
   int numBytes;
   Buffer.Clear();
   numBytes = Buffer.Pack(URN);
   if (numBytes == -1) return FALSE;
   numBytes = Buffer.Pack(Lname);
   if (numBytes == -1) return FALSE;
   numBytes = Buffer.Pack(Fname);
   if (numBytes == -1) return FALSE;
   numBytes = Buffer.Pack(Address);
   if (numBytes == -1) return FALSE;
   numBytes = Buffer.Pack(Semester);
   if (numBytes == -1) return FALSE;
   numBytes = Buffer.Pack(Branch);
   if (numBytes == -1) return FALSE;
   numBytes = Buffer.Pack(College);
   if (numBytes == -1) return FALSE;
   return TRUE;
}

int Student :: Unpack(IOBuffer & Buffer)//all the feilds are packed to the buffer

{
   Clear();
   int numBytes;
   numBytes = Buffer.Unpack(URN);
   if (numBytes == -1) return FALSE;
   URN[numBytes] = 0;
   numBytes = Buffer.Unpack(Lname);
   if (numBytes == -1) return FALSE;
   Lname[numBytes] = 0;
   numBytes = Buffer.Unpack(Fname);
   if (numBytes == -1) return FALSE;
   Fname[numBytes] = 0;
   numBytes = Buffer.Unpack(Address);
   if (numBytes == -1) return FALSE;
   Address[numBytes] = 0;
   numBytes = Buffer.Unpack(Semester);
   if (numBytes == -1) return FALSE;
   Semester[numBytes] = 0;
   numBytes = Buffer.Unpack(Branch);
   if (numBytes == -1) return FALSE;
   Branch[numBytes] = 0;
   numBytes = Buffer.Unpack(College);
   if (numBytes == -1) return FALSE;
   College[numBytes] = 0;
   return TRUE;
}

int Student :: InitBuffer (DelimFieldBuffer & Buffer)
{
   return TRUE;
}
void Student :: Print(ostream & stream, char * label) const
{
    gotoxy(3,4);
    if (label == 0) stream << "Stutent:";
    else stream << label;
    gotoxy(3,5);
    stream << "Reg-no    : " << URN ;
    gotoxy(3,6);
    stream << "Last Name : " << Lname;
    gotoxy(3,7);
    stream << "First Name: " << Fname;
    gotoxy(3,8);
    stream << "Address   : " << Address;
    gotoxy(3,9);
    stream << "Semester  : " << Semester;
    gotoxy(3,10);
    stream << "Branch    : " << Branch;
    gotoxy(3,11);
    stream << "College   : " << College;
    stream<<flush;
}

int Student :: Search(char *myfile)
{
     fstream file(myfile,ios::in);
     Student s1;

     while(1){
	 DelimFieldBuffer :: SetDefaultDelim('|');
	 DelimFieldBuffer Buff;
	 int add=Buff.Read(file);
	 if (add==-1) return 0;
	 s1.Unpack(Buff);
	 if( strcmpi(s1.URN,URN)==0)
	   return add+1;
    }
}

int Student :: Append(char *myfile)
{
    DelimFieldBuffer :: SetDefaultDelim('|');
    DelimFieldBuffer Buff;
    Student :: InitBuffer(Buff);
    Pack(Buff);
    fstream file(myfile,ios::in|ios::out);
    file.seekp(0,ios::end);
    file.seekg(0,ios::end);
    int recaddr=Buff.Write(file);
    file.close();
    return recaddr;
}



//student.h

#include "C:\tc\bin\hash\delim.cpp"
class Student{
  public:
     char URN[13];char Lname[11];char Fname[21];char Address[50];
     char Semester[2];char Branch[6];char College[11];

     Student();
     static int InitBuffer (DelimFieldBuffer &);
     void Clear ();
     int Unpack (IOBuffer &);
     int Pack (IOBuffer &) const;
     void Print (ostream &, char *label = 0) const;
     int Search(char *);
     int Append(char *);
};

//textind.cpp

a#include "C:\tc\bin\hash\textind.h"
#include<string.h>

TextIndex:: TextIndex (int maxKeys, int unique)
	: NumKeys (0), Keys(0), RecAddrs(0) //member initialization list
{Init (maxKeys, unique);}

TextIndex :: ~TextIndex ()
{delete Keys; delete RecAddrs;}

int TextIndex :: Insert (const char * key, int recAddr)
{
	int i;
	int index = Find (key);
	if (Unique && index >= 0) return 0; // key already in
	if (NumKeys == MaxKeys) return 0; //no room for another key
	for (i = NumKeys-1; i >= 0; i--)
	{
		if (strcmp(key, Keys[i])>0) break; // insert into location i+1
		Keys[i+1] = Keys[i];
		RecAddrs[i+1] = RecAddrs[i];
	}
	Keys[i+1] = strdup(key);
	RecAddrs[i+1] = recAddr;
	NumKeys ++;
	return 1;
}

int TextIndex :: Remove (const char * key)
{
	int index = Find (key);
	if (index < 0) return 0; // key not in index
	for (int i = index; i < NumKeys; i++)
	{
		Keys[i] = Keys[i+1];
		RecAddrs[i] = RecAddrs[i+1];
	}
	NumKeys --;
	return 1;
}

int TextIndex :: Search (const char * key) const
{
	int index = Find (key);
	if (index < 0) return index;
	return RecAddrs[index];
}

void TextIndex :: Print (ostream & stream) const
{
	stream << "Text Index max keys "<<MaxKeys
			<<" num keys "<<NumKeys<<endl;
	for (int i = 0; i<NumKeys; i++)
		stream <<"\tKey["<<i<<"] "<<Keys[i]
			<<" RecAddr "<<RecAddrs[i]<<endl;
}

int TextIndex :: Find (const char * key) const
{
	for (int i = 0; i < NumKeys; i++)
		if (strcmp(Keys[i], key)==0) return i;// key found
		else if (strcmp(Keys[i], key)>0) return -1;// not found
	return -1;// not found
}

int TextIndex :: Init (int maxKeys, int unique)
{
	Unique = unique != 0;
	if (maxKeys <= 0)
	{
		MaxKeys = 0;
		return 0;
	}
	MaxKeys = maxKeys;
	Keys = new char *[maxKeys];
	RecAddrs = new int [maxKeys];
	return 1;
}


//textind.h

#include<iostream.h>
class TextIndex
{
   public:
      TextIndex (int maxKeys = 100, int unique = 1);
      ~TextIndex ();
      int Insert (const char *key, int recAddr);
      int Remove (const char *key);
      int Search (const char *key)const;
      void Print (ostream &) const;
   protected:
      int MaxKeys;
      int NumKeys;
      char **Keys;
      int * RecAddrs;
      int Find (const char *key) const;
      int Init (int macKeys, int unique);
      int Unique; // if true, each key value must be unique
    friend class TextIndexBuffer;
};
//tindbuff.cpp

#include "C:\tc\bin\hash\tindbuff.h"

TextIndexBuffer::TextIndexBuffer (int keySize, int maxKeys,int extraFields, int extraSpace)
: FixedFieldBuffer (1+2*maxKeys+extraFields,
		sizeof(int)+maxKeys*keySize+maxKeys*sizeof(int) + extraSpace)
	// buffer fields consist of
	//    numKeys, actual number of keys
	//    Keys [maxKeys] key fields size = maxKeys * keySize
	//    RecAddrs [maxKeys] record address fields size = maxKeys*sizeof(int)
{
	MaxKeys = maxKeys;
	KeySize = keySize;
	AddField (sizeof(int));
	for (int i = 0; i < maxKeys; i++)
	{
		AddField (KeySize);
		AddField (sizeof(int));
	}
	Dummy = new char[keySize+1];
}

int TextIndexBuffer::Pack
	(const TextIndex & index)
{
	int result;
	Clear ();
	result = FixedFieldBuffer::Pack (&index.NumKeys);
	for (int i = 0; i < index.NumKeys; i++)
	{// note only pack the actual keys and recaddrs
		result = result && FixedFieldBuffer::Pack (index.Keys[i]);
		result = result && FixedFieldBuffer::Pack (&index.RecAddrs[i]);
	}
	for (int j = 0; j<index.MaxKeys-index.NumKeys; j++)
	{// pack dummy values for other fields
		result = result && FixedFieldBuffer::Pack (Dummy);
		result = result && FixedFieldBuffer::Pack (Dummy);
	}
	return result;
}

int TextIndexBuffer::Unpack
	(TextIndex & index)
{
	int result;
	result = FixedFieldBuffer::Unpack (&index.NumKeys);
	for (int i = 0; i < index.NumKeys; i++)
	{// note only pack the actual keys and recaddrs
		index.Keys[i] = new char[KeySize]; // just to be safe
		result = result && FixedFieldBuffer::Unpack (index.Keys[i]);
		result = result && FixedFieldBuffer::Unpack (&index.RecAddrs[i]);
	}
	for (int j = 0; j<index.MaxKeys-index.NumKeys; j++)
	{// pack dummy values for other fields
		result = result && FixedFieldBuffer::Unpack (Dummy);
		result = result && FixedFieldBuffer::Unpack (Dummy);
	}
	return result;
}

void TextIndexBuffer:: Print (ostream & stream) const
{
	stream <<"TextIndexBuffer: KeySize "<<KeySize
		<<" MaxKeys "<<MaxKeys<<endl;
	FixedFieldBuffer :: Print (stream);
}

//tindbuff.h

#include "C:\tc\bin\hash\fixfld.cpp"
#include "C:\tc\bin\hash\textind.cpp"

class TextIndexBuffer:public FixedFieldBuffer
{
   public:
     TextIndexBuffer (int keySize, int maxKeys = 100, int extraFields = 0,
			int extraSize = 0);
     int Pack (const TextIndex &);
     int Unpack (TextIndex &);
     void Print (ostream &) const;
   protected:
     int MaxKeys;
     int KeySize;
     char *Dummy;
};


//varlen.cpp

#include "C:\tc\bin\hash\varlen.h"
#include<iostream.h>
#include<fstream.h>

VariableLengthBuffer :: VariableLengthBuffer ( int maxBytes) : IOBuffer (maxBytes)
{
   Init();
}

void VariableLengthBuffer :: Clear()
{
   IOBuffer :: Clear();
}

int VariableLengthBuffer :: Read (istream & stream)
// write the number of bytes in the buffer field definitions
// the record length is represented by an unsigned short value
{
   if (stream.eof())
      return -1;
	int recaddr = stream.tellg();
   Clear();
   stream. getline(Buffer,MaxBytes,'#');
   BufferSize = strlen(Buffer);
   if (!stream.good ()){
      stream.clear();
      return -1;
   }
   return recaddr;
}

int VariableLengthBuffer :: Write (ostream & stream) const
{
    int recaddr = stream.tellp();
    stream.write(Buffer, BufferSize);
    stream.write("#",1);

    if(!stream.good())
      return -1;
    return recaddr;
}

void VariableLengthBuffer :: Print (ostream & stream) const
{
    IOBuffer :: Print(stream);
}

int VariableLengthBuffer :: Init ()
{
   Clear ();
   return TRUE;
}

//varlen.h

#include "C:\tc\bin\hash\iobuffer.cpp"
#include<iostream.h>

class VariableLengthBuffer : public IOBuffer
{
   public:
      VariableLengthBuffer (int maxBytes=200);

      VariableLengthBuffer (const VariableLengthBuffer & buffer)
	:IOBuffer(buffer){}
      void Clear ();
      int Read (istream &);
      int Write (ostream &) const;
      int PackDelimeted (void *, int);
      void Print (ostream &) const;
      int SizeOfBuffer () const;
      int Init ();
};

//tstdel.cpp

// tsthash.cpp

#include "C:\hashing\direct.cpp"
#include <iostream.h>

main ()
{
	int result;
	Directory Dir (4);
	result = Dir . Open ("tsthash");
	if (result == 0)
	{
		cout<<"Please create tsthash.dir and tsthash.bkt"<<endl;
		return 0;
	}
/*	char * keys[]={"bill", "lee", "pauline", "alan", "julie",
		"mike", "elizabeth", "mark", "ashley", "peter",
		"joan", "john", "charles", "mary", "emily"};*/
	char * keys[]={"1pi098is001", "1pi98is002", "1pi98is003", "1pi98is004", "1pi98is005",
		"1pi98is006", "1pi98is007", "1pi98is008", "1pi98is010", "1pi98is011",
		"1pi98is012", "1pi98is013", "1pi98is014", "1pi98is015", "1pi98is009"};

	const int numkeys = 15;
	for (int i = 0; i<numkeys; i++)
	{
		cout <<keys[i]<<" "<<(void*)Hash(keys[i])
			<<" "<<(void*)MakeAddress(keys[i],16)<<endl;
		result = Dir . Remove (keys[i]);
		if (result == 0) cout << "remove for "<<keys[i]<<" failed"<<endl;
		Dir . Print (cout);
	}
	return 1;
}

