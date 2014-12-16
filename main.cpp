#include <stdio.h>
#include <string.h>
#include "butterfly.h"
//#include "serialtest.cpp"
#include "test.h"

//depth 1 square are matricies 
//must be powers of 2 

int main(int argc, const char* argv[])
{
	int size = 32;
	srand(1234);//time(NULL));
	//dumb();
	printf("\nHello World \n\n");
	butterfly a(size, 1);
	butterfly b(size, 1);

	testfly ta( a);
	testfly tb( b);
	printf("a is\n");
	a.printEntries();
	printf("b is\n\n");
	b.printEntries();

	printf("a test is\n");
	ta.print();
	printf("b test is\n");
	tb.print();

	matrix m(size, true);
	printf("\nm is\n");
	m.printMatrix();
	printf("a test transposed is\n\n");
	ta.transpose();
	ta.print();

	matrix serial = middleTestmulti(ta, m, tb);
	
	a.transpose();
	
	 matrix packed = middlebmulti(a, m, b);
	serial.percenterror(serial, packed);
	printf("packed is \n");
//	 packed.printMatrix();
	 printf("serial is \n\n");
//	 serial.printMatrix();

	 matrix leftpack = leftbmulti(a, m);
	 matrix leftlose = leftTestmulti(ta, m);
	 serial.percenterror(leftlose, leftpack);
	 printf("packed is \n");
	// 	leftpack.printMatrix();
	 printf("serial is \n");
	// 	 leftlose.printMatrix();


	return 0;
}