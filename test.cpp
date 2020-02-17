#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <vector>
using namespace std;

#include "grid.hpp"
#include "ring.hpp"
#include "prompt.hpp"


#define message(x) msg.push_back(x); printf("%s\n", x)

int old_main();

int main(int argc, char* argv[]);
void automated_tests(vector<const char*>& msg);
void interactive_tests(vector<const char*>& msg);
void print_header(const char* str);
void print_footer(const char* str);

bool ring_automated_test_1(vector<const char*>& msg);
bool prompt_interactive_test_1(vector<const char*>& msg);
bool ring_interactive_test_1(vector<const char*>& msg);


int main(int argc, char* argv[])
{
	size_t i;
	size_t size;
	vector<const char*> msg;

	setvbuf(stdout, 0, _IONBF, 0);
	// return old_main();

	automated_tests(msg);
	//interactive_tests(msg);

	printf("All tests finished\n\n");

	print_header("Test message log");
	i = 0;
	size = msg.size();
	while (i < size)
	{
		printf("%s\n", msg[i]);
		i++;
	}
	print_footer("Test message log");
}

void automated_tests(vector<const char*>& msg)
{
	ring_automated_test_1(msg);
}

void interactive_tests(vector<const char*>& msg)
{
	prompt_interactive_test_1(msg);
	ring_interactive_test_1(msg);
}

void print_header(const char* str)
{
	size_t i;
	size_t len;
	char* buffer;

	len = strlen(str);
	buffer = new char[len + 1];

	i = 0;
	while (i < len)
	{
		buffer[i] = '=';
		i++;
	}
	buffer[i] = 0;
	printf("%s\n%s\n", str, buffer);

	delete[] buffer;
}

void print_footer(const char* str)
{
	size_t i;
	size_t len;
	char* buffer;

	len = strlen(str);
	buffer = new char[len + 1];

	i = 0;
	while (i < len)
	{
		buffer[i] = '-';
		i++;
	}
	buffer[i] = 0;
	printf("%s\n\n", buffer);

	delete[] buffer;
}

bool ring_automated_test_1(vector<const char*>& msg)
{
	const char*const desc = "Ring Automated Test 1: Ring and charge rotations, balance and pole restrictions";
	print_header(desc);

	const size_t charges = 10; /* 10 charges */
	const size_t times = 10000000; /* 10 million iterations */

	ring* myRing;
	size_t i, j;
	bool ret;

	double min;
	double max;
	double rotmin;
	double rotmax;
	double rad;
	size_t neg;
	size_t pos;

	myRing = new ring(charges, 100.0);
	ret = true;

	printf("Running (%ld charges, %ld times)...", charges, times);
	min = 0; max = 0;
	rotmin = 30; rotmax = 30;
	neg = 0; pos = 0;
	j = 0;
	while (j < times)
	{
		myRing->initialize(rand());

		i = 0;
		while (i < charges)
		{
			rad = atan(myRing->getCharge(i).getY() / myRing->getCharge(i).getX()) * (180 / M_PI);
			if (rad < min) min = rad;
			if (rad > max) max = rad;

			if (myRing->getCharge(i).getX() > 0) pos++;
			if (myRing->getCharge(i).getX() < 0) neg++;

			i++;
		}

		myRing->rotate(M_PI * (30.0 / 180.0));

		i = 0;
		while (i < charges)
		{
			rad = atan(myRing->getCharge(i).getZ() / myRing->getCharge(i).getX()) * (180 / M_PI);
			if (rad < rotmin) rotmin = rad;
			if (rad > rotmax) rotmax = rad;

			i++;
		}

		j++;
	}

	printf("done\n");
	printf("Charge positions :: Min: %0.3f | Max: %0.3f\n", min, max);
	printf("Ring charge rotations :: Min: %0.3f | Max: %0.3f\n", rotmin, rotmax);
	printf("Charge ring sides :: Left: %ld | Right: %ld\n", neg, pos);

	if (abs(static_cast<long>(pos - neg)) > 1) /* 1 in case its an odd number of charges */
	{
		message("ring_automated_test_1(): Unbalanced amount of charges");
		ret = false;
	}

	if ((rotmin < 29.999) || (rotmax > 30.001))
	{
		message("ring_automated_test_1(): Bad ring rotation detected");
		ret = false;
	}
	delete myRing;

	print_footer(desc);
	return ret;
}


