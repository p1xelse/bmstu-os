#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"
#include "errors.h"

#define EPS 1e-8

int init_solution(solution_t *sol, int count)
{
    sol->indexes = calloc(count, sizeof(int));
    sol->count = count;

    if (!sol->indexes)
        return ERR_MEM;

    sol->data = calloc(count, sizeof(double));

    if (!sol->data)
        return ERR_MEM;

    for (int i = 0; i < count; i++)
        sol->indexes[i] = i;

    return EXIT_SUCCESS;
}

void free_solution(solution_t *sol)
{
    free(sol->indexes);
    free(sol->data);
}

void swap_ind_sol(solution_t *sol, int dst, int src)
{
    int tmp = sol->indexes[dst];
    sol->indexes[dst] = sol->indexes[src];
    sol->indexes[src] = tmp;
}

double **allocate_matrix(int n, int m)
{
    double **data = malloc(n * sizeof(double *) + n * m * sizeof(double));

    if (!data)
        return NULL;

    for (int i = 0; i < n; i++)
        data[i] = (double *)((char *)data + n * sizeof(double *) + i * m * sizeof(double));

    return data;
}

int read_dimensions(int *n, int *m, FILE *f)
{
    if (fscanf(f, "%d %d", n, m) != COUNT_DIM)
        return ERR_READ_DIM;

    if (*n <= 0 || *m <= 0)
        return ERR_INVALID_DIM;

    return EXIT_SUCCESS;
}

int check_eq_dim(matrix_t *m1, matrix_t *m2)
{
    if (m1->m == m2->m && m1->n == m2->n)
        return EXIT_SUCCESS;

    return ERR_NO_EQ_DIM;
}

int check_mult_dim(matrix_t *m1, matrix_t *m2)
{
    if (m1->m == m2->n)
        return EXIT_SUCCESS;

    return ERR_NO_EQ_DIM;
}

int read_matrix(matrix_t *matrix, char *filename)
{
    int rc = EXIT_SUCCESS;
    int rc_err = EXIT_SUCCESS;

    int n, m;

    FILE *f = fopen(filename, "r");

    if (f == NULL)
    {
        rc = EXIT_FAILURE;
        rc_err = ERR_OPEN_FILE;
    }

    rc = rc || (rc_err = read_dimensions(&n, &m, f));

    if (!rc)
    {
        matrix->n = n;
        matrix->m = m;
        matrix->data = allocate_matrix(n, m);

        if (!matrix->data)
        {
            rc = EXIT_FAILURE;
            rc_err = ERR_ALLOCATE;
        }

        for (int i = 0; i < matrix->n; i++)
            for (int j = 0; j < matrix->m; j++)
                if (fscanf(f, "%lf", &matrix->data[i][j]) != 1)
                {
                    rc = EXIT_FAILURE;
                    rc_err = ERR_READ_ELEM;
                }
    }

    double buf;

    if (!rc && fscanf(f, "%lf", &buf) == 1)
    {
        rc_err = ERR_COUNT_ELEM;
        rc = EXIT_FAILURE;
    }

    if (rc_err != ERR_OPEN_FILE)
        fclose(f);

    return rc_err;
}

void multip_matrix(matrix_t *m1, matrix_t *m2, matrix_t *mult)
{
    for (int i = 0; i < m1->n; i++)
    {
        for (int j = 0; j < m2->m; j++)
        {
            mult->data[i][j] = 0;
            for (int k = 0; k < m1->m; k++)
                mult->data[i][j] += m1->data[i][k] * m2->data[k][j];
        }
    }
}

int create_multip_matrix(matrix_t *m1, matrix_t *m2, matrix_t *mult)
{
    if (check_mult_dim(m1, m2) != EXIT_SUCCESS)
        return ERR_INV_MULT_DIM;

    mult->n = m1->n;
    mult->m = m2->m;

    mult->data = allocate_matrix(mult->n, mult->m);

    if (mult->data == NULL)
        return ERR_MEM;

    multip_matrix(m1, m2, mult);

    return EXIT_SUCCESS;
}

void add_matrix(matrix_t *m1, matrix_t *m2, matrix_t *add_result)
{
    for (int i = 0; i < m1->n; i++)
        for (int j = 0; j < m1->m; j++)
            add_result->data[i][j] = m1->data[i][j] + m2->data[i][j];
}

