#ifndef TEST_H
#define TEST 1

#include "butterfly.h"

class testfly{//holds unpacked butterfly matrices 
public:
	int size; //width of square matrix 
	int depth;// the number of recursive levels
	bool transposed;
	bint * entries; // diagonal entries 
	testfly::testfly(butterfly b){    // r0    R1
		testfly::size = b.size;		//   R0		-R1
		testfly::depth = b.depth;
		//make matrix propigate with butterfly 
		//make second fill with butterfly 
		//multily together 
		//refuill old  multiply until done 

		testfly::entries = (bint *)malloc(size * size * sizeof(bint));
		testfly::transposed = false;
		for (int i = 0; i < size *size; i++) testfly::entries[i] = 0;
		for (int i = 0; i < size / 2; i++){  //upper left
			testfly::entries[i * size + i] = b.entries[i] * 1 / sqrt(2);
		}
		for (int i = 0; i < size / 2; i++){  //uper right
			testfly::entries[i * size + i + size / 2 ] = b.entries[i + size / 2] * 1 / sqrt(2);
		}
		for (int i = 0; i < size / 2; i++){  //lower left
			testfly::entries[i * size + i + size *size / 2] = b.entries[i ] * 1 / sqrt(2);
		}
		for (int i = 0; i < size / 2; i++){  //lower right
			testfly::entries[i * size + i + size *size / 2 + size / 2] = b.entries[i + size / 2] * -1 / sqrt(2);
		}
		
		return;
	}
	void testfly::print(void){
		for (int i = 0; i < size; i++){
			for (int j = 0; j < size; j++){
				printf("%g    ", entries[i*size + j]);
			}
			printf("\n");
		}
	}
	void testfly::transpose(void){
		bint temp = 0.0;
		for (int i = 0; i < size; i++){
			for (int j = i + 1; j < size; j++){
				temp = entries[i * size + j];
				entries[i * size + j] = entries[j * size + i];
				entries[j * size + i] = temp;
			}
		}
	}
	//depth numb arrays 
	//each is width long 

};
matrix middleTestmulti(testfly a, matrix m, testfly b);

matrix leftTestmulti(testfly b, matrix m);

matrix matrixmulti(matrix b, matrix m);

void setSubTestFly(bint * write, int rowsize, int size, bint * read);


#endif