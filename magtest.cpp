// includes, system
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "butterfly.h"
// includes, project
#include "magma.h"
#include "magma_lapack.h"
// magma_dgetmatrix( N, nrhs, d_B, lddb, h_X, ldb );

// #define magma_getmatrix(           m, n, elemSize, dA_src, ldda, hB_dst, ldb )
// #define magma_setmatrix(           m, n, elemSize, hA_src, lda, dB_dst, lddb ) 
//    magma_dsetmatrix( N, N, A.body, lda, d_A, ldda );
//    magma_dsetmatrix( N, nrhs, h_B, ldb, d_B, lddb );
/******************* CPU memory */
#define TESTING_MALLOC_CPU( ptr, type, size )                              \
    if ( MAGMA_SUCCESS !=                                                  \
            magma_malloc_cpu( (void**) &ptr, (size)*sizeof(type) )) {      \
        fprintf( stderr, "!!!! magma_malloc_cpu failed for: %s\n", #ptr ); \
        magma_finalize();                                                  \
        exit(-1);                                                          \
    }

#define TESTING_FREE_CPU( ptr ) magma_free_cpu( ptr )

/******************* GPU memory */
#ifdef HAVE_CUBLAS
    // In CUDA, this has (void**) cast.
    #define TESTING_MALLOC_DEV( ptr, type, size )                              \
        if ( MAGMA_SUCCESS !=                                                  \
                magma_malloc( (void**) &ptr, (size)*sizeof(type) )) {          \
            fprintf( stderr, "!!!! magma_malloc failed for: %s\n", #ptr );     \
            magma_finalize();                                                  \
            exit(-1);                                                          \
        }
#else
    // For OpenCL, ptr is cl_mem* and there is no cast.
    #define TESTING_MALLOC_DEV( ptr, type, size )                              \
        if ( MAGMA_SUCCESS !=                                                  \
                magma_malloc( &ptr, (size)*sizeof(type) )) {                   \
            fprintf( stderr, "!!!! magma_malloc failed for: %s\n", #ptr );     \
            magma_finalize();                                                  \
            exit(-1);                                                          \
        }
#endif

#define TESTING_FREE_DEV( ptr ) magma_free( ptr )

/* ////////////////////////////////////////////////////////////////////////////
   -- Testing Butterfly matrices
*/
int main(int argc, char **argv)
{
    magma_init();
    magma_print_environment();

    real_Double_t   gpu_time;
    // double error;
    double *h_B;
    magmaDouble_ptr d_A, d_B;
    magma_int_t *ipiv;
    magma_int_t N, nrhs, lda, ldb, ldda, lddb, info, sizeA, sizeB;
    magma_int_t status = 0;
     
    nrhs = 1; //opts.nrhs;
    
    N = 5000; // opts.nsize[itest];
    lda    = N;
    ldb    = lda;
    ldda   = 5000;
    lddb   = ldda;
   
   
    // Allocating matrix A and Vector B 
    TESTING_MALLOC_CPU( h_B, double, ldb*nrhs );


    TESTING_MALLOC_CPU( ipiv, magma_int_t, N );
    
    // Allocating device space for host Matrix and vector
    TESTING_MALLOC_DEV( d_A, double, ldda*N    );
    TESTING_MALLOC_DEV( d_B, double, lddb*nrhs );
   
    Matrix aOut1(5000, false), aOut2(5000, false);
    /* Initialize the matrices */
    Matrix A(5000, true);
    sizeA = lda*N;
    sizeB = ldb*nrhs;

    srand(12345);

    for (int i = 0; i < sizeB; ++i) {
        h_B[i] = (double) rand() / RAND_MAX;
    }
   
    // copy to device 
    magma_dsetmatrix( N, N, A.body, lda, d_A, ldda );
    magma_dsetmatrix( N, nrhs, h_B, ldb, d_B, lddb );
    
    /* ====================================================================
       Performs operation using MAGMA
       =================================================================== */
    printf("Size A=%d, size b=%d\n", sizeA, sizeB);
    
    Butterfly U(5000, 2), V(5000, 2); 

    // Run on piv, no butterfly
    gpu_time = magma_wtime();
    magma_dgesv_gpu( N, nrhs, d_A, ldda, ipiv, d_B, lddb, &info );
    gpu_time = magma_wtime() - gpu_time;
    printf("piv time: %f\n", gpu_time);
    
    // Get output from the device
    magma_dgetmatrix( N, nrhs, d_B, lddb, aOut1.body, ldb );

    // Reset original matrix A and vector B to device
    magma_dsetmatrix( N, N, A.body, lda, d_A, ldda );
    magma_dsetmatrix( N, nrhs, h_B, ldb, d_B, lddb );

    
    // Run on no pivoting, no butterfly
    gpu_time = magma_wtime();
    magma_dgesv_nopiv_gpu( N, nrhs, d_A, ldda, d_B, lddb, &info ); 
    gpu_time = magma_wtime() - gpu_time;
    printf("no piv time: %f\n", gpu_time);
    
    // Get output from the device
    magma_dgetmatrix( N, nrhs, d_B, lddb, aOut2.body, ldb );
    if (info != 0)
        printf("magma_dgesv_gpu returned error %d: %s.\n",
               (int) info, magma_strerror( info ));
    
    /* ============================================================= */
    Matrix::percenterror(aOut1, aOut2);
     
    TESTING_FREE_CPU( h_B );
    TESTING_FREE_CPU( ipiv );
    
    TESTING_FREE_DEV( d_A );
    TESTING_FREE_DEV( d_B );
    fflush( stdout );

    magma_finalize();
    return status;
}

