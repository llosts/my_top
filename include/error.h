/*
** EPITECH PROJECT, 2024
** B-PSU-100-LIL-1-1-mytop-nathan.cheynet
** File description:
** error.h
*/

#ifndef ERROR_H_
    #define ERROR_H_

    #define FAILURE 84
    #define SUCCESS 0
    #define STDERR 2

int isvalid_frames(char const *args);
int isvalid_seconds(char const *args);
int isvalid_username(char const *args);
int isvalid_options(char const *argv[], int *index);
int isvalid_args(int argc, char const *argv[]);

#endif