bool ring_interactive_test_1(vector<const char*>& msg)
{
	const char*const desc = "Ring Interactive Test 1: Ring rotation and poles restriction";
	print_header(desc);

	const size_t charges = 10;

	ring* myRing;
	size_t i;
	bool ret;

	double rad;
	char* buffer;
	size_t len;

	myRing = new ring(charges, 100.0);
	buffer = new char[BUFSIZ];

	printf("Initializing ring...");
	myRing->initialize();
	printf("done\n");

	printf("Current ring charge coordinates\n");
	printf("+---------+---------+---------+\n");
	printf("|    X    |    Y    |    Z    |\n");
	printf("+---------+---------+---------+\n");
	i = 0;
	while (i < charges)
	{
		printf("|");

		sprintf(buffer, "%0.4f", myRing->getCharge(i).getX());
		len = strlen(buffer);
		while (len < 9)
		{
			putchar(' ');
			len++;
		}
		printf("%s|", buffer);

		sprintf(buffer, "%0.4f", myRing->getCharge(i).getY());
		len = strlen(buffer);
		while (len < 9)
		{
			putchar(' ');
			len++;
		}
		printf("%s|", buffer);

		sprintf(buffer, "%0.4f", myRing->getCharge(i).getZ());
		len = strlen(buffer);
		while (len < 9)
		{
			putchar(' ');
			len++;
		}
		printf("%s|", buffer);

		rad = atan(myRing->getCharge(i).getY() / myRing->getCharge(i).getX()) * (180 / M_PI);
		printf(" Charge placement degrees: %0.3f", rad);

		if ((rad < -75.001) || (rad > 75.001))
		{
			//message("ring_interactive_test_1(): Bad charge position");

			//print_footer(desc);
			//delete myRing;
			//delete[] buffer;
			//return false;
		}

		printf("\n");
		i++;
	}
	printf("+---------+---------+---------+\n");
	printf("\n");

	printf("Rotating ring 30 degrees (M_PI * (30.0 / 180.0))...");
	myRing->rotate(M_PI * (30.0 / 180.0));
	printf("done\n");

	printf("Rotated ring charge coordinates\n");
	printf("+---------+---------+---------+\n");
	printf("|    X    |    Y    |    Z    |\n");
	printf("+---------+---------+---------+\n");
	i = 0;
	while (i < charges)
	{
		printf("|");

		sprintf(buffer, "%0.4f", myRing->getCharge(i).getX());
		len = strlen(buffer);
		while (len < 9)
		{
			putchar(' ');
			len++;
		}
		printf("%s|", buffer);

		sprintf(buffer, "%0.4f", myRing->getCharge(i).getY());
		len = strlen(buffer);
		while (len < 9)
		{
			putchar(' ');
			len++;
		}
		printf("%s|", buffer);

		sprintf(buffer, "%0.4f", myRing->getCharge(i).getZ());
		len = strlen(buffer);
		while (len < 9)
		{
			putchar(' ');
			len++;
		}
		printf("%s|", buffer);

		rad = atan(myRing->getCharge(i).getZ() / myRing->getCharge(i).getX()) * (180 / M_PI);
		printf(" Ring rotation degrees: %0.3f", rad);

		if ((rad > 30.001) || (rad < 29.999))
		{
			message("ring_interactive_test_1(): Bad ring rotation");

			print_footer(desc);
			delete myRing;
			delete[] buffer;
			return false;
		}

		printf("\n");
		i++;
	}
	printf("+---------+---------+---------+\n");
	printf("\n");

	if (prompt_yn("Are the charges coordinates after rotation correct?") == true)
	{
		ret = true;
	}
	else
	{
		message("ring_interactive_test_1(): User noted rotation is incorrect");
		ret = false;
	}

	delete myRing;
	delete[] buffer;

	print_footer(desc);
	return ret;
}



