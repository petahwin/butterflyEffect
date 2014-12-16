#ifndef BUTTERFLY_H
#define BUTTERFLY_H 1

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <random>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>



class matrix {//n by n matrix 
public:
	int n;
	double * body;

	void printMatrix(void){
		for (int i = 0; i < matrix::n; i++){
			for (int j = 0; j < matrix::n; j++){
				printf("%g    ", matrix::body[i*matrix::n + j]);
			}
			printf("\n");
		}
	}
	//assume m is correct 
	void matrix::percenterror(matrix m, matrix A){
		assert(m.n == A.n);
		double total = 0.0;
		double totalerr = 0.0;
		for (int i = 0; i < m.n; i++){
			for (int j = 0; j < m.n; j++){
				total += m.body[i*m.n + j];
				totalerr += abs(m.body[i*m.n + j] - A.body[i*m.n + j]);
			}

		}
		double percenterr = totalerr / total;

		printf("\ntotal error was %g  percent error was %g\n\n", totalerr, percenterr);
		return;
	}
	 matrix::matrix(int inn, bool randfill){
		matrix::n = inn;
		if (randfill == true){
			matrix::body = (double *)malloc(sizeof(double)* n* n);
			
		
			for (int i = 0; i < n* n; i++)
				matrix::body[i] = (double)rand();
			
		}
		else matrix::body = (double *)calloc(n* n, sizeof(double));
	}
};
#define bint double //butterfly entires can be ints or doubles
class butterfly{
public :
	int size; //width of square matrix 
	int depth;// the number of recursive levels
	bool transposed;
	bint * entries; // diagonal entries size * depth


	butterfly::butterfly(int insize, int indepth){
		butterfly::size = insize;
		butterfly::depth = indepth;
		butterfly::entries = (bint *)malloc(depth * size * sizeof(bint ));
		butterfly::transposed = false;
		
		int r = rand();
		for (int i = 0; i < indepth * insize; i++){
			entries[i] = (bint)rand();// / INT_MAX;
		}
		return;
	}
	//very cheep function for transposing  
	void butterfly::transpose(void){
		butterfly::transposed = !butterfly::transposed;
	}
	void butterfly::printEntries(void){
		for (int i = 0; i < size * depth; i++){
			printf("%g  ", entries[i]);
		}
	}
		//depth numb arrays 
		//each is width long 
};



/**/
//butterfly entries could be doubles or ints 


//faster combination of left  and right multi 
matrix middlebmulti(butterfly a, matrix m, butterfly b);
// modifies matrix m to be the result of b * m 
matrix leftbmulti(butterfly b,matrix m);

//not implimented yet
// modifies matrix m to be the result of m * b
int rightbmulti( matrix m, butterfly b);
 



#endif