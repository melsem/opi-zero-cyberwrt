#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define LCD_CHR        1

extern char lcd_language[16];
extern void lcd_send(uint8_t mode, uint8_t val);

int lcd_print_cyr(const unsigned char *utf8_str, int max_chars) {
    int printed_count = 0;
    const unsigned char *p = utf8_str;

    while (*p && printed_count < max_chars) {
        if ((*p == 0xD0 || *p == 0xD1) && *(p + 1) != 0) {
            uint16_t unicode = ((*p & 0x1F) << 6) | (*(p + 1) & 0x3F);
            p += 2;

            if (strcmp(lcd_language, "en_ua") == 0) {
                if (unicode >= 0x0410 && unicode <= 0x042F) lcd_send(LCD_CHR, unicode - 0x0410 + 0x41);
                else if (unicode >= 0x0430 && unicode <= 0x043F) lcd_send(LCD_CHR, unicode - 0x0430 + 0x61);
                else if (unicode >= 0x0440 && unicode <= 0x044F) lcd_send(LCD_CHR, unicode - 0x0440 + 0x72);
                else if (unicode == 0x0406 || unicode == 0x0456) lcd_send(LCD_CHR, 'I');
                else if (unicode == 0x0407 || unicode == 0x0457) lcd_send(LCD_CHR, 0xEA);
                else lcd_send(LCD_CHR, '?');
            } 
            else { // ОСЬ ЦЕЙ БЛОК БУВ ЗЛАМАНИЙ ЧЕРЕЗ ПРОПУЩЕНУ ДУЖКУ ВИЩЕ
                switch (unicode) {
                    case 0x0410: case 0x0430: lcd_send(LCD_CHR, 'A'); break;
                    case 0x0412: case 0x0432: lcd_send(LCD_CHR, 'B'); break;
                    case 0x0415: case 0x0435: lcd_send(LCD_CHR, 'E'); break;
                    case 0x041A: case 0x043A: lcd_send(LCD_CHR, 'K'); break;
                    case 0x041C: case 0x043C: lcd_send(LCD_CHR, 'M'); break;
                    case 0x041D: case 0x043D: lcd_send(LCD_CHR, 'H'); break;
                    case 0x041E: case 0x043E: lcd_send(LCD_CHR, 'O'); break;
                    case 0x0420: case 0x0440: lcd_send(LCD_CHR, 'P'); break;
                    case 0x0421: case 0x0441: lcd_send(LCD_CHR, 'C'); break;
                    case 0x0422: case 0x0442: lcd_send(LCD_CHR, 'T'); break;
                    case 0x0425: case 0x0445: lcd_send(LCD_CHR, 'X'); break;
                    case 0x0406: case 0x0456: lcd_send(LCD_CHR, 'I'); break;
                    case 0x0423: case 0x0443: lcd_send(LCD_CHR, 'Y'); break;

                    case 0x0411: case 0x0431: lcd_send(LCD_CHR, 0x00); break; // Б
                    case 0x0413: case 0x0433: lcd_send(LCD_CHR, 0x01); break; // Г
                    case 0x0414: case 0x0434: lcd_send(LCD_CHR, 0x02); break; // Д
                    case 0x0416: case 0x0436: lcd_send(LCD_CHR, 0x03); break; // Ж
                    case 0x0418: case 0x0438: case 0x0419: case 0x0439: 
                                              lcd_send(LCD_CHR, 0x04); break; // И / Й
                    case 0x041F: case 0x043F: lcd_send(LCD_CHR, 0x05); break; // П
                    case 0x0424: case 0x0444: lcd_send(LCD_CHR, 0x06); break; // Ф
                    case 0x042E: case 0x044E: lcd_send(LCD_CHR, 0x07); break; // Ю (CGRAM 7)

                    case 0x0426: case 0x0446: lcd_send(LCD_CHR, 249);  break; // Ц (Код 249 з ROM)

                    case 0x0417: case 0x0437: lcd_send(LCD_CHR, '3'); break;
                    case 0x0427: case 0x0447: lcd_send(LCD_CHR, '4'); break;
                    case 0x041B: case 0x043B: lcd_send(LCD_CHR, 0x05); break; // Л -> П
                    case 0x042C: case 0x044C: lcd_send(LCD_CHR, 'b'); break; // Ь -> b
                    case 0x0407: case 0x0457: lcd_send(LCD_CHR, 'I'); break; // Ї -> І

                    default:                  lcd_send(LCD_CHR, '?'); break;
                }
            } // Закриваємо блок else
            printed_count++;
        } else {
            lcd_send(LCD_CHR, *p++);
            printed_count++;
        }
    }
    return printed_count;
}

