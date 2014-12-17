// includes, system
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// includes, project
#include "magma.h"
#include "magma_lapack.h"

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
   -- Testing dgesv_gpu
*/
int main(int argc, char **argv)
{
    magma_init();
    magma_print_environment();

    real_Double_t   gflops, cpu_perf, cpu_time, gpu_perf, gpu_time;
    double          error, Rnorm, Anorm, Xnorm, *work;
    double c_one     = MAGMA_D_ONE;
    double c_neg_one = MAGMA_D_NEG_ONE;
    double *h_A, *h_B, *h_X;
    magmaDouble_ptr d_A, d_B;
    magma_int_t *ipiv;
    magma_int_t N, nrhs, lda, ldb, ldda, lddb, info, sizeA, sizeB;
    magma_int_t ione     = 1;
    magma_int_t ISEED[4] = {0,0,0,1};
    magma_int_t status = 0;
    
    // magma_opts opts;
    // parse_opts( argc, argv, &opts );
     
    nrhs = 1; //opts.nrhs;
    
    printf("    N  NRHS   CPU GFlop/s (sec)   GPU GFlop/s (sec)   ||B - AX|| / N*||A||*||X||\n");
    printf("================================================================================\n");
    //for( int itest = 0; itest < opts.ntest; ++itest ) {
      //  for( int iter = 0; iter < opts.niter; ++iter ) {
            N = 5000; // opts.nsize[itest];
            lda    = N;
            ldb    = lda;
            ldda   = ((N+31)/32)*32;
            lddb   = ldda;
            // gflops = ( FLOPS_DGETRF( N, N ) + FLOPS_DGETRS( N, nrhs ) ) / 1e9;
            
            TESTING_MALLOC_CPU( h_A, double, lda*N    );
            TESTING_MALLOC_CPU( h_B, double, ldb*nrhs );

            TESTING_MALLOC_CPU( h_X, double, ldb*nrhs );
            TESTING_MALLOC_CPU( work, double,      N );
            TESTING_MALLOC_CPU( ipiv, magma_int_t, N );
            
            TESTING_MALLOC_DEV( d_A, double, ldda*N    );
            TESTING_MALLOC_DEV( d_B, double, lddb*nrhs );
            
            /* Initialize the matrices */
            sizeA = lda*N;
            sizeB = ldb*nrhs;

            srand(12345);
            for (int i = 0; i < sizeA; ++i) {
                h_A[i] = (double) rand() / RAND_MAX;
            }

            for (int i = 0; i < sizeB; ++i) {
                h_B[i] = (double) rand() / RAND_MAX;
            }

            // lapackf77_dlarnv( &ione, ISEED, &sizeA, h_A );
            // lapackf77_dlarnv( &ione, ISEED, &sizeB, h_B );
            
            magma_dsetmatrix( N, N,    h_A, lda, d_A, ldda );
            magma_dsetmatrix( N, nrhs, h_B, ldb, d_B, lddb );
            
            /* ====================================================================
               Performs operation using MAGMA
               =================================================================== */
            printf("Size A=%d, size b=%d\n", sizeA, sizeB);
            
            gpu_time = magma_wtime();
            magma_dgesv_gpu( N, nrhs, d_A, ldda, ipiv, d_B, lddb, &info );
            
            gpu_time = magma_wtime() - gpu_time;
            printf("piv time: %f\n", gpu_time);

            magma_dsetmatrix( N, N,    h_A, lda, d_A, ldda );
            magma_dsetmatrix( N, nrhs, h_B, ldb, d_B, lddb );

            gpu_time = magma_wtime();
            magma_dgesv_nopiv_gpu( N, nrhs, d_A, ldda, d_B, lddb, &info );
            
            gpu_time = magma_wtime() - gpu_time;
            printf("no piv time: %f\n", gpu_time);
            

            // gpu_perf = gflops / gpu_time;
            if (info != 0)
                printf("magma_dgesv_gpu returned error %d: %s.\n",
                       (int) info, magma_strerror( info ));
            
            /* ============================================================= */
             
            TESTING_FREE_CPU( h_A );
            TESTING_FREE_CPU( h_B );
            TESTING_FREE_CPU( h_X );
            TESTING_FREE_CPU( work );
            TESTING_FREE_CPU( ipiv );
            
            TESTING_FREE_DEV( d_A );
            TESTING_FREE_DEV( d_B );
            fflush( stdout );
   //     }
   //     if ( opts.niter > 1 ) {
            printf( "\n" );
   //     }
   // }

    magma_finalize();
    return status;
}
