/*
** EPITECH PROJECT, 2024
** B-PSU-100-LIL-1-1-mytop-nathan.cheynet
** File description:
** manage_initialization.c
*/

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>

#include "error.h"
#include "top.h"

void initialize_top_options(int argc, char const *argv[], options_t *top)
{
    int index = 1;
    char *endptr;

    while (index < argc && argv[index] != NULL) {
        if (argv[index][1] == 'U') {
            index += (argv[index][2] != '\0') ? 0 : 1;
            resolve_user_from_uid((argv[index][0] != '-') ? atoi(argv[index]) :
                atoi(&argv[index][2]), top->user, sizeof(top->user));
        }
        if (argv[index][1] == 'd') {
            index += (argv[index][2] != '\0') ? 0 : 1;
            top->seconds = strtoul(argv[index], &endptr, 10);
        }
        if (argv[index][1] == 'n') {
            index += (argv[index][2] != '\0') ? 0 : 1;
            top->frames = strtoul(argv[index], &endptr, 10);
        }
        index += 1;
    }
}

void initialize_default_top_settings(options_t *top)
{
    memset(top, 0, sizeof(options_t));
    top->seconds = 3;
    top->frames = -1;
    top->head = NULL;
}

void free_process_list(process_t *head)
{
    process_t *current = head;
    process_t *next = NULL;

    while (current) {
        next = current->next;
        if (current)
            free(current);
        current = next;
    }
}

options_t initialize_top(int argc, char const *argv[])
{
    options_t top;

    initialize_default_top_settings(&top);
    initialize_top_options(argc, argv, &top);
    update_system(&top);
    update_process_list(&top);
    return top;
}
