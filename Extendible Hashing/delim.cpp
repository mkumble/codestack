#include <string.h>
#include "c:\tc\HASH\delim.h"

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
