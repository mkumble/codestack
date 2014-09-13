#include<iostream.h>
class TextIndex
{
   public:
      TextIndex (int maxKeys = 100, int unique = 1);
      ~TextIndex ();
      int Insert (const char *key, int recAddr);
      int Remove (const char *key);
      int Search (const char *key)const;
      void Print (ostream &) const;
   protected:
      int MaxKeys;
      int NumKeys;
      char **Keys;
      int * RecAddrs;
      int Find (const char *key) const;
      int Init (int macKeys, int unique);
      int Unique; // if true, each key value must be unique
    friend class TextIndexBuffer;
};