#include "butterfly.h"
#include <cuda.h>
#include<cuda_runtime.h>
#define blocksize 8
#define MaxTW 8
//matrix must be mutiple of 2 in size 


/*----------------------------Matrix METHODS---------------------------*/

Matrix::Matrix(int n_in, bool randfill) {
	n = n_in;
	if (randfill == true) {
		body = (double *)malloc(sizeof(double)* n* n);
		for (int i = 0; i < n* n; i++) body[i] = (double) rand();
	}
	else {
		body = (double *)calloc(n* n, sizeof(double));
	}
}

void Matrix::printMatrix(void) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%g    ", body[i*n + j]);
		}
		printf("\n\n");
	}
}

//assume m is correct 
void Matrix::percenterror(Matrix m, Matrix A){
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

	printf("\ntotal error was %g  percent error \
		           was %g\n\n", totalerr, percenterr);
	return;
}

/*--------------------------Butterfly METHODS--------------------------*/

Butterfly::Butterfly(int insize, int indepth) {
	size = insize;
	depth = indepth;
	entries = (bint *)malloc(depth * size * sizeof(bint));
	transposed = false;

	int r = rand();
	for (int i = 0; i < indepth * insize; i++){
		entries[i] =  (bint)rand();// / INT_MAX;
	}
	return;
}

//very cheep function for transposing  
void Butterfly::transpose(void) {
	transposed = !transposed;
}

void Butterfly::printEntries(void) {
	for (int i = 0; i < size * depth; i++) {
		printf("%g  ", entries[i]);
	}
}

struct Data {
    int vals[4][3];
	int bigindex[4];
};

Data * writeloc(int rowsize, int bsize){
	// initilize the vals and big index 
	//write them to GPU mem
	Data cur;
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
	printf("big 1:%d  2:%d   3:%d   4:%d\n", cur.bigindex[0], cur.bigindex[1], cur.bigindex[2], cur.bigindex[3]);
	Data * gpuloc;
	cudaMalloc((void**)&gpuloc, sizeof(Data));
	cudaMemcpy(gpuloc, & cur, sizeof(Data), cudaMemcpyHostToDevice);
	return gpuloc;

}

__global__ void gpu_buttermulti(double * C, int bsize, int rowsize, bint * A, double * M, bint * B, Data * data) {
	//do the row and colith entry in in quadrent 
	int col = threadIdx.x + blockDim.x * blockIdx.x;
	int row = threadIdx.y + blockDim.y * blockIdx.y;
	double Clocal[4];
	for (int block = 0; block < 4; block++){
		Clocal[block] = M[row * rowsize + col];// row * rowsize + col;
	//C[ row* rowsize + col] = row * rowsize + col;
			//M[row * rowsize + col];
		for (int j = 1; j < 4; j++){

			Clocal[block] +=
				M[data->bigindex[j] + row* rowsize + col] * data->vals[block][j - 1];
		}
	}
	for (int block = 0; block < 4; block++){
		Clocal[block] *= A[row + bsize / 2 * (block / 2)] *
							B[col + (block % 2) * bsize / 2] * .5;
	}
	for (int block = 0; block < 4; block++){
		C[data->bigindex[block] + row* rowsize + col] = Clocal[block];
	}

}
__global__ void gpu_LeftButtermulti(double * C, int bsize, int rowsize, bint * A, double * M) {
	//each responsible for 4 entries 
	int col = threadIdx.x + blockDim.x * blockIdx.x;
	int row = threadIdx.y + blockDim.y * blockIdx.y;
	// c is   c0   A0 + a2		c1  a1 + a3
	//		  c2   a0 - a2      c3  a1 -a3
	double Clocal[2];
	Clocal[0]= M[row* rowsize + col];
	Clocal[1]= M[row* rowsize + col];

	Clocal[0]+= M[(row + bsize / 2)* rowsize + col];
	Clocal[1] -= M[(row + bsize / 2)* rowsize + col];

	Clocal[0]*= A[row] / sqrt(2.0);
	Clocal[1]*= A[row + bsize / 2] / sqrt(2.0);
	C[row* rowsize + col] = Clocal[0];
	C[(row + bsize / 2)* rowsize + col] = Clocal[1];


}



