/*
** EPITECH PROJECT, 2024
** B-PSU-100-LIL-1-1-mytop-nathan.cheynet
** File description:
** manage_updates.c
*/

#include <ctype.h>
#include <dirent.h>
#include <ncurses.h>

#include "top.h"
#include "error.h"

void update_system(options_t *top)
{
    FILE *file;
    char buffer[256];

    file = fopen("/proc/uptime", "r");
    if (file && fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%ld", &top->uptime);
        fclose(file);
    }
    file = fopen("/proc/loadavg", "r");
    if (file && fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%f %f %f", &top->loads[0], &top->loads[1],
            &top->loads[2]);
        fclose(file);
    }
}

void update_process_list(options_t *top)
{
    DIR *proc_dir = opendir("/proc");
    struct dirent *entry = readdir(proc_dir);

    if (!proc_dir || !entry)
        return;
    free_process_list(top->head);
    top->head = NULL;
    while (entry != NULL) {
        if (!isdigit(entry->d_name[0])) {
            entry = readdir(proc_dir);
            continue;
        }
        if (fetch_process_data(entry->d_name, &top->head) == -1) {
            entry = readdir(proc_dir);
            continue;
        }
        entry = readdir(proc_dir);
    }
    closedir(proc_dir);
}

int refresh_top(options_t *top, unsigned *prev_cpu_stats)
{
    clear();
    update_system(top);
    update_process_list(top);
    display_header(top, prev_cpu_stats);
    display_processes(top);
    refresh();
    return SUCCESS;
}
