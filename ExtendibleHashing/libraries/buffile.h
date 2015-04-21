#include"C:\tc\hash\length.cpp"
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


