/*
** EPITECH PROJECT, 2024
** B-PSU-100-LIL-1-1-mytop-nathan.cheynet
** File description:
** manage_errors.c
*/

/*
** EPITECH PROJECT, 2024
** B-PSU-100-LIL-1-1-mytop-nathan.cheynet
** File description:
** manage_errors.c
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

int validate_frame_count(char const *args)
{
    int i = (args && args[0] != '-') ? 0 : 2;

    if (!args || *args == '\0') {
        fprintf(stderr, "top: option requires an argument -- 'n'\n");
        return FAILURE;
    }
    if ((args[1] == 'n' && args[2] == '-') ||
            (args[0] == '-' && args[1] != 'n')) {
        fprintf(stderr, "top: -n requires positive argument.\n");
        return FAILURE;
    }
    for (; args[i] != '\0'; i++) {
        if (!isdigit(args[i])) {
            fprintf(stderr, "top: bad iterations argument '%s'\n",
                (args[0] != '-') ? args : &args[2]);
            return FAILURE;
        }
    }
    return SUCCESS;
}

int validate_seconds_interval(char const *s)
{
    int dot_count = 0;
    int i = (s && s[0] != '-') ? 0 : 2;

    if (!s || *s == '\0') {
        fprintf(stderr, "top: option requires an argument -- 'd'\n");
        return FAILURE;
    }
    if ((s[1] == 'd' && s[2] == '-') || (s[0] == '-' && s[1] != 'd')) {
        fprintf(stderr, "top: -d requires positive argument.\n");
        return FAILURE;
    }
    for (; s[i] != '\0'; i++) {
        if ((!isdigit(s[i]) && s[i] != '.') || dot_count > 1) {
            fprintf(stderr, "top: bad delay interval '%s'\n",
                (s[0] != '-') ? s : &s[2]);
            return FAILURE;
        }
        dot_count += (s[i] == '.') ? 1 : 0;
    }
    return SUCCESS;
}

int validate_username(char const *args)
{
    FILE *file = fopen("/etc/passwd", "r");
    char buffer[256];
    char *token = NULL;

    if (!file) {
        fprintf(stderr, "top: Unable to validate user\n");
        return FAILURE;
    }
    while (fgets(buffer, sizeof(buffer), file)) {
        token = strtok(buffer, ":");
        if (token && strcmp(token, args) == 0) {
            fclose(file);
            return SUCCESS;
        }
    }
    fclose(file);
    fprintf(stderr, "top: Invalid user\n");
    return FAILURE;
}

int validate_option(char const *argv[], int *index)
{
    if (argv[*index][1] == 'n') {
        *index += (argv[*index][2] != '\0') ? 0 : 1;
        return validate_frame_count(argv[*index]);
    }
    if (argv[*index][1] == 'd') {
        *index += (argv[*index][2] != '\0') ? 0 : 1;
        return validate_seconds_interval(argv[*index]);
    }
    if (argv[*index][1] == 'U') {
        *index += (argv[*index][2] != '\0') ? 0 : 1;
        return validate_username(argv[*index]);
    }
    fprintf(stderr, "top: invalid option -- '%c'\n", argv[*index][1]);
    return FAILURE;
}

int validate_arguments(int argc, char const *argv[])
{
    int index = 1;

    if (argc == 1)
        return SUCCESS;
    while (index < argc && argv[index] != NULL) {
        if (argv[index][0] != '-') {
            fprintf(stderr, "Error: Options must start with '-'.\n");
            return FAILURE;
        }
        if (validate_option(argv, &index) != SUCCESS)
            return FAILURE;
        index += 1;
    }
    return SUCCESS;
}
