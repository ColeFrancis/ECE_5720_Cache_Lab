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
 * Justin Hulme (A________)
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
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char transpose_reversed_desc[] = "Transpose Reversed";
void transpose_reversed(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (j = 0; j < M; j++) {
        for (i = 0; i < N; i++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    } 
}

#define BLOCKSIZE (16)

char transpose_blocks_desc[] = "Transpose Blocks";
void transpose_blocks(int M, int N, int A[N][M], int B[M][N])
{
    for (int block_x = 0; block_x < M; block_x += BLOCKSIZE)
    {
        for (int block_y = 0; block_y < N; block_y += BLOCKSIZE)
        {
            for (int block_offset_x = 0; block_offset_x < BLOCKSIZE; block_offset_x++)
            {
                for (int block_offset_y = 0; block_offset_y < BLOCKSIZE; block_offset_y++)
                {
                    int temp = A[block_x+block_offset_x][block_y+block_offset_y];
                    B[block_x+block_offset_x][block_y+block_offset_y] = temp;
                }
            }
        }
    }
}

void transpose_blocks_param(int M, int N, int A[N][M], int B[M][N], int blocksize_x, int blocksize_y)
{
    for (int block_x = 0; block_x < M; block_x += blocksize_x)
    {
        for (int block_y = 0; block_y < N; block_y += blocksize_y)
        {
            for (int block_offset_x = 0; block_offset_x < blocksize_x; block_offset_x++)
            {
                for (int block_offset_y = 0; block_offset_y < blocksize_y; block_offset_y++)
                {
                    int temp = A[block_x+block_offset_x][block_y+block_offset_y];
                    B[block_x+block_offset_x][block_y+block_offset_y] = temp;
                }
            }
        }
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

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);
    registerTransFunction(transpose_reversed, transpose_reversed_desc);

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

