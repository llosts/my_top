/*
** EPITECH PROJECT, 2024
** B-PSU-100-LIL-1-1-mytop-nathan.cheynet
** File description:
** manage_display.c
*/

#include <ncurses.h>
#include <time.h>
#include <utmp.h>
#include <string.h>

#include "top.h"

static void count_tasks(char state, int *task)
{
    switch (state) {
        case 'R':
            task[0] += 1;
            break;
        case 'T':
            task[2] += 1;
            break;
        case 'Z':
            task[3] += 1;
            break;
        default:
            break;
    }
}

static int display_uptime(int cols)
{
    long uptime = get_uptime();
    int days = uptime / 86400;
    int hours = (uptime % 86400) / 3600;
    int minutes = (uptime % 3600) / 60;

    if (uptime == -1) {
        mvprintw(0, cols, "up ??:??,");
        return 10;
    }
    if (days > 0) {
        mvprintw(0, cols, "up %d day%s, %2d:%2d,", days,
            (days > 1 ? "s" : ""), hours, minutes);
        return 18;
    }
    if (hours > 0) {
        mvprintw(0, cols, "up %2d:%02d,", hours, minutes);
        return 10;
    }
    mvprintw(0, cols, "up %2d min,", minutes);
    return 11;
}

static void display_time(int *cols)
{
    int hour = 0;
    int minute = 0;
    int second = 0;
    char time_str[9];
    const char *prefix = "top - ";

    if (fetch_rtc_time(&hour, &minute, &second) == -1) {
        mvprintw(0, *cols, "%s<invalid time>", prefix);
        *cols += strlen(prefix) + strlen("<invalid time>");
        return;
    }
    hour = (hour + 1) % 24;
    snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d",
        hour, minute, second);
    mvprintw(0, *cols, "%s%s", prefix, time_str);
    *cols += strlen(prefix) + strlen(time_str) + 1;
}

static void display_users(int *cols)
{
    struct utmp entry;
    int user_count = -1;
    FILE *file = fopen("/var/run/utmp", "r");

    if (!file) {
        mvprintw(0, *cols, " 0 user,");
        return;
    }
    while (fread(&entry, sizeof(entry), 1, file)) {
        if (entry.ut_type == USER_PROCESS)
            user_count += 1;
    }
    if (user_count == -1)
        user_count += 1;
    fclose(file);
    mvprintw(0, *cols, " %d user%s,", user_count, (user_count > 1) ? "s" : "");
    *cols += 9 + (user_count > 1 ? 1 : 0);
}

static void display_task_summary(const options_t *top)
{
    int total = 0;
    int task[] = {0, 0, -1, 0};
    process_t *current = top->head;

    while (current) {
        total += 1;
        count_tasks(current->state, task);
        current = current->next;
    }
    task[1] = total - (task[0] + task[2] + task[3]);
    mvprintw(1, 0,
        "Tasks: %d total, %3d running, %3d sleeping, %3d stopped, %3d zombie",
        total, task[0], task[1], task[2], task[3]);
}

static void display_cpu_usage(unsigned *prev_stats)
{
    unsigned curr_stats[10] = {0};
    float cpu_usage[8] = {0};

    if (get_cpu_stats(curr_stats) == -1)
        return;
    compute_cpu_usage(prev_stats, curr_stats, cpu_usage);
    memcpy(prev_stats, curr_stats, sizeof(curr_stats));
    mvprintw(2, 0, "%%Cpu(s): %4.1f us, %4.1f sy, %4.1f ni, %4.1f id,",
        cpu_usage[0], cpu_usage[1], cpu_usage[2], cpu_usage[3]);
    mvprintw(2, 44, " %4.1f wa, %4.1f hi, %4.1f si, %4.1f st",
        cpu_usage[4], cpu_usage[5], cpu_usage[6], cpu_usage[7]);
}

static void display_memory_swap_usage(void)
{
    unsigned long mem_stats[7] = {0};
    float mem_usage[7] = {0};
    FILE *file = fopen("/proc/meminfo", "r");

    if (!file) {
        mvprintw(3, 0, "Error: Unable to read memory info");
        return;
    }
    get_meminfo(mem_stats, file);
    compute_memory_usage(mem_stats, mem_usage);
    mvprintw(3, 0,
        "MiB Mem : %8.1f total, %8.1f free, %8.1f used, %8.1f buff/cache",
        mem_usage[0], mem_usage[1], mem_usage[2], mem_usage[3]);
    mvprintw(4, 0,
        "MiB Swap: %8.1f total, %8.1f free, %8.1f used. %8.1f avail Mem",
        mem_usage[4], mem_usage[4], mem_usage[5], mem_usage[6]);
}

void display_header(const options_t *top, unsigned *prev_cpu_stats)
{
    int cols = 0;

    display_time(&cols);
    cols += display_uptime(cols);
    display_users(&cols);
    mvprintw(0, cols, " load average: %.2f, %.2f, %.2f",
        top->loads[0],
        top->loads[1],
        top->loads[2]);
    display_task_summary(top);
    display_cpu_usage(prev_cpu_stats);
    display_memory_swap_usage();
}

void display_processes(const options_t *top)
{
    process_t *current = top->head;
    int r = 7;

    mvprintw(r - 1, 0, "%7s %s %7s %3s %7s %6s %6s S %5s %5s %9s %s",
        "PID", "USER", "PR", "NI", "VIRT", "RES", "SHR",
        "%CPU", "%MEM", "TIME+", "COMMAND");
    while (current && r < LINES - 1) {
        mvprintw(r, 0, "%7d %-7s %4d %3d %7lu %6lu %6lu %c %5.2f %5.2f %9s %s",
            current->pid, current->user, current->priority,
            current->nice_value, current->virt, current->res, current->shr,
            current->state, current->cpu_usage, current->mem_usage, "time",
            current->command);
        current = current->next;
        r++;
    }
}