bool prompt_interactive_test_1(vector<const char*>& msg)
{
	const char*const desc = "Prompt Interactive Test 1: Input test and validation";
	print_header(desc);

	bool ret;
	char* buffer;

	short _short;
	unsigned short _unsignedshort;
	int _int;
	unsigned int _unsignedint;
	long _long;
	unsigned long _unsignedlong;
	float _float;
	double _double;
	long double _longdouble;
	char* _string;

	buffer = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ));

	ret = true;

	_short = prompt_short("Enter \"short\" number");
	sprintf(buffer, "The number you entered is %hd, is this correct?", _short);
	if (prompt_yn(buffer) != true)
	{
		message("prompt_interactive_test_1(): User answered \"short\" input as incorrect");
		ret = false;
	}

	_unsignedshort = prompt_unsignedshort("Enter \"unsigned short\" number");
	sprintf(buffer, "The number you entered is %hu, is this correct?", _unsignedshort);
	if (prompt_yn(buffer) != true)
	{
		message("prompt_interactive_test_1(): User answered \"unsigned short\" input as incorrect");
		ret = false;
	}

	_int = prompt_int("Enter \"int\" number");
	sprintf(buffer, "The number you entered is %d, is this correct?", _int);
	if (prompt_yn(buffer) != true)
	{
		message("prompt_interactive_test_1(): User answered \"int\" input as incorrect");
		ret = false;
	}

	_unsignedint = prompt_unsignedint("Enter \"unsigned int\" number");
	sprintf(buffer, "The number you entered is %u, is this correct?", _unsignedint);
	if (prompt_yn(buffer) != true)
	{
		message("prompt_interactive_test_1(): User answered \"unsigned int\" input as incorrect");
		ret = false;
	}

	_long = prompt_long("Enter \"long\" number");
	sprintf(buffer, "The number you entered is %ld, is this correct?", _long);
	if (prompt_yn(buffer) != true)
	{
		message("prompt_interactive_test_1(): User answered \"long\" input as incorrect");
		ret = false;
	}

	_unsignedlong = prompt_unsignedlong("Enter \"unsigned long\" number");
	sprintf(buffer, "The number you entered is %lu, is this correct?", _unsignedlong);
	if (prompt_yn(buffer) != true)
	{
		message("prompt_interactive_test_1(): User answered \"unsigned long\" input as incorrect");
		ret = false;
	}

	_float = prompt_float("Enter \"float\" number");
	sprintf(buffer, "The number you entered is %f, is this correct?", _float);
	if (prompt_yn(buffer) != true)
	{
		message("prompt_interactive_test_1(): User answered \"float\" input as incorrect");
		ret = false;
	}

	_double = prompt_double("Enter \"double\" number");
	sprintf(buffer, "The number you entered is %lf, is this correct?", _double);
	if (prompt_yn(buffer) != true)
	{
		message("prompt_interactive_test_1(): User answered \"double\" input as incorrect");
		ret = false;
	}

	_longdouble = prompt_longdouble("Enter \"long double\" number");
	sprintf(buffer, "The number you entered is %Lf, is this correct?", _longdouble);
	if (prompt_yn(buffer) != true)
	{
		message("prompt_interactive_test_1(): User answered \"long double\" input as incorrect");
		ret = false;
	}

	_string = prompt_string("Enter a \"string\"");
	sprintf(buffer, "The string you entered is \"%s\", is this correct?", _string); /* NOTE: Possibility of buffer overflow (but not on stack) */
	if (prompt_yn(buffer) != true)
	{
		message("prompt_interactive_test_1(): User answered \"string\" input as incorrect");
		ret = false;
	}

	free(_string);
	free(buffer);

	print_footer(desc);
	return ret;
}
/*
	Enter "short" number: 50000
	The number you entered is -15536, is this correct?: no
	prompt_interactive_test_1(): User answered "short" input as incorrect
	Enter "unsigned short" number: 80000
	The number you entered is 14464, is this correct?: nope
	prompt_interactive_test_1(): User answered "unsigned short" input as incorrect
	Enter "int" number: 4000000000
	The number you entered is -294967296, is this correct?: nope
	prompt_interactive_test_1(): User answered "int" input as incorrect
	Enter "unsigned int" number: 8000000000
	The number you entered is 3705032704, is this correct?: no
	prompt_interactive_test_1(): User answered "unsigned int" input as incorrect
	Enter "long" number: 10000000000000000000
	The number you entered is 9223372036854775807, is this correct?: nope
	prompt_interactive_test_1(): User answered "long" input as incorrect
	Enter "unsigned long" number: 50000000000000000000
	The number you entered is 18446744073709551615, is this correct?: nope
	prompt_interactive_test_1(): User answered "unsigned long" input as incorrect
	Enter "float" number: 1234567890
	The number you entered is 1234567936.000000, is this correct?: nope
	prompt_interactive_test_1(): User answered "float" input as incorrect
	Enter "double" number: 111122223333444455556666777788889999
	The number you entered is 111122223333444463764500077525073920.000000, is this correct?: no
	prompt_interactive_test_1(): User answered "double" input as incorrect
	Enter "long double" number: 1111111111111111111111111111111111111111
	The number you entered is 1111111111111111111111671891239418986496.000000, is this correct?: no
	prompt_interactive_test_1(): User answered "long double" input as incorrect
	Enter a "string": This is my string
	The string you entered is "This is my string", is this correct?: finally!
*/



































