#ifndef CH_BUFFER_H
#define CH_BUFFER_H

#define BUFFER_FILE	"/dev/shm/thd_digit_buffer"
#define LOCK_FILE	"/dev/shm/thd_digit_lock"
#define CMD_FILE	"/dev/shm/thd_digit_cmd"
#define LAST_TIME_FILE	"/dev/shm/thd_last_press_time"
#define HOLD_FLAG_FILE	"/dev/shm/thd_hold_flag"

#define DEFAULT_CE_PRESS	200
#define DEFAULT_DEBOUNCE_HOLD	800
#define DEFAULT_CH_TIMEOUT	1000

extern int log_enabled;

#define LOG_PRINT(fmt, ...) \
    do { \
        if (log_enabled) { \
            printf(fmt, ##__VA_ARGS__); \
            fflush(stdout); \
        } \
    } while (0)

long long get_current_ms();
int handle_state_2(char digit, int debounce_hold, char *l_command);
int handle_state_1(char digit, int ce_press, int ch_timeout, char *k_command, char *l_command);

#endif // CH_BUFFER_H
