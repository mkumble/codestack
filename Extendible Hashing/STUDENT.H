#include "C:\tc\hash\delim.cpp"
class Student{
  public:
     char URN[13];char Lname[11];char Fname[21];char Address[50];
     char Semester[2];char Branch[6];char College[11];

     Student();
     static int InitBuffer (DelimFieldBuffer &);
     void Clear ();
     int Unpack (IOBuffer &);
     int Pack (IOBuffer &) const;
     void Print (ostream &, char *label = 0) const;
     int Search(char *);
     int Append(char *);
};
