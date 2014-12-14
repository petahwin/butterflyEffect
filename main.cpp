#include <stdio.h>
#include <string.h>
#include "butterfly.h"
//#include "serialtest.cpp"



int main(int argc, const char* argv[])
{
	int size = 16;

	printf("\nHello World \n\n");
	butterfly a(size, 1);
	butterfly b(size, 1);

//	testfly ta(size, 1, a);
//	testfly tb(size, 1, b);

	struct matrix m;
	initMatrix(&m, size);

//	struct 	matrix serial = middleTestmulti(ta, m, tb);
	
	a.transpose();

	struct matrix packed = middlebmulti(a, m, b);
	//percenterror(serial, packed);

	return 0;
}