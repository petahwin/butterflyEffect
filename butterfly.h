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

#define bint double //butterfly entires can be ints or doubles

struct Matrix {//n by n Matrix 
    int n;
    double * body;

    Matrix(int n_in, bool randfill = true);
    void printMatrix(void);

    // Static (class level) method
    // assume m is correct 
    static void percenterror(Matrix m, Matrix A);
};

struct Butterfly {
    int size; //width of square matrix 
    int depth;// the number of recursive levels
    bool transposed;
    bint * entries; // diagonal entries size * depth

    Butterfly(int insize, int indepth);

    //very cheep function for transposing  
    void transpose(void);

    void printEntries(void);
        // depth numb arrays 
        // each is width long 
};

/**/
//butterfly entries could be doubles or ints 


//faster combination of left  and right multi 
Matrix middlebmulti(Butterfly a, Matrix m, Butterfly b);
// modifies matrix m to be the result of b * m 
Matrix leftbmulti(Butterfly a, Matrix m);


void leftBVect(Butterfly a, double * vect, int size);
//not implimented yet
// modifies matrix m to be the result of m * b
int rightbmulti(Matrix m, Butterfly b);
 
;


#endif
