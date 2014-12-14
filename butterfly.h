#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <random>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


#define bint double //butterfly entires can be ints or doubles
 struct matrix {//n by n matrix 
	int n;
	double * body;
};
 void printMatrix(matrix m){
	 for (int i = 0; i < m.n; i++){
		 for (int j = 0; j < m.n; i++){
			 printf("%f    ", m.body[i*m.n + j]);
		 }
		 printf("\n");
	 }
 }
 //assume m is correct 
 void percenterror(matrix m, matrix A){
	 assert(m.n == A.n);
	 double total = 0.0;
	 double totalerr = 0.0;
	 for (int i = 0; i < m.n; i++){
		 for (int j = 0; j < m.n; i++){
			 total += m.body[i*m.n + j];
			 totalerr += abs(m.body[i*m.n + j] + m.body[i*m.n + j]);
		 }
		
	 }
	 double percenterr = totalerr / total;
	
	 printf("total error was %f  percent error was %f\n", totalerr, percenterr);
	 return;
 }
 void initMatrix(matrix * m, int n){
	 m->n = n;
	 m->body = (double *)malloc(sizeof(double)* n* n);
	 srand(time(NULL));
	 for (int i = 0; i < n* n; i++){
		 m->body[i] = rand();
	 }
 }


class butterfly{
public :
	int size; //width of square matrix 
	int depth;// the number of recursive levels
	bool transposed;
	bint ** entries; // diagonal entries 


	butterfly::butterfly(int insize, int indepth){
		butterfly::size = size;
		butterfly::depth = depth;
		butterfly::entries = (bint **)malloc(depth * sizeof(bint *));
		butterfly::transposed = false;
		srand(time(NULL));
		int r = rand();
		for (int i = 0; i < depth; i++){
			entries[i] = (bint *)malloc(size * sizeof(bint));
			for (int j = 0; j < size; j++){
				entries[i][j] = (bint)rand();
			}
		}
		return;
	}
	//very cheep function for transposing  
	void butterfly::transpose(void){
		butterfly::transposed = !butterfly::transposed;
	}
		//depth numb arrays 
		//each is width long 
};



/**/
//butterfly entries could be doubles or ints 


//faster combination of left  and right multi 
struct matrix middlebmulti( butterfly a, struct matrix m, butterfly b);
// modifies matrix m to be the result of b * m 
int leftbmulti(butterfly b, struct matrix m);
// modifies matrix m to be the result of m * b
int rightbmulti(struct matrix m, butterfly b);
 
