/*
** EPITECH PROJECT, 2024
** B-PSU-100-LIL-1-1-mytop-nathan.cheynet
** File description:
** top.h
*/

#include <pwd.h>
#include <sys/sysinfo.h>
#include <sys/types.h>

#ifndef TOP_H_
    #define TOP_H_

    #define MAX_PROCESSES 1024

typedef struct process_s {
    pid_t pid;
    char user[32];
    int priority;
    int nice_value;
    unsigned long virt;
    unsigned long res;
    unsigned long shr;
    char state;
    float cpu_usage;
    float mem_usage;
    char command[256];
    struct process_s *next;
} process_t;

typedef struct options_s {
    char user[32];
    float loads[3];
    long uptime;
    unsigned seconds;
    unsigned frames;
    process_t *head;
} options_t;

int my_top(options_t top);
int refresh_top(options_t *top, unsigned *prev_cpu_stats);
int my_top_core_loop(options_t *top, unsigned *prev_cpu_stats);

void initialize_top_options(int argc, const char *argv[], options_t *top);
void initialize_default_top_settings(options_t *top);
void free_process_list(process_t *head);
options_t initialize_top(int argc, const char *argv[]);

void update_system(options_t *top);
void update_process_list(options_t *top);

void display_processes(const options_t *top);
void display_header(const options_t *top, unsigned *prev_cpu_stats);

void resolve_user_from_uid(uid_t uid, char *user, size_t user_len);
int fetch_rtc_time(int *hour, int *minute, int *second);
int fetch_process_data(const char *pid_dir, process_t **process);

void compute_memory_usage(const unsigned long *mem_stats,
    float *mem_usage);
void compute_cpu_usage(const unsigned *prev_stats,
    const unsigned *curr_stats, float *usage);
int get_cpu_stats(unsigned *curr_stats);
int get_meminfo(unsigned long *mem_stats, FILE *file);
long get_uptime(void);

int validate_frame_count(const char *args);
int validate_seconds_interval(const char *s);
int validate_username(const char *args);
int validate_option(const char *argv[], int *index);
int validate_arguments(int argc, const char *argv[]);

#endif
