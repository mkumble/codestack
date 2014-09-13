#include "C:\tc\hash\varlen.cpp"

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


