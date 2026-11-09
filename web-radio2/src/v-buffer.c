#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "v-buffer.h"

int log_enabled = 0;

#define LOG_PRINT(fmt, ...) \
    do { \
        if (log_enabled) { \
            printf(fmt, ##__VA_ARGS__); \
            fflush(stdout); \
        } \
    } while (0)

long long get_current_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)(tv.tv_sec) * 1000 + (tv.tv_usec / 1000);
}

static int is_audio_button(const char *name) {
    return (strcmp(name, "vol+") == 0 || strcmp(name, "vol-") == 0 ||
            strcmp(name, "subtrebl+") == 0 || strcmp(name, "subtrebl-") == 0 ||
            strcmp(name, "subbas+") == 0 || strcmp(name, "subbas-") == 0 ||
            strcmp(name, "bass+") == 0 || strcmp(name, "bass-") == 0 ||
            strcmp(name, "treble+") == 0 || strcmp(name, "treble-") == 0);
}

int handle_v_state_2(const char *button_name, const char *l_command) {
    long long current_time = get_current_ms();
    long long last_exec_time = 0;
    
    const char *file_path = is_audio_button(button_name) ? VOL_TIME_FILE : SCAN_TIME_FILE;
    int repeat_delay = is_audio_button(button_name) ? DEFAULT_VOL_REPEAT : DEFAULT_SCAN_REPEAT;

    FILE *f_t = fopen(file_path, "r");
    if (f_t) {
        if (fscanf(f_t, "%lld", &last_exec_time) != 1) {
            last_exec_time = 0;
        }
        fclose(f_t);
    }

    if ((current_time - last_exec_time) < repeat_delay) {
        return 0; 
    }

    f_t = fopen(file_path, "w");
    if (f_t) {
        fprintf(f_t, "%lld", current_time);
        fclose(f_t);
    }

    if (l_command != NULL && strlen(l_command) > 0) {
        LOG_PRINT("[LOG] Автоповтор кнопки %s (паузу в %d мс пройдено): %s\n", button_name, repeat_delay, l_command);
        system(l_command);
    }
    return 0;
}

int handle_v_state_1(const char *button_name, int ce_press, const char *k_command) {
    long long current_time = get_current_ms();
    long long last_time = 0;
    const char *file_path = is_audio_button(button_name) ? VOL_TIME_FILE : SCAN_TIME_FILE;

    FILE *f_t = fopen(file_path, "r");
    if (f_t) {
        if (fscanf(f_t, "%lld", &last_time) == 1) {
            if ((current_time - last_time) < ce_press) {
                LOG_PRINT("[LOG] Спрацював захист від брязкоту кнопки %s (-p). Ігноруємо.\n", button_name);
                fclose(f_t);
                return 0;
            }
        }
        fclose(f_t);
    }

    f_t = fopen(file_path, "w");
    if (f_t) {
        fprintf(f_t, "%lld", current_time);
        fclose(f_t);
    }

    if (k_command != NULL && strlen(k_command) > 0) {
        LOG_PRINT("[LOG] Миттєве виконання для %s (-k): %s\n", button_name, k_command);
        system(k_command);
    }
    return 0;
}

int main(int argc, char **argv) {
    char button_name[32] = {0};
    char *k_command = NULL;
    char *l_command = NULL;
    int state = 0;
    int ce_press = DEFAULT_CE_PRESS;
    int opt;

    while ((opt = getopt(argc, argv, "v:k:l:p:s:r?")) != -1) {
        switch (opt) {
            case 's': state = atoi(optarg); break;
            case 'v': strncpy(button_name, optarg, sizeof(button_name) - 1); break;
            case 'k': k_command = optarg; break;
            case 'l': l_command = optarg; break;
            case 'p': ce_press = atoi(optarg); break;
            case 'r':
            case '?': log_enabled = 1; break;
            default: return 1;
        }
    }

    int is_audio = is_audio_button(button_name);
    int is_scan = (strcmp(button_name, "scan+") == 0 || strcmp(button_name, "scan-") == 0);

    if ((!is_audio && !is_scan) || (state != 1 && state != 2)) {
        LOG_PRINT("[LOG] Перевірка параметрів НЕ ПРОЙШЛА! (button=%s, state=%d)\n", button_name, state);
        return 1;
    }

    if (state == 1 && k_command == NULL) return 1;
    if (state == 2 && l_command == NULL) return 1;

    if (state == 2) {
        return handle_v_state_2(button_name, l_command);
    } else {
        return handle_v_state_1(button_name, ce_press, k_command);
    }
}
