#include <cstdlib>
#include <cstdio>
using namespace std;

int myfunc (int x)
{
	printf("myfunc(%d)\n", x);
	return x + 1;
}

int main()
{
	int (*y)(int);

	y = &myfunc;

	printf("Calling myfunc() returned %d\n", y(5));
	return 0;
}
