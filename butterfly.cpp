#include "butterfly.h"

class testfly{//holds unpacked butterfly matrices 
public : //only use for testing 
	int size; //width of square matrix 
	int depth;// the number of recursive levels
	bool transposed;
	bint * entries; // diagonal entries 
	testfly( butterfly b);
	//depth numb arrays 
	//each is width long 

};
testfly::testfly(butterfly b){
	testfly::size = b.size;
	testfly::depth = b.depth;
	testfly::entries = (bint *)malloc(size * size);
	testfly::transposed = false;
	for (int i = 0; i < size *size; i++) testfly::entries[i] = 0;
	for (int i = 0; i < size; i++){
		testfly::entries[i * size] = b.entries[0][i];
		for (int j = 1; j < b.depth; j++){\
			//blrag 
			testfly::entries[i * size] *= b.entries[j][i];
		}
		
	}
	return;
}