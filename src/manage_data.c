/*
** EPITECH PROJECT, 2024
** B-PSU-100-LIL-1-1-mytop-nathan.cheynet
** File description:
** manage_data.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pwd.h>

#include "top.h"

static int fetch_process_user_info(const char *pid_dir, process_t *process)
{
    char path[64];
    FILE *file;
    char buffer[256];
    uid_t uid;

    snprintf(path, sizeof(path), "/proc/%s/status", pid_dir);
    file = fopen(path, "r");
    if (!file)
        return (-1);
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "Uid:", 4) == 0 &&
                sscanf(buffer, "Uid:\t%d", &uid) == 1) {
            resolve_user_from_uid(uid, process->user, sizeof(process->user));
            break;
        }
    }
    fclose(file);
    return 0;
}

static int fetch_process_basic_info(const char *pid_dir, process_t *process)
{
    char path[64];
    FILE *file;

    snprintf(path, sizeof(path), "/proc/%s/stat", pid_dir);
    file = fopen(path, "r");
    if (!file)
        return -1;
    if (fscanf(file, "%d %*s %c", &process->pid, &process->state) != 2) {
        fclose(file);
        return -1;
    }
    fclose(file);
    return 0;
}

static int fetch_process_memory_info(const char *pid_dir, process_t *proc)
{
    char path[64];
    FILE *file;

    snprintf(path, sizeof(path), "/proc/%s/statm", pid_dir);
    file = fopen(path, "r");
    if (!file)
        return -1;
    if (fscanf(file, "%lu %lu %lu", &proc->virt, &proc->res, &proc->shr)
            != 3) {
        fclose(file);
        return -1;
    }
    fclose(file);
    return 0;
}

static int fetch_process_command_name(const char *pid_dir, process_t *proc)
{
    char path[64];
    FILE *file;

    snprintf(path, sizeof(path), "/proc/%s/comm", pid_dir);
    file = fopen(path, "r");
    if (file) {
        if (!fgets(proc->command, sizeof(proc->command), file))
            snprintf(proc->command, sizeof(proc->command), "[%s]", pid_dir);
        fclose(file);
    } else
        snprintf(proc->command, sizeof(proc->command), "[%s]", pid_dir);
    return 0;
}

static char *fetch_user_entry_by_uid(uid_t uid, char *buffer,
    size_t buffer_len)
{
    FILE *file = fopen("/etc/passwd", "r");
    char *user_uid = NULL;

    if (!file)
        return NULL;
    while (fgets(buffer, buffer_len, file)) {
        user_uid = strstr(buffer, ":");
        user_uid = strstr(user_uid + 1, ":");
        user_uid = strstr(user_uid + 1, ":");
        if (user_uid && atoi(user_uid + 1) == (int)uid) {
            fclose(file);
            return buffer;
        }
    }
    fclose(file);
    return NULL;
}

void resolve_user_from_uid(uid_t uid, char *user, size_t user_len)
{
    char buffer[256];
    char *entry = fetch_user_entry_by_uid(uid, buffer, sizeof(buffer));
    char *token = NULL;

    if (entry) {
        token = strtok(entry, ":");
        strncpy(user, token, user_len - 1);
        user[user_len - 1] = '\0';
    } else
        snprintf(user, user_len, "%d", uid);
}

int fetch_rtc_time(int *hour, int *minute, int *second)
{
    FILE *file = fopen("/proc/driver/rtc", "r");
    char buffer[256];

    if (!file)
        return -1;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "rtc_time", 8) == 0) {
            sscanf(buffer, "rtc_time : %2d:%2d:%2d", hour, minute, second);
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return -1;
}

int fetch_process_data(const char *pid_dir, process_t **head)
{
    process_t *new_process = malloc(sizeof(process_t));

    if (!new_process)
        return -1;
    memset(new_process, 0, sizeof(process_t));
    if (fetch_process_basic_info(pid_dir, new_process) == -1 ||
        fetch_process_user_info(pid_dir, new_process) == -1 ||
        fetch_process_memory_info(pid_dir, new_process) == -1 ||
        fetch_process_command_name(pid_dir, new_process) == -1) {
        free(new_process);
        return -1;
    }
    new_process->priority = 20;
    new_process->next = *head;
    *head = new_process;
    return 0;
}
