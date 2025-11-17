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
    if (M == N && M == 32){
        for (int i_block = 0; i_block < M; i_block += 8)  
        {
            for (int j_block = 0; j_block < N; j_block += 8)
            {
                if (i_block == j_block)
                {
                    for (int i = i_block; i < i_block + 8; i++) 
                    {
                        int temp = -1;
                        for (int j = j_block; j < j_block + 8; j++)
                        {
                            if (j == i){
                                temp = A[j][i]; 
                            }
                            else{
                                B[i][j] = A[j][i];
                            }
                        }

                        B[i][i] = temp;
                    }
                }
                else
                {
                    for (int i = i_block; i < i_block + 8; i++) 
                    {
                        for (int j = j_block; j < j_block + 8; j++)
                        {
                            B[i][j] = A[j][i];
                        }
                    }
                }
            }
        }  
    }
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

#define BLOCK_SIZE (8)

char transpose_blocks_naive_desc[] = "Transpose Blocks Naive";
void transpose_blocks_naive(int M, int N, int A[N][M], int B[M][N])
{
    // transpose_blocks_param(M, N, A, B, BLOCKSIZE, BLOCKSIZE);
    for (int i_block = 0; i_block < M; i_block += BLOCK_SIZE)  
    {
        for (int j_block = 0; j_block < N; j_block += BLOCK_SIZE)
        {
            for (int i = i_block; i < i_block + BLOCK_SIZE; i++) 
            {
                for (int j = j_block; j < j_block + BLOCK_SIZE; j++)
                {
                    B[i][j] = A[j][i];
                }
            }
        }
    }  
}

char transpose_blocks_diagonal_desc[] = "Transpose Blocks diagonal";
void transpose_blocks_diagonal(int M, int N, int A[N][M], int B[M][N])
{
    for (int block_j = 0; block_j < M; block_j += BLOCK_SIZE)
    {
        for (int block_i = 0; block_i < N; block_i += BLOCK_SIZE)
        {
            if (block_j == block_i) // diagonal block
            {
                for (int offset_i = 0; offset_i < BLOCK_SIZE; offset_i++)
                {
                    int i = block_i + offset_i;

                    int temp = 0;      // temp for diagonal element
                    int diag_index = -1;

                    for (int offset_j = 0; offset_j < BLOCK_SIZE; offset_j++)
                    {
                        int j = block_j + offset_j;

                        if (i < N && j < M)
                        {
                            if (i != j)
                            {
                                B[j][i] = A[i][j];
                            }
                            else
                            {
                                temp = A[i][j];
                                diag_index = i;
                            }
                        }
                    }

                    // write diagonal element AFTER processing the row
                    if (diag_index != -1)
                    {
                        B[diag_index][diag_index] = temp;
                    }
                }
            }
            else // off-diagonal blocks
            {
                for (int offset_i = 0; offset_i < BLOCK_SIZE; offset_i++)
                {
                    for (int offset_j = 0; offset_j < BLOCK_SIZE; offset_j++)
                    {
                        int i = block_i + offset_i;
                        int j = block_j + offset_j;

                        if (i < N && j < M)
                        {
                            B[j][i] = A[i][j];
                        }
                    }
                }
            }
        }
    }
}

/*void transpose_blocks(int M, int N, int A[N][M], int B[M][N])
{
    // transpose_blocks_param(M, N, A, B, BLOCKSIZE, BLOCKSIZE);
    for (int block_j = 0; block_j < M; block_j += BLOCKSIZE)  // columns of A
    {
        for (int block_i = 0; block_i < N; block_i += BLOCKSIZE)  // rows of A
        {
            if (block_j == block_i) {
                // diagonal tile: swap inner loops compared to off-diagonal tiles
                
            } else {
                // off-diagonal tiles
                for (int offset_j = 0; offset_j < BLOCKSIZE; offset_j++) {
                    for (int offset_i = 0; offset_i < BLOCKSIZE; offset_i++) {
                        int i = block_i + offset_i;
                        int j = block_j + offset_j;

                        if (i < N && j < M) {
                            B[j][i] = A[i][j];
                        }
                    }
                }
            }  
        }
    }
}*/

void transpose_blocks_param(int M, int N, int A[N][M], int B[M][N], int blocksize_x, int blocksize_y)
{
    for (int block_j = 0; block_j < M; block_j += blocksize_y)  // columns of A
    {
        for (int block_i = 0; block_i < N; block_i += blocksize_x)  // rows of A
        {
            if (block_j == block_i) {
                // diagonal tile: swap inner loops compared to off-diagonal tiles
                for (int offset_i = 0; offset_i < blocksize_x; offset_i++) {
                    for (int offset_j = 0; offset_j < blocksize_y; offset_j++) {
                        int i = block_i + offset_i;
                        int j = block_j + offset_j;

                        if (i < N && j < M) {
                            B[j][i] = A[i][j];
                        }
                    }
                }
            } else {
                // off-diagonal tiles
                for (int offset_j = 0; offset_j < blocksize_y; offset_j++) {
                    for (int offset_i = 0; offset_i < blocksize_x; offset_i++) {
                        int i = block_i + offset_i;
                        int j = block_j + offset_j;

                        if (i < N && j < M) {
                            B[j][i] = A[i][j];
                        }
                    }
                }
            }  
        }
    }
}

char transpose_tiled_desc[] = "Transpose tiled";
void transpose_tiled(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;
    for (i = 0; i < 32; i++){
        for (j = i; j < 32; j++){
            tmp = B[i][j];
            B[i][j] = A[j][i];
            A[j][i] = tmp;
        }
    }
}

char transpose_triangle_desc[] = "Transpose tiled";
void transpose_triangle(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = i; j < M; j++) {
            if (i != j){
                B[i][j] = A[j][i];
                B[j][i] = A[i][j];
            }
        }
    }    

}

char hyper_32_desc[] = "hyper_32";
void hyper_32(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    if (M != 32){
        return;
    }

    for (i = 0; i < 16; i++){
        for (j = 0; j < 16; j++){
            B[j][i] = A[i][j];
        }
    }

    for (i = 0; i < 16; i++){
        for (j = 16; j < 32; j++){
            B[j][i] = A[i][j];
        }
    }

    for (i = 16; i < 32; i++){
        for (j = 0; j < 16; j++){
            B[j][i] = A[i][j];
        }
    }

    for (i = 16; i < 32; i++){
        for (j = 16; j < 32; j++){
            B[j][i] = A[i][j];
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
    // registerTransFunction(trans, trans_desc);
    // registerTransFunction(transpose_reversed, transpose_reversed_desc);
    //registerTransFunction(transpose_blocks_naive, transpose_blocks_naive_desc);
    registerTransFunction(transpose_blocks_diagonal, transpose_blocks_diagonal_desc);
    // registerTransFunction(transpose_tiled, transpose_tiled_desc);
    // registerTransFunction(transpose_triangle, transpose_triangle_desc);
    // registerTransFunction(hyper_32, hyper_32_desc);
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

