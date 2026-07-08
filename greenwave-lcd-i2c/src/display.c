#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <uci.h> // Офіційне швидке C-API для роботи з конфігурацією OpenWrt

#define LCD_CHR        1
#define LCD_CMD        0
#define PIN_ENABLE     0x04

int lcd_i2c_dev;
int lcd_addr = 0x27; 

int lcd_cols = 16;
int lcd_rows = 2;
uint8_t pin_backlight = 0x08; 
char tmp_file_path[128] = "/tmp/play_name";
char vol_file_path[128] = "/tmp/tda_volume";
char lcd_language[16] = "en_en"; // Усунено warning (чітко вказано розмір масиву)
int lcd_scroll_gap = 3; 
int lcd_vol_style = 1; 

uint8_t cyr_custom_chars[8][8] = {
    {0x1F, 0x10, 0x10, 0x1E, 0x11, 0x11, 0x1E, 0x00}, // 0x00: Б
    {0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00}, // 0x01: Г / Ґ
    {0x06, 0x0A, 0x0A, 0x0A, 0x0A, 0x1F, 0x11, 0x00}, // 0x02: Д
    {0x15, 0x15, 0x15, 0x0E, 0x15, 0x15, 0x15, 0x00}, // 0x03: Ж
    {0x11, 0x11, 0x13, 0x15, 0x19, 0x11, 0x11, 0x00}, // 0x04: И / Й
    {0x07, 0x09, 0x09, 0x09, 0x09, 0x11, 0x11, 0x00}, // 0x05: РЕДАГОВАНО ДЛЯ ЛІТЕРИ "Л"
    {0x04, 0x0E, 0x15, 0x15, 0x0E, 0x04, 0x04, 0x04}, // 0x06: Ф
    {0x12, 0x15, 0x15, 0x1D, 0x15, 0x15, 0x12, 0x00}  // 0x07: Ю
};

// Допоміжна утиліта для швидкого та безпечного зчитування опцій UCI
static const char *get_uci_option(struct uci_context *ctx, struct uci_package *p, const char *section, const char *option) {
    struct uci_section *s = uci_lookup_section(ctx, p, section);
    if (!s) return NULL;
    
    struct uci_option *o = uci_lookup_option(ctx, s, option);
    if (!o || o->type != UCI_TYPE_STRING) return NULL;
    
    return o->v.string;
}

void get_uci_display_config(int *dev, int *lcd_addr_ptr, int *cols, int *rows, uint8_t *backlight, char *file_path, char *vol_path, char *lang) {
    struct uci_context *ctx = uci_alloc_context();
    if (!ctx) return;

    struct uci_package *p = NULL;
    // Завантажуємо файл конфігурації /etc/config/greenwave-lcd-i2c
    if (uci_load(ctx, "greenwave-lcd-i2c", &p) != UCI_OK) {
        uci_free_context(ctx);
        return;
    }

    // Шукаємо першу анонімну секцію за типом "greenwave"
    struct uci_element *e;
    const char *section_name = NULL;
    uci_foreach_element(&p->sections, e) {
        struct uci_section *s = uci_to_section(e);
        if (strcmp(s->type, "greenwave") == 0) {
            section_name = s->e.name;
            break;
        }
    }

    if (section_name) {
        const char *val;

        if ((val = get_uci_option(ctx, p, section_name, "i2c_dev"))) *dev = atoi(val);
        if ((val = get_uci_option(ctx, p, section_name, "lcd_adres"))) *lcd_addr_ptr = (int)strtol(val, NULL, 16);
        if ((val = get_uci_option(ctx, p, section_name, "lcd_cols"))) { int v = atoi(val); if (v > 0) *cols = v; }
        if ((val = get_uci_option(ctx, p, section_name, "lcd_rows"))) { int v = atoi(val); if (v > 0) *rows = v; }
        if ((val = get_uci_option(ctx, p, section_name, "lcd_backlight"))) *backlight = (atoi(val) == 0) ? 0x00 : 0x08;
        
        // Безпечне копіювання з жорстким контролем розміру виділеного буфера (захист від Buffer Overflow)
        if ((val = get_uci_option(ctx, p, section_name, "tmp_file"))) {
            strncpy(file_path, val, 127);
            file_path[127] = '\0';
        }
        if ((val = get_uci_option(ctx, p, section_name, "vol_file"))) {
            strncpy(vol_path, val, 127);
            vol_path[127] = '\0';
        }
        if ((val = get_uci_option(ctx, p, section_name, "language"))) {
            strncpy(lang, val, 15);
            lang[15] = '\0';
        }

        if ((val = get_uci_option(ctx, p, section_name, "scroll_gap"))) { int v = atoi(val); if (v >= 0) lcd_scroll_gap = v; }
        if ((val = get_uci_option(ctx, p, section_name, "vol_style"))) { int v = atoi(val); if (v >= 1 && v <= 5) lcd_vol_style = v; }
    }

    uci_free_context(ctx); // Повністю звільняємо контекст UCI
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

    // Кастомні символи вантажимо ТІЛЬКИ для en_en. 
    // Для апаратних кириличних прошивок (en_ua / en_ru) вони не потрібні.
    if (strcmp(lcd_language, "en_en") == 0) {
        lcd_load_custom_chars();
    }
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
    // Захист від виходу за межі пам'яті (Buffer Overflow) на обірваних UTF-8 символах
    while (*str && (*str & 0xC0) == 0x80) str++;
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

