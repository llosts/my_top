/*
** EPITECH PROJECT, 2024
** B-PSU-100-LIL-1-1-mytop-nathan.cheynet
** File description:
** my_top.c
*/

#include <ncurses.h>
#include <unistd.h>

#include "top.h"
#include "error.h"

int my_top_core_loop(options_t *top, unsigned *cpu_stats)
{
    int ch = 0;
    const int check_interval_ms = 50;
    int elapsed_time_ms = 0;

    while (ch != 'q') {
        if (elapsed_time_ms == 0 && refresh_top(top, cpu_stats) != SUCCESS)
            return FAILURE;
        ch = getch();
        napms(check_interval_ms);
        elapsed_time_ms += check_interval_ms;
        if (elapsed_time_ms >= (const int)(top->seconds * 1000))
            elapsed_time_ms = 0;
    }
    return SUCCESS;
}

int my_top(options_t top)
{
    unsigned prev_cpu_stats[10] = {0};

    initscr();
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(0);
    if (my_top_core_loop(&top, prev_cpu_stats) != SUCCESS) {
        endwin();
        return FAILURE;
    }
    endwin();
    return SUCCESS;
}