int create_add_matrix(matrix_t *m1, matrix_t *m2, matrix_t *add_result)
{
    if (check_eq_dim(m1, m2) != EXIT_SUCCESS)
        return ERR_NO_EQ_DIM;

    add_result->m = m1->m;
    add_result->n = m1->n;

    add_result->data = allocate_matrix(m1->n, m1->m);

    if (add_result->data == NULL)
        return ERR_MEM;

    add_matrix(m1, m2, add_result);

    return EXIT_SUCCESS;
}

int print_matrix(matrix_t matrix, char *filename)
{
    FILE *f = fopen(filename, "w");

    if (f == NULL)
        return ERR_OPEN_FILE;

    fprintf(f, "%d %d\n", matrix.n, matrix.m);

    for (int i = 0; i < matrix.n; i++)
        for (int j = 0; j < matrix.m; j++)
            fprintf(f, (j == matrix.m - 1) ? "%f\n" : "%f ", matrix.data[i][j]);

    fclose(f);

    return EXIT_SUCCESS;
}



int get_col_max(matrix_t *matrix, int row)
{
    double max = matrix->data[row][0];
    // *row = 0;
    int col = row;

    for (int j = 0; j < matrix->n; j++)
        if (matrix->data[row][j] > max)
        {
            max = matrix->data[row][j];
            col = j;
        }

    return col;
}

void delete_col(matrix_t *matrix, int ind)
{
    if (ind != matrix->m - 1)
        for (int i = 0; i < matrix->n; i++)
            memmove(matrix->data[i] + ind, matrix->data[i] + ind + 1, (matrix->m - ind - 1) * sizeof(double));

    for (int i = 1; i < matrix->n; i++)
    {
        matrix->data[i] -= i;
        memmove(matrix->data[i], matrix->data[i] + i, (matrix->m - 1) * sizeof(double));
    }

    matrix->m--;

    for (int i = 0; i < matrix->n; i++)
        matrix->data[i] = (double *)((char *)matrix->data + matrix->n * sizeof(double *) + i * matrix->m * sizeof(double));
}

void delete_row(matrix_t *matrix, int ind)
{
    if (ind != matrix->n - 1)
        memmove(matrix->data[ind], matrix->data[ind + 1], ((matrix->n - ind - 1) * matrix->m * sizeof(double)));

    memmove(matrix->data + ind, matrix->data + ind + 1, sizeof(double *) * (matrix->n - ind - 1) + (matrix->n - 1) * matrix->m * sizeof(double));

    matrix->n--;

    for (int i = 0; i < matrix->n; i++)
        matrix->data[i] = (double *)((char *)matrix->data + matrix->n * sizeof(double *) + i * matrix->m * sizeof(double));
}

void transform_matrix_col(matrix_t *matrix, int row, int col)
{
    for (int i = 0; i < matrix->n; i++)
    {
        if (i != row)
        {
            double coef = -matrix->data[i][col] / matrix->data[row][col];

            for (int j = 0; j < matrix->m; j++)
            {
                matrix->data[i][j] += coef * matrix->data[row][j];
            }
        }
    }
}

void swap_col(matrix_t *matrix, int col_dst, int col_src)
{
    for (int i = 0; i < matrix->n; i++)
    {
        double tmp = matrix->data[i][col_dst];
        matrix->data[i][col_dst] = matrix->data[i][col_src];
        matrix->data[i][col_src] = tmp;
    }
}

int slae_solution(matrix_t *matrix, solution_t *solution)
{
    for (int i = 0; i < matrix->n; i++)
    {
        int ind = get_col_max(matrix, i);

        if (fabs(matrix->data[i][ind]) > EPS)
        {
            swap_col(matrix, i, ind);
            swap_ind_sol(solution, i, ind);
            transform_matrix_col(matrix, i, i);
        }
        else
            return ERR_SOLUTION;
    }

    for (int i = 0; i < matrix->n; i++)
        if (fabs(matrix->data[i][i]) > EPS)
            solution->data[i] = matrix->data[i][matrix->m - 1] / matrix->data[i][i];
        else
            return ERR_SOLUTION;

    return EXIT_SUCCESS;
}

int print_slae_sol(solution_t *sol, char *filename)
{
    FILE *f = fopen(filename, "w");

    if (f == NULL)
        return ERR_OPEN_FILE;

    fprintf(f, "%d 1\n", sol->count);

    for (int i = 0; i < sol->count; i++)
        fprintf(f, "%f\n", sol->data[sol->indexes[i]]);

    fclose(f);

    return EXIT_SUCCESS;
}