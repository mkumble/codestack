// tsthash.cpp

#include "C:\hashing\direct.cpp"
#include <iostream.h>

main ()
{
	int result;
	Directory Dir (4);
	result = Dir . Open ("tsthash");
	if (result == 0)
	{
		cout<<"Please create tsthash.dir and tsthash.bkt"<<endl;
		return 0;
	}
/*	char * keys[]={"bill", "lee", "pauline", "alan", "julie",
		"mike", "elizabeth", "mark", "ashley", "peter",
		"joan", "john", "charles", "mary", "emily"};*/
	char * keys[]={"1pi098is001", "1pi98is002", "1pi98is003", "1pi98is004", "1pi98is005",
		"1pi98is006", "1pi98is007", "1pi98is008", "1pi98is010", "1pi98is011",
		"1pi98is012", "1pi98is013", "1pi98is014", "1pi98is015", "1pi98is009"};

	const int numkeys = 15;
	for (int i = 0; i<numkeys; i++)
	{
		cout <<keys[i]<<" "<<(void*)Hash(keys[i])
			<<" "<<(void*)MakeAddress(keys[i],16)<<endl;
		result = Dir . Remove (keys[i]);
		if (result == 0) cout << "remove for "<<keys[i]<<" failed"<<endl;
		Dir . Print (cout);
	}
	return 1;
}