#include "butterfly.h"



class testfly{//holds unpacked butterfly matrices 
public:
	int size; //width of square matrix 
	int depth;// the number of recursive levels
	bool transposed;
	bint * entries; // diagonal entries 
	testfly(int size, int depth, butterfly b);
	//depth numb arrays 
	//each is width long 

};
testfly::testfly(int insize, int indepth, butterfly b){
	testfly::size = size;
	testfly::depth = depth;
	testfly::entries = (bint *)malloc(size * size);
	testfly::transposed = false;
	for (int i = 0; i < size *size; i++) testfly::entries[i] = 0;
	for (int i = 0; i < size; i++){
		testfly::entries[i * size] = b.entries[0][i];
		for (int j = 1; j < b.depth; j++){
			testfly::entries[i * size] *= b.entries[j][i];
		}

	}
	return;
}


struct matrix middleTestmulti(testfly a, struct matrix m, testfly b){
	struct matrix mid;
	mid.n = m.n;
	mid.body = (double *)malloc(mid.n * mid.n * sizeof(double));
	for (int col = 0; col < m.n; col++){//itterate acrross row 
		for (int row = 0; row < m.n; row++){//itterate down to next row
			for (int i = 0; i < m.n; i++){
				mid.body[row + col * m.n] +=
					a.entries[col + i + row* m.n] *
					m.body[(row + i) * m.n + col];
			}
		}

	}
	struct matrix ret;
	ret.n = m.n;
	ret.body = (double *)malloc(mid.n * mid.n * sizeof(double));
	for (int col = 0; col < m.n; col++){//itterate acrross row 
		for (int row = 0; row < m.n; row++){//itterate down to next row
			for (int i = 0; i < m.n; i++){
				ret.body[row + col * m.n] +=
					mid.body[col + i + row* m.n] *
					b.entries[(row + i) * m.n + col];
			}
		}

	}
	free(mid.body);
	return ret;
}

