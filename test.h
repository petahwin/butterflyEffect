#include "butterfly.h"

class testfly{//holds unpacked butterfly matrices 
public:
	int size; //width of square matrix 
	int depth;// the number of recursive levels
	bool transposed;
	bint * entries; // diagonal entries 
	testfly::testfly(butterfly b){
		testfly::size = b.size;
		testfly::depth = b.depth;
		testfly::entries = (bint *)malloc(size * size * sizeof(bint));
		testfly::transposed = false;
		for (int i = 0; i < size *size; i++) testfly::entries[i] = 0;
		for (int i = 0; i < size; i++){
			testfly::entries[i * size] = b.entries[i];
			for (int j = 1; j < b.depth; j++){
				testfly::entries[i * size] *= b.entries[j * size + i];
			}

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
	//depth numb arrays 
	//each is width long 

};
matrix middleTestmulti(testfly a, matrix m, testfly b);