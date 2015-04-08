#include "C:\tc\hash\iobuffer.cpp"
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
