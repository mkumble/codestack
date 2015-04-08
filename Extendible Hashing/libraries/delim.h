#include "C:\tc\hash\direct.cpp"

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

