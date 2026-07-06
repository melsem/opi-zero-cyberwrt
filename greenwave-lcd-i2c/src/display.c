#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define LCD_CHR        1
#define LCD_CMD        0
#define PIN_ENABLE     0x04

#define UCI_CFG "uci -q get greenwave-lcd-i2c.@greenwave[0]."

int lcd_i2c_dev;
int lcd_addr = 0x27; 

int lcd_cols = 16;
int lcd_rows = 2;
uint8_t pin_backlight = 0x08; 
char tmp_file_path[128] = "/tmp/play_name";
char vol_file_path[128] = "/tmp/tda_volume";
char lcd_language[16] = "en_en";
int lcd_scroll_gap = 3; // Нова глобальна змінна для відступу бігучого рядка
int lcd_vol_style = 1; // Нова опція стилю шкали (від 1 до 5)

uint8_t cyr_custom_chars[8][8] = {
    {0x1F, 0x10, 0x10, 0x1E, 0x11, 0x11, 0x1E, 0x00}, // Б
    {0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00}, // Г
    {0x06, 0x0A, 0x0A, 0x0A, 0x0A, 0x1F, 0x11, 0x00}, // Д
    {0x15, 0x15, 0x15, 0x0E, 0x15, 0x15, 0x15, 0x00}, // Ж
    {0x11, 0x11, 0x13, 0x15, 0x19, 0x11, 0x11, 0x00}, // И / Й
    {0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00}, // П
    {0x04, 0x0E, 0x15, 0x15, 0x0E, 0x04, 0x04, 0x04}, // Ф
    {0x12, 0x15, 0x15, 0x1D, 0x15, 0x15, 0x12, 0x00}  // 0x07: Ю (Нова графічна маска літери Ю)
};

void get_uci_display_config(int *dev, int *lcd_addr, int *cols, int *rows, uint8_t *backlight, char *file_path, char *vol_path, char *lang) {
    FILE *fp;
    char path[128];
    
    fp = popen(UCI_CFG "i2c_dev", "r");
    if (fp && fgets(path, sizeof(path), fp)) { *dev = atoi(path); pclose(fp); } else if (fp) pclose(fp);

    fp = popen(UCI_CFG "lcd_adres", "r");
    if (fp && fgets(path, sizeof(path), fp)) { *lcd_addr = (int)strtol(path, NULL, 16); pclose(fp); } else if (fp) pclose(fp);

    fp = popen(UCI_CFG "lcd_cols", "r");
    if (fp && fgets(path, sizeof(path), fp)) { int val = atoi(path); if (val > 0) *cols = val; pclose(fp); } else if (fp) pclose(fp);

    fp = popen(UCI_CFG "lcd_rows", "r");
    if (fp && fgets(path, sizeof(path), fp)) { int val = atoi(path); if (val > 0) *rows = val; pclose(fp); } else if (fp) pclose(fp);

    fp = popen(UCI_CFG "lcd_backlight", "r");
    if (fp && fgets(path, sizeof(path), fp)) { *backlight = (atoi(path) == 0) ? 0x00 : 0x08; pclose(fp); } else if (fp) pclose(fp);

    fp = popen(UCI_CFG "tmp_file", "r");
    if (fp && fgets(path, sizeof(path), fp)) { path[strcspn(path, "\r\n")] = 0; if (strlen(path) > 0) strcpy(file_path, path); pclose(fp); } else if (fp) pclose(fp);

    fp = popen(UCI_CFG "vol_file", "r");
    if (fp && fgets(path, sizeof(path), fp)) { path[strcspn(path, "\r\n")] = 0; if (strlen(path) > 0) strcpy(vol_path, path); pclose(fp); } else if (fp) pclose(fp);

    fp = popen(UCI_CFG "language", "r");
    if (fp && fgets(path, sizeof(path), fp)) { 
        path[strcspn(path, "\r\n")] = 0; 
        strcpy(lang, (strlen(path) > 0) ? path : "en_en");
        pclose(fp); 
    } else { strcpy(lang, "en_en"); if (fp) pclose(fp); }

    fp = popen(UCI_CFG "scroll_gap", "r");
    if (fp && fgets(path, sizeof(path), fp)) { int val = atoi(path); if (val >= 0) lcd_scroll_gap = val; pclose(fp); } else if (fp) pclose(fp);

    // Зчитуємо стиль гучності
    fp = popen(UCI_CFG "vol_style", "r");
    if (fp && fgets(path, sizeof(path), fp)) { int val = atoi(path); if (val >= 1 && val <= 5) lcd_vol_style = val; pclose(fp); } else if (fp) pclose(fp);
}

void lcd_strobe(uint8_t data) {
    ioctl(lcd_i2c_dev, I2C_SLAVE, lcd_addr);
    uint8_t buf = data | PIN_ENABLE | pin_backlight;
    write(lcd_i2c_dev, &buf, 1); usleep(500);
    buf = (data & ~PIN_ENABLE) | pin_backlight;
    write(lcd_i2c_dev, &buf, 1); usleep(500);
}

void lcd_send(uint8_t mode, uint8_t val) {
    lcd_strobe((val & 0xF0) | mode);
    lcd_strobe(((val << 4) & 0xF0) | mode);
}

void lcd_load_custom_chars() {
    for (int i = 0; i < 8; i++) {
        lcd_send(LCD_CMD, 0x40 + (i * 8));
        for (int j = 0; j < 8; j++) lcd_send(LCD_CHR, cyr_custom_chars[i][j]);
    }
}

void lcd_init() {
    ioctl(lcd_i2c_dev, I2C_SLAVE, lcd_addr);
    lcd_strobe(0x30); usleep(5000);
    lcd_strobe(0x30); usleep(200);
    lcd_strobe(0x30);
    lcd_strobe(0x20);
    lcd_send(LCD_CMD, 0x28);
    lcd_send(LCD_CMD, 0x0C);
    lcd_send(LCD_CMD, 0x01); usleep(2000);

    if (strcmp(lcd_language, "en_en") == 0) lcd_load_custom_chars();
}

void lcd_set_cursor(int row, int col) {
    int addr = 0x80;
    if (row == 1) addr = 0xC0;
    else if (row == 2) addr = 0x80 + lcd_cols;
    else if (row == 3) addr = 0xC0 + lcd_cols;
    lcd_send(LCD_CMD, addr + col);
}

unsigned char* get_utf8_offset(unsigned char *str, int offset_chars) {
    while (*str && offset_chars > 0) {
        if ((*str & 0xC0) != 0x80) offset_chars--;
        str++;
    }
    while ((*str & 0xC0) == 0x80) str++;
    return str;
}

int strlen_utf8(const char *str) {
    int len = 0;
    unsigned char *p = (unsigned char *)str;
    while (*p) {
        if ((*p & 0xC0) != 0x80) len++;
        p++;
    }
    return len;
}

