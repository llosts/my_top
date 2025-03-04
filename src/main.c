/*
** EPITECH PROJECT, 2024
** B-PSU-100-LIL-1-1-mytop-nathan.cheynet
** File description:
** main.c
*/

#include <stddef.h>

#include "error.h"
#include "top.h"

int main(int argc, char const *argv[])
{
    options_t top;

    if (validate_arguments(argc, argv) != SUCCESS)
        return FAILURE;
    top = initialize_top(argc, argv);
    if (!top.head) {
        free_process_list(top.head);
        return FAILURE;
    }
    if (my_top(top) != SUCCESS) {
        free_process_list(top.head);
        return FAILURE;
    }
    free_process_list(top.head);
    return SUCCESS;
}
