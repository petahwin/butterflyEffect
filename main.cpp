#include <stdio.h>
#include <string.h>
#include "butterfly.h"
//#include "serialtest.cpp"
#include "test.h"



int main(int argc, const char* argv[])
{
	int size = 4;
	//dumb();
	printf("\nHello World \n\n");
	butterfly a(size, 1);
	butterfly b(size, 1);

	testfly ta( a);
	testfly tb( b);
	printf("a is\n");
	a.printEntries();
	printf("b is\n");
	b.printEntries();

	printf("a test is\n");
	ta.print();
	printf("b test is\n");
	tb.print();

	matrix m(size, true);
	
	m.printMatrix();
	matrix serial = middleTestmulti(ta, m, tb);
	
	a.transpose();
	
	 matrix packed = middlebmulti(a, m, b);
	serial.percenterror(serial, packed);
	printf("packed is \n");
	 packed.printMatrix();
	 printf("serial is \n");
	 serial.printMatrix();
	return 0;
}