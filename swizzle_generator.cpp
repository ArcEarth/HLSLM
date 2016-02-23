#include <iostream>
#include <algorithm>
#include <numeric>

template <typename Iterator>
inline bool next_combination(Iterator first,
	Iterator k,
	Iterator last);

template <typename Iterator>
inline bool next_combination(const Iterator first, Iterator k, const Iterator last)
{
	/* Credits: Thomas Draper */
	// http://stackoverflow.com/a/5097100/8747
	if ((first == last) || (first == k) || (last == k))
		return false;
	Iterator itr1 = first;
	Iterator itr2 = last;
	++itr1;
	if (last == itr1)
		return false;
	itr1 = last;
	--itr1;
	itr1 = k;
	--itr2;
	while (first != itr1)
	{
		if (*--itr1 < *itr2)
		{
			Iterator j = k;
			while (!(*itr1 < *j)) ++j;
			std::iter_swap(itr1, j);
			++itr1;
			++j;
			itr2 = k;
			std::rotate(itr1, j, last);
			while (last != j)
			{
				++j;
				++itr2;
			}
			std::rotate(k, itr2, last);
			return true;
		}
	}
	std::rotate(first, k, last);
	return false;
}

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

	for (int l = 1; l <= n; l++)
	{
		//gen_len(l, l);
		std::iota(arr, arr + n, 0);

		// remove the identity
		if (l == n)
			next_permutation(arr, arr + l);

		do {
			do
			{
				print(l);
			} while (next_permutation(arr, arr + l));
		} while (next_combination(arr, arr + l, arr + n));
	}
}