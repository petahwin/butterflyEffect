#include "butterfly.h"
#include "test.h"

Matrix middleTestmulti(testfly a, Matrix m, testfly b){
	Matrix mid(m.n, true);
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
	Matrix ret(m.n, true);
	for (int row = 0; row < m.n; row++ ){//itterate down to next row 
		for (int col = 0; col < m.n; col++){//itterate acrross row
		
			ret.body[row * m.n + col] = 0.0;
			for (int i = 0; i < m.n; i++){
//				printf("adding %d %d    %g   %g\n", 
	//				i + row* m.n, (i)* m.n + col, ret.body[row * m.n + col],
		//			b.entries[(i)* m.n + col]);
				ret.body[row * m.n + col ] +=
					mid.body[ i + row* m.n] *
					b.entries[( i) * m.n + col];
			}
		}

	}
	free(mid.body);
	return ret;
}
Matrix leftTestmulti(testfly b, Matrix m){
	Matrix mid(m.n, true);
	assert(m.n == b.size);
	for (int row = 0; row < m.n; row++){//itterate acrross row 
		for (int col = 0; col < m.n; col++){//itterate down to next row
			mid.body[row * m.n + col] = 0.0;
			for (int i = 0; i < m.n; i++){
				mid.body[row *m.n + col] +=
					b.entries[i + row* m.n] *
					m.body[(i)* m.n + col];
			}
		}

	}
	return mid;
}
double * Matrixmulti(Matrix b, Matrix m){
	double * body = (double *) malloc(sizeof(double)* m.n * m.n);
	assert(m.n == b.n);
	for (int row = 0; row < m.n; row++){//itterate acrross row 
		for (int col = 0; col < m.n; col++){//itterate down to next row
			body[row * m.n + col] = 0.0;
			for (int i = 0; i < m.n; i++){
				body[row *m.n + col] +=
					b.body[i + row* m.n] *
					m.body[(i)* m.n + col];
			}
		}

	}
	return body;
}

void setSubTestFly(bint * write, int rowsize, int size, bint * read){
	//write in a Matrix first entry to write to 
	//rowsize is the lenght of the rows in the original Matrix
	//size is the dimentions of this sub Matrix 
	//read is an location in a Butterfly entry 
	int i = 0;
	//assumes entries were zero ed first 
	for ( i = 0; i < size / 2; i++){  //upper left
		write[i * rowsize + i] = read[i] * 1 / sqrt(2);
	}
	for ( i = 0; i < size / 2; i++){  //uper right
		write[i * rowsize + i + size / 2] = read[i + size / 2] * 1 / sqrt(2);
	}
	for ( i = 0; i <size / 2; i++){  //lower left
		write[i * rowsize + i + rowsize *size / 2] = read[i] * 1 / sqrt(2);
	}
	for ( i = 0; i < size / 2; i++){  //lower right
		write[i * rowsize + i + rowsize *size / 2 + size / 2] = read[i + size / 2] * -1 / sqrt(2);
	}
/*	printf("\n Bkock \n");
	for ( i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			printf("%g    ", write[i*rowsize + j]);
		}
		printf("\n");
	}*/

	return;
}


