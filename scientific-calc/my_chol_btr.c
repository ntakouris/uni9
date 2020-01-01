#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "blas.h"
#include "lapack.h"

#include "mex.h"
#include "matrix.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

    double *A, *B, *C;              /* pointers to input & output matrices*/
    ptrdiff_t rows_A, cols_A;
    ptrdiff_t rows_B, cols_B;

    // create input matrix (A)
    //A = mxGetPr(prhs[0]);       /* first input matrix */
    A = mxGetDoubles(prhs[0]);
    rows_A = mxGetM(prhs[0]);
    cols_A = mxGetN(prhs[0]);

    // create input matrix (B)
    B = mxGetDoubles(prhs[1]);

    //B = mxGetPr(prhs[1]); // part of old api

    rows_B = mxGetM(prhs[1]);
    cols_B = mxGetN(prhs[1]);

    // create output matrix (C)
    plhs[0] = mxCreateDoubleMatrix(rows_A, cols_B, mxREAL);
    C = mxGetPr(plhs[0]);

    printf("A[0]: %lf\n", A[0]);
    printf("A[1]: %lf\n", A[1]);

    // compute mm multiplication using blas3 operations
    char  chn   = 'N';
    double one  = 1;
    double zero = 0;

    dgemm_(&chn, &chn, 
           &rows_A,  &rows_A, &cols_A, 
           &one, A, &cols_B, B, &rows_B, 
           &one, C,  &rows_A);

}