Matrix middlebmulti(Butterfly a, Matrix m, Butterfly b){
	//compute chunks of C 
	assert(a.size = m.n);
	assert(b.depth = a.depth);
	// check gpu ready



	// --------------------SET PARAMETERS AND DATA -----------------------
	int gpucount = 0;
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
	cudaMalloc((void**)&Mgpu, matSize);

	cudaMemcpy(Agpu, a.entries, butterSize, cudaMemcpyHostToDevice);
	cudaMemcpy(Bgpu, b.entries, butterSize, cudaMemcpyHostToDevice);
	cudaMemcpy(Mgpu, m.body, matSize, cudaMemcpyHostToDevice);

	dim3 Grid(m.n / (blocksize * 2), m.n / (blocksize * 2)); //Grid structure
	dim3 Block(blocksize, blocksize); //Block structure
	printf("\nblock:%d grid:%d m.n:%d \n\n ", blocksize, m.n / (blocksize * 2), m.n);
	Matrix C(m.n, true);
	//start at lowest depth and work outward
	if (a.depth == 2){
		dim3 miniGrid(m.n / (blocksize * 4), m.n / (blocksize * 4)); //Grid structure
		//cuda alloc spae for D
		Matrix D(m.n, true);
		cudaMalloc((void**)&Dgpu, matSize);
		//set vars 
		Data * data = writeloc(m.n, m.n / 2);

		//upper left  a1   m11  b1
		gpu_buttermulti<<<miniGrid, Block>>>(Dgpu, m.n / 2, m.n,
			Agpu + m.n, Mgpu, Bgpu + m.n, data);


		//upper right   a1  m12   b2
		gpu_buttermulti << <miniGrid, Block >> >(Dgpu + m.n / 2, m.n / 2, m.n,
			Agpu + m.n, Mgpu + m.n / 2, Bgpu + m.n / 2 + m.n, data);

		//lower left  a2   m21  b1
		gpu_buttermulti << <miniGrid, Block >> >(Dgpu + m.n * (m.n) / 2, m.n / 2, m.n, 
			Agpu + m.n + m.n / 2, Mgpu + m.n * m.n / 2, Bgpu + m.n, data);


		//lower right  a2   m22  b2
		gpu_buttermulti << <miniGrid, Block >> >(Dgpu + m.n * (m.n + 1) / 2,
			m.n / 2, m.n, Agpu + m.n + m.n / 2,
			Mgpu + m.n * (m.n + 1) / 2, Bgpu + m.n + m.n / 2, data);
		cudaFree(data);

		data = writeloc(m.n, m.n );
		//make space for C on gpu
		cudaMalloc((void**)&Cgpu, matSize);
		// now the depth 1 Butterfly
		gpu_buttermulti << <Grid, Block >> >(Cgpu, m.n, m.n, Agpu, Dgpu, Bgpu, data);

		cudaMemcpy(C.body, Cgpu, matSize, cudaMemcpyDeviceToHost);

		free(D.body);
		cudaFree(Dgpu);

	}
	else {
		Data * data = writeloc(m.n, m.n);
		cudaMalloc((void**)&Cgpu, matSize);
		//make space for C on gpu
		gpu_buttermulti << <Grid, Block >> >(Cgpu, m.n, m.n, Agpu, Mgpu, Bgpu, data);
		cudaMemcpy(C.body, Cgpu, matSize, cudaMemcpyDeviceToHost);
		cudaFree(data);
	}


	cudaFree(Cgpu);
	cudaFree(Mgpu);
	cudaFree(Agpu);
	cudaFree(Bgpu);
	return C;

}

Matrix leftbmulti(Butterfly a, Matrix m){
	//compute chunks of C 
	assert(a.size = m.n);
	// check gpu ready



	// --------------------SET PARAMETERS AND DATA -----------------------
	int gpucount = 0;
	cudaError_t errorcode;

	errorcode = cudaGetDeviceCount(&gpucount);
	if (errorcode == cudaErrorNoDevice) {
		printf("No GPUs are visible\n");
		exit(-1);
	}


	double * Agpu, *Mgpu, *Cgpu, *Dgpu;
	int butterSize = a.depth * a.size * sizeof(bint);
	int matSize = m.n * m.n * sizeof(double);
	// push a m b onto gpu 
	cudaMalloc((void**)&Agpu, butterSize);
	
	cudaMalloc((void**)&Mgpu, matSize);

	cudaMemcpy(Agpu, a.entries, butterSize, cudaMemcpyHostToDevice);

	cudaMemcpy(Mgpu, m.body, matSize, cudaMemcpyHostToDevice);

	dim3 Grid(m.n / (blocksize ), m.n / (blocksize * 2)); //Grid structure
	dim3 Block(blocksize, blocksize); //Block structure
	printf("\nblock:%d grid:%d m.n:%d \n\n ", blocksize, m.n / (blocksize * 2), m.n);
	Matrix C(m.n, true);
	//start at lowest depth and work outward
	if (a.depth == 2){
		dim3 miniGrid(m.n / (blocksize * 2), m.n / (blocksize * 4)); //Grid structure
		//cuda alloc spae for D
		Matrix D(m.n, true);
		cudaMalloc((void**)&Dgpu, matSize);
		
		// reset internal values 

		//upper left  a1   m11  b1
		gpu_LeftButtermulti << <miniGrid, Block >> >(Dgpu, m.n / 2, m.n,
			Agpu + m.n, Mgpu );


		//upper right   a1  m12   b2
		gpu_LeftButtermulti << <miniGrid, Block >> >(Dgpu + m.n / 2, m.n / 2, m.n,
			Agpu + m.n, Mgpu + m.n / 2);

		//lower left  a2   m21  b1
		gpu_LeftButtermulti << <miniGrid, Block >> >(Dgpu + m.n * (m.n) / 2, m.n / 2, m.n,
			Agpu + m.n + m.n / 2, Mgpu + m.n * m.n / 2);


		//lower right  a2   m22  b2
		gpu_LeftButtermulti << <miniGrid, Block >> >(Dgpu + m.n * (m.n + 1) / 2,
			m.n / 2, m.n, Agpu + m.n + m.n / 2,
			Mgpu + m.n * (m.n + 1) / 2);
		

		
		//make space for C on gpu
		cudaMalloc((void**)&Cgpu, matSize);
		// now the depth 1 Butterfly
		gpu_LeftButtermulti << <Grid, Block >> >(Cgpu, m.n, m.n, Agpu, Dgpu);

		cudaMemcpy(C.body, Cgpu, matSize, cudaMemcpyDeviceToHost);

		free(D.body);
		cudaFree(Dgpu);

	}
	else {
		
		cudaMalloc((void**)&Cgpu, matSize);
		//make space for C on gpu
		gpu_LeftButtermulti << <Grid, Block >> >(Cgpu, m.n, m.n, Agpu, Mgpu);
		cudaMemcpy(C.body, Cgpu, matSize, cudaMemcpyDeviceToHost);
		
	}


	cudaFree(Cgpu);
	cudaFree(Mgpu);
	cudaFree(Agpu);
	
	return C;

}