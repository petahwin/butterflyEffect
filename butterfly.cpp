#ifndef BUTTERFLY_CPP
#define BUTTERFLY_CPP 1

#include "butterfly.h"
/*
const static class values{
public:
	static int vals[4][3]; //c m
	values::values(void){
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
	}
};
*/
void blockBmidd(bint * C, int bsize, int rowsize, bint * A, bint * M, bint * B){
	//C is the location in a rowsize * row size matrix where the block of bsize * bsize will start 
	//A B are the correct entries in 2 butterfly matrices to start this block
	//M is the start in 
	//compute chunks of C 
	// C must be initilized to a row size by row size matrix 
	assert(bsize <= rowsize);
	assert( rowsize % rowsize == 0);
	//by a i mean m 
	//0 c1 = a11 + a12 +a21 +a22    1 c2 = a11 - a12 +a21 - a22
	//2 c3 = a11 + a12 -a21 - a22	3 c4 = a11 - a12 - a21 +a22
	static int vals[4][3];
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
	/*
	int midindex[4];// index of each block in  C
	midindex[0] = 0;
	midindex[1] = bsize / 2;
	midindex[2] = bsize * bsize / 2;
	midindex[3] = bsize * bsize / 2 + bsize / 2;
	*/
	int bigindex[4];
	bigindex[0] = 0;  // index of each block in M and write 
	bigindex[1] = bsize / 2;
	bigindex[2] = rowsize * bsize / 2;
	bigindex[3] = rowsize * bsize / 2 + bsize / 2;

	for (int block = 0; block < 4; block++){//  split matrix into blocks 

		//	printf("inddex:%d   block:%d\n", index[block], block);
		for (int row = 0; row < bsize / 2; row++){//itterate down to next row
			for (int col = 0; col < bsize / 2; col++){//itterate accross the row 
				//set intital value by loading upper left corrner
				//		printf("setting %d to %d:%g\n", index[block] + row* m.n + col,
				//				 row * m.n + col, m.body[ row * m.n + col]);
				C[bigindex[block] + row* rowsize+ col] =
					M[row * rowsize + col];
				for (int j = 1; j < 4; j++){

					C[bigindex[block] + row* rowsize + col] +=
						M[bigindex[j] + row* rowsize + col] * vals[block][j - 1];
					//				printf("adding %d to %d for %g\n", index[block] + row* m.n + col,
					//					index[j] + row* m.n + col, C.body[index[block] + row* m.n + col]);
				}

			}
		}
	}

	for (int row = 0; row < bsize; row++){
		for (int col = 0; col < bsize; col++){
			C[row* rowsize + col] *= A[row];
		}
	}

	//f1 = d1*  r'0 			f2 = d2 * r'1  	
	//f3 = d3 * r'0 			f4 = d4 * r'1  
	//devide by 2 
	for (int row = 0; row < bsize; row++){
		for (int col = 0; col < bsize; col++){
			C[row* rowsize + col] *= B[col] * .5;
		}
	}
}




