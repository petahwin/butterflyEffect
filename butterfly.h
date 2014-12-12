#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <random>
#include <time.h>
#include <stdlib.h>

#define bint double //butterfly entires can be ints or doubles
struct martix {//n by n matrix 
	int n;
	double * body;
};
class butterfly{
	int size; //width of square matrix 
	int depth;// the number of recursive levels
	bool transposed;
	bint ** entries; // diagonal entries 
	butterfly(int size, int depth);
		//depth numb arrays 
		//each is width long 
};


//should entires be doubles or ints 

butterfly::butterfly(int insize, int indepth){
	butterfly::size = size;
	butterfly::depth = depth;
	butterfly::entries = (bint **) malloc(depth * sizeof(bint *));
	butterfly::transposed = false;
	srand(time(NULL));
	int r = rand();
	for (int i = 0; i < depth; i++){
		entries[i] = (bint *)malloc(size * sizeof(bint));
		for (int j = 0; j < size; j++){
			entries[i][j] = (bint) rand();
		}
	}
	return;
}
// modifies matrix m to be the result of b * m 
int leftbmulti(butterfly b, struct matrix m);
// modifies matrix m to be the result of m * b
int rightbmulti(struct matrix m, butterfly b);
// transposes b  very cheep operation 
int bTranspose(butterfly b);