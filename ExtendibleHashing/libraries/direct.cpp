// direct.cpp

#include "c:\tc\hash\global.h">
#include "c:\tc\hash\direct.h"
#include<math.h>
#include "c:\tc\hash\hash.cpp"
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
{textcolor(YELLOW);
	cprintf("Directory Depth %d size %d\n",Depth,NumCells);
	for (int i = 0; i<NumCells; i++)
	{
		cprintf("bucket address is %d in bucket file\n",BucketAddr[i]);
		LoadBucket (PrintBucket, BucketAddr[i]);
		PrintBucket->Print(stream);
		cout << endl << "Press a key..."<<endl;
		getch();
	}
	stream <<"end directorFy"<<endl;
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
{textcolor(YELLOW);
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

