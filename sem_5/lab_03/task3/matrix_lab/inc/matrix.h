#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "stdio.h"

#define COUNT_DIM 2


typedef struct
{
    int n;
    int m;
    double **data;
} matrix_t;

typedef struct
{
    int *indexes;
    double *data;
    int count;
} solution_t;


double **allocate_matrix(int n, int m);
int read_matrix(matrix_t *matrix, char *filename);
int create_add_matrix(matrix_t *m1, matrix_t *m2, matrix_t *add_result);
int create_multip_matrix(matrix_t *m1, matrix_t *m2, matrix_t *mult);
int init_solution(solution_t *sol, int count);
int slae_solution(matrix_t *matrix, solution_t *solution);
int print_slae_sol(solution_t *sol, char *filename);
void free_solution(solution_t *sol);
int print_matrix(matrix_t matrix, char *filename);

#endif // _MATRIX_H_