#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "matrix.h"

#define ADD_ACTION 0
#define MULTIP_ACTION 1
#define TASK_ACTION 2

int define_action(int argc, char *argv);

int main(int argc, char **argv)
{
    int rc = EXIT_SUCCESS;
    int rc_err = EXIT_SUCCESS;

    int action = define_action(argc, argv[1]);

    matrix_t m1, m2, add_res, mult_res;
    m1.data = NULL;
    m2.data = NULL;
    add_res.data = NULL;
    mult_res.data = NULL;
    solution_t sol;
    sol.indexes = NULL;
    sol.data = NULL;

    char output_file_name[64];
    printf("Input output file name: \n");
    scanf("%s", output_file_name);
    output_file_name[strlen(output_file_name)] = '\0';

    switch (action)
    {
        case ADD_ACTION:
            rc = rc || (rc_err = read_matrix(&m1, argv[2]));
            rc = rc || (rc_err = read_matrix(&m2, argv[3]));
            rc = rc || (rc_err = create_add_matrix(&m1, &m2, &add_res));
            rc = rc || (rc_err = print_matrix(add_res, output_file_name));
            break;
        case MULTIP_ACTION:
            rc = rc || (rc_err = read_matrix(&m1, argv[2]));
            rc = rc || (rc_err = read_matrix(&m2, argv[3]));
            rc = rc || (rc_err = create_multip_matrix(&m1, &m2, &mult_res));
            // rc = rc || (rc_err = print_matrix(mult_res, argv[4]));
            break;
        case TASK_ACTION:
            rc = rc || (rc_err = read_matrix(&m1, argv[2]));
            rc = rc || (rc_err = (m1.m != m1.n + 1) ? ERR_DIM_GAUS_METH : EXIT_SUCCESS);
            rc = rc || (rc_err = init_solution(&sol, m1.n));
            rc = rc || (rc_err = slae_solution(&m1, &sol));
            // rc = rc || (rc_err = print_slae_sol(&sol, argv[3]));
            free_solution(&sol);
            break;
        default:
            rc = EXIT_FAILURE;
            rc_err = action;
            break;
    }

    free(m1.data);
    free(m2.data);
    free(add_res.data);
    free(mult_res.data);
    
    return rc_err;
}

int define_action(int argc, char *argv)
{
    if (argc < 4)
        return ERR_ARGC_COUNT;

    if (strlen(argv) > 1)
        return ERR_ARGV_ACTION;

    if (argc == 4 && argv[0] == 'o')
        return TASK_ACTION;
    else if (argc == 4 && argv[0] == 'a')
        return ADD_ACTION;
    else if (argc == 5 && argv[0] == 'm')
        return MULTIP_ACTION;
    else
        return ERR_ARGV_ACTION;
}