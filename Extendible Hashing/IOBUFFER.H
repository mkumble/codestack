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