#define GRID_SIZE 256
/* 128 MB memory required old main to generate sample file for paraview */
/* Generated file is also 128 MB large */
int old_main()
{
	FILE* ofile;

	int x, y, z;
	double*** val;

	double mean;
	double max;
	double min;

	mean = 0;
	min = 100000;
	max = 0;

	setvbuf(stdout, 0, _IONBF, 0);

	if ((val = new double**[GRID_SIZE]) == 0)
	{
		printf("failed\n");
		return 1;
	}

	x = 0;
	printf("Allocating memory & Calculating energy (raw)");
	while (x < GRID_SIZE)
	{
		if ((x % 8) == 0) printf(".");
		val[x] = new double*[GRID_SIZE];
		y = 0;
		while (y < GRID_SIZE)
		{
			val[x][y] = new double[GRID_SIZE];
			z = 0;
			while (z < GRID_SIZE)
			{
				val[x][y][z] = 0;
				val[x][y][z] += GRID_SIZE / (1 + sqrt(pow((GRID_SIZE * (1.0 / 4.0)) - x, 2) + pow((GRID_SIZE * (1.0 / 4.0)) - y, 2) + pow((GRID_SIZE * (1.0 / 4.0)) - z, 2)));
				val[x][y][z] += GRID_SIZE / (1 + sqrt(pow((GRID_SIZE * (3.0 / 4.0)) - x, 2) + pow((GRID_SIZE * (1.0 / 4.0)) - y, 2) + pow((GRID_SIZE * (1.0 / 4.0)) - z, 2)));
				val[x][y][z] += GRID_SIZE / (1 + sqrt(pow((GRID_SIZE * (1.0 / 4.0)) - x, 2) + pow((GRID_SIZE * (3.0 / 4.0)) - y, 2) + pow((GRID_SIZE * (1.0 / 4.0)) - z, 2)));
				val[x][y][z] += GRID_SIZE / (1 + sqrt(pow((GRID_SIZE * (3.0 / 4.0)) - x, 2) + pow((GRID_SIZE * (3.0 / 4.0)) - y, 2) + pow((GRID_SIZE * (1.0 / 4.0)) - z, 2)));
				val[x][y][z] += GRID_SIZE / (1 + sqrt(pow((GRID_SIZE * (1.0 / 4.0)) - x, 2) + pow((GRID_SIZE * (1.0 / 4.0)) - y, 2) + pow((GRID_SIZE * (3.0 / 4.0)) - z, 2)));
				val[x][y][z] += GRID_SIZE / (1 + sqrt(pow((GRID_SIZE * (3.0 / 4.0)) - x, 2) + pow((GRID_SIZE * (1.0 / 4.0)) - y, 2) + pow((GRID_SIZE * (3.0 / 4.0)) - z, 2)));
				val[x][y][z] += GRID_SIZE / (1 + sqrt(pow((GRID_SIZE * (1.0 / 4.0)) - x, 2) + pow((GRID_SIZE * (3.0 / 4.0)) - y, 2) + pow((GRID_SIZE * (3.0 / 4.0)) - z, 2)));
				val[x][y][z] += GRID_SIZE / (1 + sqrt(pow((GRID_SIZE * (3.0 / 4.0)) - x, 2) + pow((GRID_SIZE * (3.0 / 4.0)) - y, 2) + pow((GRID_SIZE * (3.0 / 4.0)) - z, 2)));
				val[x][y][z] += GRID_SIZE / (1 + sqrt(pow((GRID_SIZE * (2.0 / 4.0)) - x, 2) + pow((GRID_SIZE * (2.0 / 4.0)) - y, 2) + pow((GRID_SIZE * (2.0 / 4.0)) - z, 2)));

				mean += val[x][y][z];
				if (val[x][y][z] < min) min = val[x][y][z];
				if (val[x][y][z] > max) max = val[x][y][z];

				z++;
			}
			y++;
		}
		x++;
	}
	printf("done\n");

	printf("opening /home/crypt/Desktop/output.raw for binary output..."); 
	if ((ofile = fopen("/home/crypt/Desktop/output.raw", "wb")) == 0) /* Absolute path? */
	{
		printf("failed\n");
		return 1;
	}
	printf("done\n");

	x = 0;
	printf("Writing to file");
	while (x < GRID_SIZE)
	{
		if ((x % 8) == 0) printf(".");
		y = 0;
		while (y < GRID_SIZE)
		{
			fwrite(val[x][y], sizeof(double), GRID_SIZE, ofile);
			y++;
		}
		x++;
	}
	printf("done\n");

	printf("Mean: %lf\n", mean / GRID_SIZE / GRID_SIZE / GRID_SIZE);
	printf("Minimum: %lf\n", min);
	printf("Maximum: %lf\n", max);

	fclose(ofile);

	return 0;
	
}

