#include "C:\tc\hash\buffile.cpp"
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

