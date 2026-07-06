#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

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
extern int lcd_print_cyr(const unsigned char *utf8_str, int max_chars);

// Утиліта для друку тексту строго по центру екрана
void print_centered(const char* text) {
    int len = strlen_utf8(text);
    int spaces = (lcd_cols - len) / 2;
    if (spaces < 0) spaces = 0;
    
    for (int i = 0; i < spaces; i++) lcd_send(LCD_CHR, ' ');
    int printed = lcd_print_cyr((unsigned char*)text, lcd_cols - spaces);
    int total = spaces + printed;
    while (total < lcd_cols) { lcd_send(LCD_CHR, ' '); total++; }
}

int main() {
    int i2c_dev_num = 0;
    get_uci_display_config(&i2c_dev_num, &lcd_addr, &lcd_cols, &lcd_rows, &pin_backlight, tmp_file_path, vol_file_path, lcd_language); 

    char device_path[32];
    sprintf(device_path, "/dev/i2c-%d", i2c_dev_num);
    if ((lcd_i2c_dev = open(device_path, O_RDWR)) < 0) return 1;
    
    lcd_init();
    
    char current_mode[256] = "";
    char last_mode[256] = "";
    int current_volume = 0;
    int last_volume = -1;
    
    int scroll_pos = 0;
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

    while (1) {
        if (ticks % 4 == 0) {
            char new_mode[256] = "";
            FILE *f = fopen(tmp_file_path, "r");
            if (f) { fgets(new_mode, sizeof(new_mode), f); new_mode[strcspn(new_mode, "\r\n")] = 0; fclose(f); }
            if (strlen(new_mode) == 0) strcpy(new_mode, default_msg);

            if (strcmp(new_mode, current_mode) != 0) {
                strcpy(current_mode, new_mode);
                scroll_pos = 0;
            }

            FILE *f_vol = fopen(vol_file_path, "r");
            if (f_vol) { if (fscanf(f_vol, "%d", &current_volume) != 1) current_volume = 0; fclose(f_vol); }
            if (current_volume < 0) current_volume = 0;
            if (current_volume > 100) current_volume = 100;
        }

        // --- РОБОТА З РЯДКОМ 1 (Динамічна бігуча строка / Статика) ---
        int text_len_chars = strlen_utf8(current_mode);
        lcd_set_cursor(0, 0);

        if (text_len_chars <= lcd_cols) {
            if (strcmp(current_mode, last_mode) != 0 || first_run) {
                print_centered(current_mode);
                strcpy(last_mode, current_mode);
            }
        } else {
            int total_scroll_loop = text_len_chars + lcd_scroll_gap;

            if (scroll_pos < text_len_chars) {
                unsigned char* start_p = get_utf8_offset((unsigned char*)current_mode, scroll_pos);
                int printed = lcd_print_cyr(start_p, lcd_cols);

                if (printed < lcd_cols) {
                    int remaining_space = lcd_cols - printed;
                    int actual_gap_spaces = text_len_chars + lcd_scroll_gap - scroll_pos;
                    if (actual_gap_spaces > remaining_space) actual_gap_spaces = remaining_space;
                    
                    for (int i = 0; i < actual_gap_spaces; i++) { lcd_send(LCD_CHR, ' '); printed++; }
                    if (printed < lcd_cols) { lcd_print_cyr((unsigned char*)current_mode, lcd_cols - printed); }
                }
            } else {
                int spaces_left_to_scroll = total_scroll_loop - scroll_pos;
                int printed = 0;
                for (int i = 0; i < spaces_left_to_scroll && printed < lcd_cols; i++) { lcd_send(LCD_CHR, ' '); printed++; }
                if (printed < lcd_cols) { lcd_print_cyr((unsigned char*)current_mode, lcd_cols - printed); }
            }

            scroll_pos++;
            if (scroll_pos >= total_scroll_loop) scroll_pos = 0; 
            strcpy(last_mode, ""); 
        }

        // --- РОБОТА З РЯДКОМ 2 (PROGRESS BAR З ДУЖКАМИ ТА КОНТРОЛЕМ АДАПТИВНОСТІ) ---
        if (current_volume != last_volume || first_run) {
            lcd_set_cursor(1, 0);
            
            // Текст після закриваючої дужки (наприклад: "45%" або "100%")
            char tail_txt[16];
            snprintf(tail_txt, sizeof(tail_txt), "%d%%", current_volume);
            int tail_len = strlen(tail_txt);

            // Друкуємо початок "VOL [" (займає рівно 5 символів)
            lcd_print_cyr((unsigned char*)"VOL (", 5);

            // Чистий розрахунок довжини шкали: віднімаємо 5 (заголовок) та 1 (дужка ']') та довжину відсотків
            int bar_max_chars = lcd_cols - 5 - 1 - tail_len; 
            int filled_chars = (current_volume * bar_max_chars) / 100;
            
            for (int i = 0; i < bar_max_chars; i++) {
                switch (lcd_vol_style) {
                    case 2: // Стрілочний індикатор
                        lcd_send(LCD_CHR, (i < filled_chars) ? '|' : '.');
                        break;
                    case 3: // Медіаплеєр з бігунком
                        if (filled_chars == 0) lcd_send(LCD_CHR, '-');
                        else lcd_send(LCD_CHR, (i == filled_chars - 1) ? 0xFF : '-');
                        break;
                    case 4: // Сітка аудіо-ресивера
                        lcd_send(LCD_CHR, (i < filled_chars) ? '=' : ' ');
                        break;
                    case 5: // Медіаплеєр зі стрілочкою '>'
                        if (filled_chars == 0) lcd_send(LCD_CHR, '-');
                        else lcd_send(LCD_CHR, (i == filled_chars - 1) ? '>' : '-');
                        break;
                    case 1: // Класичний варіант
                    default:
                        lcd_send(LCD_CHR, (i < filled_chars) ? 0xFF : ' ');
                        break;
                }
            }
            
            // Виводимо закриваючу дужку та відсотки в один рядок
            lcd_send(LCD_CHR, ')');
            lcd_print_cyr((unsigned char*)tail_txt, tail_len);
            
            // Затирання можливих хвостів пробілами до кінця геометрії lcd_cols
            int total_printed = 5 + bar_max_chars + 1 + tail_len;
            while (total_printed < lcd_cols) {
                lcd_send(LCD_CHR, ' ');
                total_printed++;
            }
            
            last_volume = current_volume;
        }

        // --- РОБОТА З РЯДКАМИ 3 ТА 4 ---
        if (lcd_rows == 4 && first_run) {
            lcd_set_cursor(2, 0);
            print_centered("--- Smart Amp ---");
            lcd_set_cursor(3, 0);
            print_centered("OpenWrt System");
        }

        first_run = 0;
        ticks++;
        usleep(250000); 
    }
    
    close(lcd_i2c_dev);
    return 0;
}

