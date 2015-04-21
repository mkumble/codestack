#include "C:\tc\hash\varlen.h"
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