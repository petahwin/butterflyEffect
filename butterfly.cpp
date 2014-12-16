#ifndef BUTTERFLY_CPP
#define BUTTERFLY_CPP 1

#include "butterfly.h"

void dumb(void){
	printf("dumb got called\n");
}

matrix middlebmulti(butterfly a, matrix m, butterfly b){
	//compute chunks of C 
	matrix C(m.n, true);
	//by a i mean m 
	//0 c1 = a11 + a12 +a21 +a22    1 c2 = a11 - a12 +a21 - a22
	//2 c3 = a11 + a12 -a21 - a22	3 c4 = a11 - a12 - a21 +a22
	static int vals[4][3]; //c m
	vals[0][0] = 1; 
	vals[0][1] = 1; // c1  m 3 
	vals[0][2] = 1;

	vals[1][0] = -1;
	vals[1][1] = 1;
	vals[1][2] = -1;

	vals[2][0] = 1;
	vals[2][1] = -1;
	vals[2][2] = -1;

	vals[3][0] = -1;
	vals[3][1] = -1;
	vals[3][2] = 1;

	int index[4];
	index[0] = 0;
	index[1] = m.n/2;
	index[2] = m.n * m.n / 2;
	index[3] = m.n * m.n / 2 + m.n / 2;

	for (int block = 0; block < 4; block++){//  split matrix into blocks 
		
	//	printf("inddex:%d   block:%d\n", index[block], block);
		for (int row = 0; row < m.n / 2; row++){//itterate down to next row
			for (int col = 0; col < m.n / 2; col++){//itterate accross the row 
			//set intital value 
		//		printf("setting %d to %d:%g\n", index[block] + row* m.n + col,
	//				 row * m.n + col, m.body[ row * m.n + col]);
				C.body[ index[block] + row* m.n + col] =
					m.body[ row * m.n + col];
				for (int j = 1; j < 4; j++){
					
					C.body[index[block] + row* m.n + col] +=
						m.body[index[j] + row* m.n + col] * vals[block][j - 1];
	//				printf("adding %d to %d for %g\n", index[block] + row* m.n + col,
	//					index[j] + row* m.n + col, C.body[index[block] + row* m.n + col]);
				}

			}
		}
	}
	//printf(" c 111is \n");
	//C.printMatrix();
	//r0 r1 are diagonals of a 
	//d1 =  r0  * c1     d2 =  r0 * c2
	//d3 =  r1	* c3	d4 =  r1  * c4
	for (int row = 0; row < m.n; row++){
		for (int col = 0; col < m.n; col++){
			C.body[row* m.n + col] *= a.entries[row];
		}
	}


	//printf(" c 222 is \n");
	//C.printMatrix();
	//f1 = d1*  r'0 			f2 = d2 * r'1  	
	//f3 = d3 * r'0 			f4 = d4 * r'1  
	//devide by 2 
	for (int row = 0; row < m.n; row++){
		for (int col = 0; col < m.n; col++){
			C.body[row* m.n + col] *= b.entries[col] * .5;
		}
	}
	//printf(" c 333 is \n");
	//C.printMatrix();
	return C;
}


#endif