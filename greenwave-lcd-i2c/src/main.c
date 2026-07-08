#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>  // Для оптимізації читання через stat()
#include <syslog.h>    // Для запису критичних помилок конфігурації у logread

#define LCD_CHR        1
#define LCD_CMD        0

extern int lcd_i2c_dev;
extern int lcd_addr;
extern int lcd_cols;
extern int lcd_rows;
extern uint8_t pin_backlight;
extern char tmp_file_path[128];
extern char vol_file_path[128];
extern char lcd_language[16];
extern int lcd_scroll_gap;
extern int lcd_vol_style; 

extern void get_uci_display_config(int *dev, int *lcd_addr, int *cols, int *rows, uint8_t *backlight, char *file_path, char *vol_path, char *lang);
extern void lcd_init();
extern void lcd_send(uint8_t mode, uint8_t val);
extern void lcd_set_cursor(int row, int col);
extern int strlen_utf8(const char *str);
extern unsigned char* get_utf8_offset(unsigned char *str, int offset_chars);
extern int lcd_print_cyr(const unsigned char *utf8_str, int max_chars, int is_track);

// Утиліта для друку тексту строго по центру екрана
void print_centered_ext(const char* text, int is_track) {
    int len = strlen_utf8(text);
    int spaces = (lcd_cols - len) / 2;
    if (spaces < 0) spaces = 0;
    
    for (int i = 0; i < spaces; i++) lcd_send(LCD_CHR, ' ');
    int printed = lcd_print_cyr((unsigned char*)text, lcd_cols - spaces, is_track);
    int total = spaces + printed;
    while (total < lcd_cols) { lcd_send(LCD_CHR, ' '); total++; }
}

