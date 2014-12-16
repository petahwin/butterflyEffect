#ifndef TEST_H
#define TEST 1

#include "butterfly.h"
void setSubTestFly(bint * write, int rowsize, int size, bint * read);
double * matrixmulti(matrix b, matrix m);

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

		
		testfly::transposed = false;

		
		if (b.depth == 1){
			testfly::entries = (bint *)malloc(size * size * sizeof(bint));
			for (int i = 0; i < size *size; i++) testfly::entries[i] = 0;
			setSubTestFly(entries, size, size, b.entries);
			
		}
		else{ // do 2 case
			matrix old(size, false);
			double * oldbody = old.body;
			matrix add(size, false);
			setSubTestFly(old.body, size, size, b.entries);
			//wrtite a butterfly in the upper left
			setSubTestFly(add.body, size, size / 2, b.entries + size);
			//now lower right
			setSubTestFly(add.body + (size + 1 )* size /2 , size, size / 2, b.entries + size + size /2 );
			//multiply together stick in old
		//	printf("\nadd is  \n");
	//		add.printMatrix();
	//		printf("\nold is  \n");
	//		old.printMatrix();
			entries = matrixmulti(add, old);
			
			//copy out
			
	//		free(add.body);
	//	free(old.body);

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




#endif