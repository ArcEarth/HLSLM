#include <iostream>
#include <vector>

//char elems[] = { 'x','y','z','w' };
char elems[] = { 'x','y','z','w' };
char* argelems[] = { "0","1","2","3" };
int n = std::size(elems);
char* macro = "_MAKE_SWS_";
int arr[4];

char name[5];

using namespace std;
// print arr[0-l)
void print(int l)
{
	cout << macro << '(';

	name[l] = '\0';

	for (int i = 0; i < l; i++)
	{
		name[i] = elems[arr[i]];
	}

	cout << name;

	for (int i = 0; i < l; i++)
	{
		cout << "," << argelems[arr[i]];
	}

	cout << ')' << endl; 
}

void gen_len(int cl, int l)
{
	for (int i = 0; i < n; i++)
	{
		arr[cl - 1] = i;
		if (cl > 1)
			gen_len(cl - 1, l);
		else
			print(l);
	}
}

int main(int argc, char** argv)
{
	if (argc >= 2)
		n = atoi(argv[1]);

	if (argc >= 3)
		macro = argv[2];

	for (int l = 1; l <= 4; l++)
	{
		gen_len(l, l);
	}
}