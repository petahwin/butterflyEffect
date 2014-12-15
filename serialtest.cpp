#ifndef TESTFLY_H
#define TESTFLY_H 1
#include "butterfly.h"
#include "test.h"




matrix middleTestmulti(testfly a, matrix m, testfly b){
	matrix mid(m.n, true);
	assert(a.size == m.n);
	assert(m.n == b.size);
	for (int row = 0; row < m.n; row++){//itterate acrross row 
		for (int col = 0; col < m.n; col++){//itterate down to next row
			mid.body[row * m.n + col] = 0.0;
			for (int i = 0; i < m.n; i++){
				mid.body[row *m.n + col ] +=
					a.entries[i + row* m.n] *
					m.body[( i) * m.n + col];
			}
		}

	}
	printf("mid is \n");
	mid.printMatrix();
	matrix ret(m.n, true);
	for (int row = 0; row < m.n; row++ ){//itterate down to next row 
		for (int col = 0; col < m.n; col++){//itterate acrross row
		
			ret.body[row * m.n + col] = 0.0;
			for (int i = 0; i < m.n; i++){
				printf("adding %d %d    %g   %g\n", 
					i + row* m.n, (i)* m.n + col, ret.body[row * m.n + col],
					b.entries[(i)* m.n + col]);
				ret.body[row * m.n + col ] +=
					mid.body[ i + row* m.n] *
					b.entries[( i) * m.n + col];
			}
		}

	}
	free(mid.body);
	return ret;
}

#endif