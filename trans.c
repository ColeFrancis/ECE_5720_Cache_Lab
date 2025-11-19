/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 * 
 * Cole Francis (A02365322)
 * Justin Hulme (A02427854)
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int j_block, i_block, i, j, temp_0, temp_1, temp_2, temp_3, temp_4, temp_5, temp_6, temp_7;

    // 32x32 and 61x67 both work with 8x8 blocks
    if ((M == N && M == 32) || (M == 61 && N == 67)){
        // iterate through block in the x
        for (j_block = 0; j_block < M; j_block += 8)  // M: columns of A
        {
            // iterate through blocks in the y
            for (i_block = 0; i_block < N; i_block += 8)  // N: rows of A
            {
                // check for diagonal
                if (i_block == j_block)
                {
                    // iterate through rows
                    for (i = i_block; i < i_block + 8 && i < N; i++) 
                    {
                        temp_0 = -1;
                        // iterate through cols
                        for (j = j_block; j < j_block + 8 && j < M; j++)
                        {
                            // save the first one into temp to prevent thrashing and just transpose the other
                            if (j == i) {
                                temp_0 = A[i][j];
                            } else {
                                B[j][i] = A[i][j];
                            }
                        }

                        // save the first item
                        B[i][i] = temp_0;
                    }
                }
                else
                {
                    // iterate through rows
                    for (i = i_block; i < i_block + 8 && i < N; i++) 
                    {
                        // iterate through cols
                        for (j = j_block; j < j_block + 8 && j < M; j++)
                        {
                            // just transpose normally
                            B[j][i] = A[i][j];
                        }
                    }
                }
            }
        }
    }
    // special 64x64 case
    else {
        for (i_block = 0; i_block < 64; i_block += 8)
        {
            for (j_block = 0; j_block < 64; j_block += 8)
            {
                // First 8x8 is broken into two 4x8 operations
                for (i = i_block; i < i_block + 4; i++)
                {

                    temp_0 = A[i][j_block+0];
                    temp_1 = A[i][j_block+1];
                    temp_2 = A[i][j_block+2];
                    temp_3 = A[i][j_block+3];
                    temp_4 = A[i][j_block+4];
                    temp_5 = A[i][j_block+5];
                    temp_6 = A[i][j_block+6];
                    temp_7 = A[i][j_block+7];


                    B[j_block+0][i] = temp_0;
                    B[j_block+1][i] = temp_1;
                    B[j_block+2][i] = temp_2;
                    B[j_block+3][i] = temp_3;


                    // place second half in a temporary vertical slot
                    B[j_block+0][i + 4] = temp_4;
                    B[j_block+1][i + 4] = temp_5;
                    B[j_block+2][i + 4] = temp_6;
                    B[j_block+3][i + 4] = temp_7;
                }

                // Move the saved temporary 4x4 block into final location
                for (j = j_block; j < j_block + 4; j++)
                {
                    temp_4 = B[j][i_block+4];
                    temp_5 = B[j][i_block+5];
                    temp_6 = B[j][i_block+6];
                    temp_7 = B[j][i_block+7];

                    B[j][i_block+4] = A[i_block+4][j];
                    B[j][i_block+5] = A[i_block+5][j];
                    B[j][i_block+6] = A[i_block+6][j];
                    B[j][i_block+7] = A[i_block+7][j];

                    B[j+4][i_block] = temp_4;
                    B[j+4][i_block+1] = temp_5;
                    B[j+4][i_block+2] = temp_6;
                    B[j+4][i_block+3] = temp_7;
                }


                // bottom-right 4×4
                for (i = i_block + 4; i < i_block + 8; i++) 
                {
                    for (j = j_block + 4; j < j_block + 8; j++) 
                    {
                        B[j][i] = A[i][j];
                    }
                }
            }
        }
        return;
    }
}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions(void)
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