int main() {
    int i2c_dev_num = 0;
    get_uci_display_config(&i2c_dev_num, &lcd_addr, &lcd_cols, &lcd_rows, &pin_backlight, tmp_file_path, vol_file_path, lcd_language); 

    char device_path[32];
    snprintf(device_path, sizeof(device_path), "/dev/i2c-%d", i2c_dev_num);
    if ((lcd_i2c_dev = open(device_path, O_RDWR)) < 0) return 1;
    
    lcd_init();
    
    char current_mode[256] = "";
    char last_mode[256] = "";
    int scroll_pos = 0;

    int current_volume = 0;
    int last_volume = -1;
    char current_vol_msg[256] = "";
    int scroll_pos_vol = 0; 
    
    int ticks = 0;
    int first_run = 1;

    char default_msg[256] = "";
    char board_name[128] = "OpenWrt";
    char firmware_ver[128] = "";

    FILE *f_board = fopen("/tmp/sysinfo/board_name", "r");
    if (f_board) { if (fgets(board_name, sizeof(board_name), f_board)) { board_name[strcspn(board_name, "\r\n")] = 0; } fclose(f_board); }
    FILE *f_ver = fopen("/etc/openwrt_version", "r");
    if (f_ver) { if (fgets(firmware_ver, sizeof(firmware_ver), f_ver)) { firmware_ver[strcspn(firmware_ver, "\r\n")] = 0; } fclose(f_ver); }
    snprintf(default_msg, sizeof(default_msg), "%s %s", board_name, firmware_ver);

    int tmp_path_ok = (strncmp(tmp_file_path, "/tmp/", 5) == 0);
    int vol_path_ok = (strncmp(vol_file_path, "/tmp/", 5) == 0);

    if (!tmp_path_ok) {
        openlog("greenwave-lcd", LOG_PID, LOG_USER);
        syslog(LOG_ERR, "UCI configuration error: tmp_file '%s' must be located in /tmp/!", tmp_file_path);
        closelog();
    }
    if (!vol_path_ok) {
        openlog("greenwave-lcd", LOG_PID, LOG_USER);
        syslog(LOG_ERR, "UCI configuration error: vol_file '%s' must be located in /tmp/!", vol_file_path);
        closelog();
    }

    time_t last_tmp_mtime = 0;
    time_t last_vol_mtime = 0;

    while (1) {
        if (ticks % 4 == 0) {
            char new_mode[256] = "";
            char new_vol_msg[256] = "";
            struct stat st_tmp;
            struct stat st_vol;

            // ==================== ОБРОБКА ДАНИХ РЯДКА 1 ====================
            if (tmp_path_ok) {
                if (stat(tmp_file_path, &st_tmp) == 0) {
                    if (st_tmp.st_mtime != last_tmp_mtime || first_run) {
                        last_tmp_mtime = st_tmp.st_mtime;
                        FILE *f = fopen(tmp_file_path, "r");
                        if (f) { 
                            if (fgets(new_mode, sizeof(new_mode), f)) {
                                new_mode[strcspn(new_mode, "\r\n")] = 0; 
                            }
                            fclose(f); 
                        }
                    } else {
                        strcpy(new_mode, current_mode);
                    }
                }
                if (strlen(new_mode) == 0) strcpy(new_mode, default_msg);
            } else {
                snprintf(new_mode, sizeof(new_mode), "Config error: change tmp_file to /tmp, otherwise will not read! (path: %s) ", tmp_file_path);
            }

            if (strcmp(new_mode, current_mode) != 0) {
                strcpy(current_mode, new_mode);
                scroll_pos = 0;
                strcpy(last_mode, ""); 
            }

            // ==================== ОБРОБКА ДАНИХ РЯДКА 2 ====================
            if (vol_path_ok) {
                if (stat(vol_file_path, &st_vol) == 0) {
                    if (st_vol.st_mtime != last_vol_mtime || first_run) {
                        last_vol_mtime = st_vol.st_mtime;
                        FILE *f_vol = fopen(vol_file_path, "r");
                        if (f_vol) { 
                            if (fscanf(f_vol, "%d", &current_volume) != 1) current_volume = 0; 
                            fclose(f_vol); 
                        }
                    }
                }
                if (current_volume < 0) current_volume = 0;
                if (current_volume > 100) current_volume = 100;
                strcpy(new_vol_msg, ""); 
            } else {
                snprintf(new_vol_msg, sizeof(new_vol_msg), "Config error: change vol_file to /tmp, otherwise will not read! (path: %s) ", vol_file_path);
            }

            if (strcmp(new_vol_msg, current_vol_msg) != 0) {
                strcpy(current_vol_msg, new_vol_msg);
                scroll_pos_vol = 0;
            }
        }

        // ==================== ВІДОБРАЖЕННЯ РЯДКА 1 ====================
        int text_len_chars = strlen_utf8(current_mode);
        lcd_set_cursor(0, 0);

        int track_flag = 1;
        if (!tmp_path_ok || strcmp(current_mode, default_msg) == 0) {
            track_flag = 0;
        }

        if (text_len_chars <= lcd_cols) {
            if (strcmp(current_mode, last_mode) != 0 || first_run) {
                print_centered_ext(current_mode, track_flag);
                strcpy(last_mode, current_mode);
            }
        } else {
            int total_scroll_loop = text_len_chars + lcd_scroll_gap;
            if (scroll_pos < text_len_chars) {
                unsigned char* start_p = get_utf8_offset((unsigned char*)current_mode, scroll_pos);
                int printed = lcd_print_cyr(start_p, lcd_cols, track_flag);
                if (printed < lcd_cols) {
                    int remaining_space = lcd_cols - printed;
                    int actual_gap_spaces = text_len_chars + lcd_scroll_gap - scroll_pos;
                    if (actual_gap_spaces > remaining_space) actual_gap_spaces = remaining_space;
                    for (int i = 0; i < actual_gap_spaces; i++) { lcd_send(LCD_CHR, ' '); printed++; }
                    if (printed < lcd_cols) { lcd_print_cyr((unsigned char*)current_mode, lcd_cols - printed, track_flag); }
                }
            } else {
                int spaces_left_to_scroll = total_scroll_loop - scroll_pos;
                int printed = 0;
                for (int i = 0; i < spaces_left_to_scroll && printed < lcd_cols; i++) { lcd_send(LCD_CHR, ' '); printed++; }
                if (printed < lcd_cols) { lcd_print_cyr((unsigned char*)current_mode, lcd_cols - printed, track_flag); }
            }
            scroll_pos++;
            if (scroll_pos >= total_scroll_loop) scroll_pos = 0; 
        }

        // ==================== ВІДОБРАЖЕННЯ РЯДКА 2 ====================
        if (!vol_path_ok) {
            int vol_txt_len = strlen_utf8(current_vol_msg);
            lcd_set_cursor(1, 0);
            int total_scroll_vol = vol_txt_len + lcd_scroll_gap;
            if (scroll_pos_vol < vol_txt_len) {
                unsigned char* start_p = get_utf8_offset((unsigned char*)current_vol_msg, scroll_pos_vol);
                // Передаємо 0 (is_track = 0): Латиниця в помилці буде з малими літерами
                int printed = lcd_print_cyr(start_p, lcd_cols, 0);
                if (printed < lcd_cols) {
                    int remaining_space = lcd_cols - printed;
                    int actual_gap_spaces = vol_txt_len + lcd_scroll_gap - scroll_pos_vol;
                    if (actual_gap_spaces > remaining_space) actual_gap_spaces = remaining_space;
                    for (int i = 0; i < actual_gap_spaces; i++) { lcd_send(LCD_CHR, ' '); printed++; }
                    if (printed < lcd_cols) { lcd_print_cyr((unsigned char*)current_vol_msg, lcd_cols - printed, 0); }
                }
            } else {
                int spaces_left_to_scroll = total_scroll_vol - scroll_pos_vol;
                int printed = 0;
                for (int i = 0; i < spaces_left_to_scroll && printed < lcd_cols; i++) { lcd_send(LCD_CHR, ' '); printed++; }
                if (printed < lcd_cols) { lcd_print_cyr((unsigned char*)current_vol_msg, lcd_cols - printed, 0); }
            }
            scroll_pos_vol++;
            if (scroll_pos_vol >= total_scroll_vol) scroll_pos_vol = 0;
            last_volume = -1; 
        } else {
            if (current_volume != last_volume || first_run) {
                lcd_set_cursor(1, 0);
                char tail_txt[16];
                snprintf(tail_txt, sizeof(tail_txt), "%d%%", current_volume);
                int tail_len = strlen(tail_txt);
                lcd_print_cyr((unsigned char*)"VOL (", 5, 0);
                int bar_max_chars = lcd_cols - 5 - 1 - tail_len; 
                if (bar_max_chars < 0) bar_max_chars = 0;
                int filled_chars = 0;
                if (bar_max_chars > 0) filled_chars = (current_volume * bar_max_chars) / 100;
                for (int i = 0; i < bar_max_chars; i++) {
                    switch (lcd_vol_style) {
                        case 2: lcd_send(LCD_CHR, (i < filled_chars) ? '|' : '.'); break;
                        case 3: 
                            if (filled_chars == 0) lcd_send(LCD_CHR, '-');
                            else lcd_send(LCD_CHR, (i == filled_chars - 1) ? 0xFF : '-');
                            break;
                        case 4: lcd_send(LCD_CHR, (i < filled_chars) ? '=' : ' '); break;
                        case 5: 
                            if (filled_chars == 0) lcd_send(LCD_CHR, '-');
                            else lcd_send(LCD_CHR, (i == filled_chars - 1) ? '>' : '-');
                            break;
                        case 1:
                        default: lcd_send(LCD_CHR, (i < filled_chars) ? 0xFF : ' '); break;
                    }
                }
                lcd_send(LCD_CHR, ')');
                lcd_print_cyr((unsigned char*)tail_txt, tail_len, 0);
                int total_printed = 5 + bar_max_chars + 1 + tail_len;
                while (total_printed < lcd_cols) { lcd_send(LCD_CHR, ' '); total_printed++; }
                last_volume = current_volume;
            }
        }

        // ==================== ВІДОБРАЖЕННЯ РЯДКІВ 3 ТА 4 ====================
        if (lcd_rows == 4 && first_run) {
            lcd_set_cursor(2, 0);
            print_centered_ext("--- Smart Amp ---", 0);
            lcd_set_cursor(3, 0);
            print_centered_ext("OpenWrt System", 0);
        }

        first_run = 0;
        ticks++;
        usleep(250000); 
    }
    close(lcd_i2c_dev);
    return 0;
}
