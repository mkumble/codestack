#include<iostream.h>
#include "c:\tc\hash\student.h"
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


