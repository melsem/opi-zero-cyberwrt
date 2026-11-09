#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ch-buffer.h"

/* Визначення макросу для керування логами в main */
#define LOG_PRINT(fmt, ...) \
    do { \
        if (log_enabled) { \
            printf(fmt, ##__VA_ARGS__); \
            fflush(stdout); \
        } \
    } while (0)

int main(int argc, char **argv) {
    char digit = '\0';
    char *k_command = NULL;
    char *l_command = NULL;
    int state = 0;
    
    int ce_press = DEFAULT_CE_PRESS;
    int debounce_hold = DEFAULT_DEBOUNCE_HOLD;
    int ch_timeout = DEFAULT_CH_TIMEOUT;
    
    int opt;

    while ((opt = getopt(argc, argv, "c:k:l:p:h:t:s:r?")) != -1) {
        switch (opt) {
            case 's': state = atoi(optarg); break;
            case 'c':
                if (strlen(optarg) > 0) {
                    digit = optarg[0];
                }
                break;
            case 'k': k_command = optarg; break;
            case 'l': l_command = optarg; break;
            case 'p': ce_press = atoi(optarg); break;
            case 'h': debounce_hold = atoi(optarg); break;
            case 't': ch_timeout = atoi(optarg); break;
            case 'r':
            case '?': log_enabled = 1; break;
            default: return 1;
        }
    }

    if (digit < '0' || digit > '9' || k_command == NULL || (state != 1 && state != 2)) {
        LOG_PRINT("[LOG] Перевірка параметрів НЕ ПРОЙШЛА! Виліт. (digit=%c, state=%d)\n", digit, state);
        return 1;
    }

    if (state == 2) {
        return handle_state_2(digit, debounce_hold, l_command);
    } else if (state == 1) {
        return handle_state_1(digit, ce_press, ch_timeout, k_command, l_command);
    }

    return 0;
}
