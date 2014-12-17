#ifndef BUTTERFLY_CU
#define BUTTERFLY_CU 1

#include "butterfly.h"
#include <cuda.h>
#include<cuda_runtime.h>

#define blocksize 4
//matrix must be mutiple of 2 in size 

struct data {
	static int vals[4][3];
	int bigindex[4];
};
data * writeloc(int rowsize, int bsize){
	// initilize the vals and big index 
	//write them to GPU mem
	data cur;
	cur.vals[0][0] = 1;
	cur.vals[0][1] = 1; // c1  m 3 
	cur.vals[0][2] = 1;

	cur.vals[1][0] = -1;
	cur.vals[1][1] = 1;
	cur.vals[1][2] = -1;

	cur.vals[2][0] = 1;
	cur.vals[2][1] = -1;
	cur.vals[2][2] = -1;

	cur.vals[3][0] = -1;
	cur.vals[3][1] = -1;
	cur.vals[3][2] = 1;

	cur.bigindex[0] = 0;  // index of each block in M and write 
	cur.bigindex[1] = bsize / 2;
	cur.bigindex[2] = rowsize * bsize / 2;
	cur.bigindex[3] = rowsize * bsize / 2 + bsize / 2;
	data * gpuloc;
	cudaMalloc((void**)&gpuloc, sizeof(data));
	cudaMemcpy(gpuloc, & cur, sizeof(data), cudaMemcpyHostToDevice);
	return gpuloc;

}

__global__ void gpu_buttermulti(double * C, int bsize, int rowsize, bint * A, bint * M, bint * B) {

	int col = threadIdx.x + blockDim.x * blockIdx.x;
	int row = threadIdx.y + blockDim.y * blockIdx.y;

	

}



/*

void blockBmidd(double * C, int bsize, int rowsize, bint * A, bint * M, bint * B){
	//A M B and C must be pointers into the GPU memory 
	//C is the location in a rowsize * row size matrix where the block of bsize * bsize will start 
	//A B are the correct entries in 2 butterfly matrices to start this block
	//M is the start in 
	//compute chunks of C 
	// C must be initilized to a row size by row size matrix 
	assert(bsize <= rowsize);
	assert(rowsize % rowsize == 0);
	//by a i mean m 
	//0 c1 = a11 + a12 +a21 +a22    1 c2 = a11 - a12 +a21 - a22
	//2 c3 = a11 + a12 -a21 - a22	3 c4 = a11 - a12 - a21 +a22
	static int vals[4][3];
	


	// each thread will write 1 entries of c by loading 1 entires of A into local mem
	//each block will load a tile from each of the 4 quadrants 




	//start parrallel section 
	for (int block = 0; block < 4; block++){//  split matrix into blocks 

		//	printf("inddex:%d   block:%d\n", index[block], block);
		for (int row = 0; row < bsize / 2; row++){//itterate down to next row
			for (int col = 0; col < bsize / 2; col++){//itterate accross the row 
				//set intital value by loading upper left corrner
				
				C[bigindex[block] + row* rowsize + col] =
					M[row * rowsize + col];
				for (int j = 1; j < 4; j++){

					C[bigindex[block] + row* rowsize + col] +=
						M[bigindex[j] + row* rowsize + col] * vals[block][j - 1];
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
	//end parrallel section 
}
*/



matrix middlebmulti(butterfly a, matrix m, butterfly b){
	//compute chunks of C 
	assert(a.size = m.n);
	assert(b.depth = a.depth);
	// check gpu ready



	// --------------------SET PARAMETERS AND DATA -----------------------
	int gpucount = 0;
	cudaEvent_t start, stop; // using cuda events to measure time
	float elapsed_time_ms; // which is applicable for asynchronous code also
	cudaError_t errorcode;

	errorcode = cudaGetDeviceCount(&gpucount);
	if (errorcode == cudaErrorNoDevice) {
		printf("No GPUs are visible\n");
		exit(-1);
	}


	double * Agpu, * Mgpu, * Bgpu , * Cgpu, *Dgpu;
	int butterSize = a.depth * a.size * sizeof(bint);
	int matSize = m.n * m.n * sizeof(double);
	// push a m b onto gpu 
	cudaMalloc((void**)&Agpu, butterSize);
	cudaMalloc((void**)&Bgpu, butterSize);
	cudaMalloc((void**)&Agpu, matSize);

	cudaMemcpy(Agpu, a.entries, butterSize, cudaMemcpyHostToDevice);
	cudaMemcpy(Bgpu, b.entries, butterSize, cudaMemcpyHostToDevice);
	cudaMemcpy(Mgpu, m.body, matSize, cudaMemcpyHostToDevice);

	dim3 Grid(m.n / blocksize , m.n / blocksize); //Grid structure
	dim3 Block(blocksize, blocksize); //Block structure

	matrix C(m.n, true);
	//start at lowest depth and work outward
	if (a.depth == 2){
		
		//cuda alloc spae for D
		matrix D(m.n, true);
		cudaMalloc((void**)&Dgpu, matSize);
		//set vars 
// reset internal values 
		//upper left  a1   m11  b1
		gpu_buttermulti<<<Grid, Block>>>(Dgpu, m.n / 2, m.n,
			Agpu + m.n, Mgpu, Bgpu + m.n);
		
		//upper right   a1  m12   b2
		gpu_buttermulti << <Grid, Block >> >(Dgpu + m.n / 2, m.n / 2, m.n,
			Agpu + m.n, Mgpu + m.n / 2, Bgpu + m.n / 2 + m.n);

		//lower left  a2   m21  b1
		gpu_buttermulti << <Grid, Block >> >(Dgpu + m.n * (m.n) / 2, m.n / 2, m.n, 
			Agpu + m.n + m.n / 2, Mgpu + m.n * m.n / 2, Bgpu + m.n);

		//lower right  a2   m22  b2
		gpu_buttermulti << <Grid, Block >> >(Dgpu + m.n * (m.n + 1) / 2,
			m.n / 2, m.n, Agpu + m.n + m.n / 2,
			Mgpu + m.n * (m.n + 1) / 2, Bgpu + m.n + m.n / 2);


		
		//make space for C on gpu
		cudaMalloc((void**)&Cgpu, matSize);
		// now the depth 1 butterfly
		gpu_buttermulti << <Grid, Block >> >(Cgpu, m.n, m.n, Agpu, Dgpu, Bgpu);
		cudaMemcpy(C.body, Cgpu, matSize, cudaMemcpyDeviceToHost);

		free(D.body);
		cudaFree(Dgpu);

	}
	else {
		cudaMalloc((void**)&Cgpu, matSize);
		//make space for C on gpu
		gpu_buttermulti << <Grid, Block >> >(Cgpu, m.n, m.n, Agpu, Mgpu, Bgpu);
		cudaMemcpy(C.body, Cgpu, matSize, cudaMemcpyDeviceToHost);
	}


	cudaFree(Cgpu);
	cudaFree(Mgpu);
	cudaFree(Agpu);
	cudaFree(Bgpu);
	return C;

}
void blockBleft(bint * C, int bsize, int rowsize, bint * A, bint * M){


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
			C[row* rowsize + col] *= A[row] / sqrt(2.0);
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

}


#endif