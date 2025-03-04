/*
** EPITECH PROJECT, 2024
** B-PSU-100-LIL-1-1-mytop-nathan.cheynet
** File description:
** manage_header.c
*/

#include <stdio.h>

void compute_memory_usage(const unsigned long *mem_stats,
    float *mem_usage)
{
    unsigned long mem_used;
    unsigned long buff_cache;
    unsigned long swap_used;

    mem_used = mem_stats[0] - mem_stats[1] - mem_stats[3] - mem_stats[4];
    buff_cache = mem_stats[3] + mem_stats[4];
    swap_used = mem_stats[5] - mem_stats[6];
    mem_usage[0] = mem_stats[0] / 1024.0f;
    mem_usage[1] = mem_stats[1] / 1024.0f;
    mem_usage[2] = mem_used / 1024.0f;
    mem_usage[3] = buff_cache / 1024.0f;
    mem_usage[4] = mem_stats[5] / 1024.0f;
    mem_usage[5] = swap_used / 1024.0f;
    mem_usage[6] = mem_stats[2] / 1024.0f;
}

void compute_cpu_usage(const unsigned *prev_stats,
    const unsigned *curr_stats, float *usage)
{
    unsigned total_diff = 0;
    unsigned idle_diff = curr_stats[3] - prev_stats[3];

    for (int i = 0; i < 10; i++)
        total_diff += curr_stats[i] - prev_stats[i];
    usage[0] = ((curr_stats[0] - prev_stats[0]) / (float)total_diff) * 100.0f;
    usage[1] = ((curr_stats[2] - prev_stats[2]) / (float)total_diff) * 100.0f;
    usage[2] = ((curr_stats[1] - prev_stats[1]) / (float)total_diff) * 100.0f;
    usage[3] = ((idle_diff) / (float)total_diff) * 100.0f;
    usage[4] = ((curr_stats[4] - prev_stats[4]) / (float)total_diff) * 100.0f;
    usage[5] = ((curr_stats[5] - prev_stats[5]) / (float)total_diff) * 100.0f;
    usage[6] = ((curr_stats[6] - prev_stats[6]) / (float)total_diff) * 100.0f;
    usage[7] = ((curr_stats[7] - prev_stats[7]) / (float)total_diff) * 100.0f;
}

int get_cpu_stats(unsigned *curr_stats)
{
    FILE *file = fopen("/proc/stat", "r");
    char buffer[256];

    if (!file)
        return (-1);
    if (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "cpu %u %u %u %u %u %u %u %u %u %u",
            &curr_stats[0], &curr_stats[1], &curr_stats[2], &curr_stats[3],
            &curr_stats[4], &curr_stats[5], &curr_stats[6], &curr_stats[7],
            &curr_stats[8], &curr_stats[9]);
    } else {
        fclose(file);
        return (-1);
    }
    fclose(file);
    return (0);
}

int get_meminfo(unsigned long *mem_stats, FILE *file)
{
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "MemTotal: %lu kB", &mem_stats[0]) == 1)
            continue;
        if (sscanf(buffer, "MemFree: %lu kB", &mem_stats[1]) == 1)
            continue;
        if (sscanf(buffer, "MemAvailable: %lu kB", &mem_stats[2]) == 1)
            continue;
        if (sscanf(buffer, "Buffers: %lu kB", &mem_stats[3]) == 1)
            continue;
        if (sscanf(buffer, "Cached: %lu kB", &mem_stats[4]) == 1)
            continue;
        if (sscanf(buffer, "SwapTotal: %lu kB", &mem_stats[5]) == 1)
            continue;
        if (sscanf(buffer, "SwapFree: %lu kB", &mem_stats[6]) == 1)
            continue;
    }
    fclose(file);
    return (0);
}

long get_uptime(void)
{
    long uptime = 0;
    char buffer[256];
    FILE *file;

    file = fopen("/proc/uptime", "r");
    if (file && fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%ld", &uptime);
        fclose(file);
        return uptime;
    }
    fclose(file);
    return -1;
}
