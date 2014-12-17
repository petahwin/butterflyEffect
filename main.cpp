#include <stdio.h>
#include <string.h>
#include "butterfly.h"
#include "test.h"


//depth 1 square are matricies 
//must be powers of 2 

int main(int argc, const char* argv[])
{
	int size = 64;
	int depth = 2;
	srand(1234);//time(NULL));

	printf("\nHello World \n\n");
	Butterfly a(size, depth);
	Butterfly b(size, depth);

	printf("a is\n");
//	a.printEntries();
	printf("b is\n\n");
//	b.printEntries();


	testfly ta( a);
	testfly tb( b);


	printf("a test is\n");
//	ta.print();
	printf("b test is\n");
//	tb.print();

	Matrix m(size, true);
	printf("\nm is\n");
	//m.printMatrix();
	//printf("a test transposed is\n\n");
	ta.transpose();
	//ta.print();

	Matrix serial = middleTestmulti(ta, m, tb);
	
	a.transpose();
	
	 Matrix packed = middlebmulti(a, m, b);
	serial.percenterror(serial, packed);
	printf("packed is \n");
//	 packed.printMatrix();
	 printf("serial is \n\n");
// serial.printMatrix();

	 Matrix leftpack = leftbmulti(a, m);
	 Matrix leftlose = leftTestmulti(ta, m);
	 serial.percenterror(leftlose, leftpack);
	 printf("packed is \n");
	// 	leftpack.printMatrix();
	 printf("serial is \n");
	// 	 leftlose.printMatrix();
	 double vals[64];
	 for (int i = 0; i < 64; i++){
		 vals[i] = i + 1;
	 }
	 leftBVect(a, vals ,64 );
	 for (int i = 0; i < 64; i++){
		 printf("%d   ", vals[i]  ); 
	 }
	return 0;
}
void LUsolver(){
	int size = 64;
	int depth = 2;
	Matrix M(size, true);

	//make vector x and B
	Butterfly A1(size, depth);
	Butterfly A2(size, depth);

	Matrix condtioned = middlebmulti(A1, M, A2);


}