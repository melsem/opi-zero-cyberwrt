#ifndef V_BUFFER_H
#define V_BUFFER_H

#define VOL_TIME_FILE	"/dev/shm/thd_last_vol_time"
#define SCAN_TIME_FILE	"/dev/shm/thd_last_scan_time"

#define DEFAULT_VOL_REPEAT	200
#define DEFAULT_SCAN_REPEAT	200
#define DEFAULT_CE_PRESS	200

long long get_current_ms();
int handle_v_state_2(const char *button_name, const char *l_command);
int handle_v_state_1(const char *button_name, int ce_press, const char *k_command);

#endif // V_BUFFER_H