#define CHARGES 1024 * 1024 * 32
#define RADIUS 10
int old_main2() /* from main.cpp */
{
	return 0;

	ring R;
	R.charges() = CHARGES;
	R.radius() = RADIUS;
	size_t i, j;

	char* buffer;
	buffer = reinterpret_cast<char*>(malloc(BUFSIZ));

	setvbuf(stdout, 0, _IONBF, 0);

	printf("Initializing...");
	R.initialize();
	printf("done\n");

/*
	printf("Before rotating\n");
	printf("X                Y                Z               \n");
	printf("================ ================ ================\n");

	i = 0;
	while (i < CHARGES)
	{
		sprintf(buffer, "%lf", R.getCharge()[i].getX());
		printf("%s", buffer);
		j = 17 - strlen(buffer);
		while (j > 0)
		{
			putchar(' ');
			j--;
		}

		sprintf(buffer, "%lf", R.getCharge()[i].getY());
		printf("%s", buffer);
		j = 17 - strlen(buffer);
		while (j > 0)
		{
			putchar(' ');
			j--;
		}

		sprintf(buffer, "%lf", R.getCharge()[i].getZ());
		printf("%s", buffer);
		j = 17 - strlen(buffer);
		while (j > 0)
		{
			putchar(' ');
			j--;
		}

		putchar('\n');
		i++;
	}
*/

	printf("Rotating...");
	R.rotate(M_PI * 2.0 * (1.0 / 6.0));
	printf("done\n");

/*
	printf("After rotating\n");
	printf("X                Y                Z               \n");
	printf("================ ================ ================\n");

	i = 0;
	while (i < CHARGES)
	{
		sprintf(buffer, "%lf", R.getCharge()[i].getX());
		printf("%s", buffer);
		j = 17 - strlen(buffer);
		while (j > 0)
		{
			putchar(' ');
			j--;
		}

		sprintf(buffer, "%lf", R.getCharge()[i].getY());
		printf("%s", buffer);
		j = 17 - strlen(buffer);
		while (j > 0)
		{
			putchar(' ');
			j--;
		}

		sprintf(buffer, "%lf", R.getCharge()[i].getZ());
		printf("%s", buffer);
		j = 17 - strlen(buffer);
		while (j > 0)
		{
			putchar(' ');
			j--;
		}

		putchar('\n');
		i++;
	}
*/

}
