#include "main.hpp"

#include <cstdio>
#include <cstring>
using namespace std;


#include "ring.hpp"
#include "rings_thread.hpp"
#include "grid.hpp"
#include "system.hpp"


int main()
{
	sys _sys;

	printf("Hello World!\n");
	if (_sys.main() == true)
	{
		printf("Byw World!\n");
		return 0;
	}
	else
	{
		printf("Program terminated unsuccessfully\n");
		return 1;
	}
}


int old_main2()
{
	size_t i, j;
	#define THREADS 8
	rings_thread** myrings;
	bool finish;
	
	myrings = reinterpret_cast<rings_thread**>(malloc(sizeof(rings_thread) * THREADS));

	i = 0;	
	while (i < THREADS)
	{
		myrings[i] = new rings_thread(4000000);
		i++;
	}
	
	j = 0;
	finish = false;
	while (finish != false)
	{
		i = 0;
		finish = true;
		while (i < THREADS)
		{
			if (myrings[i]->finished() == true)
			{
				printf("*");
			}
			else
			{
				printf(".");
				finish = false;
			}
			i++;
		}
		printf("\n");
	}

	i = 0;	

	while (i < THREADS)
	{
		printf("Thread %ld: Distribution is %lf\n", i, myrings[i]->getDistrib());
		delete myrings[i];
		i++;
	}
	free(myrings);
	
	return 0;
}


int old_main()
{
	grid myGrid(2, 2, 2, -1, -1, -1, 256, 256, 256);
	ring myRing(1000, 1.0);
	
	myGrid.init();
	myRing.initialize();

	size_t i, sizeI, j, sizeJ;
	charge* tmp;
	tmp = myRing.getCharge();

	i = j = 0; sizeI = myGrid.size(); sizeJ = myRing.size();

	while (j < sizeJ)
	{
		tmp[j].setLoad(400);
		j++;
	}

	while (i < sizeI)
	{
		charge_c C;
		double load;

		C = myGrid.getCharge(i); load = 0;
		j = 0;
		while (j < sizeJ)
		{
//			printf("%0.5f\n", tmp[j].difference(C).magnitude());
			load += tmp[j].getLoad() / tmp[j].difference(C).magnitude();
			j++;
		}
//		printf("Load of %ld (%0.5f, %0.5f, %0.5f): %0.5f\n", i, C.getX(), C.getY(), C.getZ(), load);
		C.setLoad(load);
		i++;
	}

	FILE* ofile;
	ofile = fopen("/home/crypt/Desktop/output.raw", "wb");
	fwrite(myGrid.getArray(), sizeof(charge_s), sizeI, ofile);

	charge_s* C;
	C = myGrid.getArray();
	i = 0;
	while (i < sizeI)
	{
//		printf("Load of %ld: %0.5f\n", i, C[i].getLoad());
		i++;
	}

	return 0;
}