matrix middlebmulti(butterfly a, matrix m, butterfly b){
	//compute chunks of C 
	assert(a.size = m.n);
	assert(b.depth = a.depth);
	matrix C(m.n, true);
	//start at lowest depth and work outward
	if (a.depth == 2){
		matrix D(m.n, true);
		//upper left  a1   m11  b1
		blockBmidd(D.body, m.n/2, m.n, a.entries+ m.n, m.body, b.entries + m.n);
		//upper right   a1  m12   b2
		blockBmidd(D.body + m.n /2, m.n / 2, m.n, a.entries + m.n, m.body + m.n/2, b.entries + m.n /2 + m.n);
		//lower left  a2   m21  b1
		blockBmidd(D.body + m.n * (m.n ) / 2, m.n / 2, m.n, a.entries + m.n + m.n / 2, m.body + m.n * m.n / 2, b.entries+ m.n);

		//lower right  a2   m22  b2
		blockBmidd(D.body + m.n * (m.n + 1) / 2,
			m.n / 2, 		m.n, a.entries + m.n + m.n / 2,
			m.body + m.n * (m.n + 1) / 2,		b.entries + m.n + m.n / 2);
	//	printf("\nD is \n");
	//	D.printMatrix();
		// now the depth 1 butterfly
		blockBmidd(C.body, m.n, m.n, a.entries, D.body, b.entries);
		
		free(D.body);

	}else blockBmidd(C.body, m.n, m.n, a.entries, m.body, b.entries);
	return C;

}
void blockBleft(bint * C, int bsize, int rowsize, bint * A, bint * M ){
	

	// c is   c0   A0 + a2		c1  a1 + a3
	//		  c2   a0 - a2      c3  a1 -a3

	//fill pairs in a colomns at a time
	for (int row = 0; row < bsize / 2; row++){//itterate down to next row
		for (int col = 0; col < bsize; col++){//itterate accross the row 
			C[row* rowsize + col] = M[row* rowsize + col];
			C[(row + bsize / 2)* rowsize + col] = M[row* rowsize + col];

			C[row * rowsize + col] += M[(row + bsize / 2)* rowsize + col];
			C[(row + bsize / 2)* rowsize + col] -= M[(row + bsize / 2)* rowsize + col];

		}
	}

	//C.printMatrix();
	//r0 r1 are diagonals of a 
	//d1 =  r0  * c1     d2 =  r0 * c2
	//d3 =  r1	* c3	d4 =  r1  * c4
	for (int row = 0; row < bsize; row++){
		for (int col = 0; col < bsize; col++){
			C[row* rowsize + col] *= A[row] / sqrt(2);
		}
	}

}
matrix leftbmulti(butterfly b, matrix m){
	matrix C(m.n, false);


	if (b.depth == 2){
		matrix D(m.n, true);
		//upper left  a1   m11  b1
		blockBleft(D.body, m.n / 2, m.n, b.entries + m.n, m.body);
		//upper right   a1  m12   b2
		blockBleft(D.body + m.n / 2, m.n / 2, m.n, b.entries + m.n, m.body + m.n / 2);
		//lower left  a2   m21  b1
		blockBleft(D.body + m.n * (m.n) / 2, m.n / 2, m.n, b.entries + m.n + m.n / 2,
			m.body + m.n * m.n / 2);

		//lower right  a2   m22  b2
		blockBleft(D.body + m.n * (m.n + 1) / 2,
			m.n / 2, m.n, b.entries + m.n + m.n / 2,
			m.body + m.n * (m.n + 1) / 2);
		//	printf("\nD is \n");
		//	D.printMatrix();
		// now the depth 1 butterfly
		blockBleft(C.body, m.n, m.n, b.entries, D.body);

		free(D.body);

	}
	else blockBleft(C.body, m.n, m.n, b.entries, m.body);
	return C;

	// c is   c0   A0 + a2		c1  a1 + a3
	//		  c2   a0 - a2      c3  a1 -a3
	/*
	//fill pairs in a colomns at a time
	for (int row = 0; row < m.n / 2; row++){//itterate down to next row
		for (int col = 0; col < m.n ; col++){//itterate accross the row 
			C.body[row* m.n + col] = m.body[row* m.n + col];
			C.body[(row + m.n / 2)* m.n + col] = m.body[row* m.n + col];

			C.body[row* m.n + col] += m.body[(row + m.n / 2)* m.n + col];
			C.body[(row + m.n / 2)* m.n + col] -= m.body[(row + m.n / 2)* m.n  + col];

		}
	}
	
	//C.printMatrix();
	//r0 r1 are diagonals of a 
	//d1 =  r0  * c1     d2 =  r0 * c2
	//d3 =  r1	* c3	d4 =  r1  * c4
	for (int row = 0; row < m.n; row++){
		for (int col = 0; col < m.n; col++){
			C.body[row* m.n + col] *= b.entries[row] / sqrt(2);
		}
	}

	*/
	return C;
}


#endif