#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define LCD_CHR        1

// Усунено warning компілятора: типи масивів тепер чітко збігаються
extern char lcd_language[16];
extern void lcd_send(uint8_t mode, uint8_t val);

int lcd_print_cyr(const unsigned char *utf8_str, int max_chars, int is_track) {
    int printed_count = 0;
    const unsigned char *p = utf8_str;

    // Латиниця стає ВЕЛИКОЮ (Caps Lock) ТІЛЬКИ якщо це реально ТРЕК (is_track = 1)!
    // Для системної інформації та помилок (і на 1, і на 2 рядку) латиниця завжди залишається як є.
    int force_upper_lat = is_track; 

    while (*p && printed_count < max_chars) {
        if ((*p == 0xD0 || *p == 0xD1) && *(p + 1) != 0) {
            uint16_t unicode = ((*p & 0x1F) << 6) | (*(p + 1) & 0x3F);
            p += 2;

            // ==================== РЕЖИМИ EN_UA ТА EN_RU (АПАРАТНА КИРИЛИЦЯ В ROM) ====================
            if (strcmp(lcd_language, "en_ua") == 0 || strcmp(lcd_language, "en_ru") == 0) {
                // Апаратну КИРИЛИЦЮ ми примусово переводимо у ВЕЛИКУ завжди,
                // оскільки малі кириличні літери в залізному ROM A02 часто криві або відсутні.
                if (unicode >= 0x0430 && unicode <= 0x044F) unicode -= 0x20; // а-я -> А-Я
                if (unicode == 0x0456) unicode = 0x0406; // і -> І
                if (unicode == 0x0457) unicode = 0x0407; // ї -> Ї
                if (unicode == 0x0491) unicode = 0x0490; // ґ -> Ґ

                switch (unicode) {
                    case 0x0410: lcd_send(LCD_CHR, 0x41); break; // А (латинська A)
                    case 0x0411: lcd_send(LCD_CHR, 0xA0); break; // Б
                    case 0x0412: lcd_send(LCD_CHR, 0x42); break; // В (латинська B)
                    case 0x0413: case 0x0490: lcd_send(LCD_CHR, 0xA1); break; // Г / Ґ
                    case 0x0414: lcd_send(LCD_CHR, 0xE0); break; // Д
                    case 0x0415: lcd_send(LCD_CHR, 0x45); break; // Е (латинська E)
                    case 0x0416: lcd_send(LCD_CHR, 0xA3); break; // Ж
                    case 0x0417: lcd_send(LCD_CHR, 0xA4); break; // З
                    case 0x0418: case 0x0419: lcd_send(LCD_CHR, 0xA5); break; // И / Й
                    case 0x041A: lcd_send(LCD_CHR, 0x4B); break; // К (латинська K)
                    case 0x041B: lcd_send(LCD_CHR, 0xA6); break; // Л
                    case 0x041C: lcd_send(LCD_CHR, 0x4D); break; // М (латинська M)
                    case 0x041D: lcd_send(LCD_CHR, 0x48); break; // Н (латинська H)
                    case 0x041E: lcd_send(LCD_CHR, 0x4F); break; // О (латинська O)
                    case 0x041F: lcd_send(LCD_CHR, 0xA7); break; // П
                    case 0x0420: lcd_send(LCD_CHR, 0x50); break; // Р (латинська P)
                    case 0x0421: lcd_send(LCD_CHR, 0x43); break; // С (латинська C)
                    case 0x0422: lcd_send(LCD_CHR, 0x54); break; // Т (латинська T)
                    case 0x0423: lcd_send(LCD_CHR, 0xA8); break; // У
                    case 0x0424: lcd_send(LCD_CHR, 0xA9); break; // Ф
                    case 0x0425: lcd_send(LCD_CHR, 0x58); break; // Х (латинська X)
                    case 0x0426: lcd_send(LCD_CHR, 0xAA); break; // Ц
                    case 0x0427: lcd_send(LCD_CHR, 0xAB); break; // Ч
                    case 0x0428: lcd_send(LCD_CHR, 0xAC); break; // Ш
                    case 0x0429: lcd_send(LCD_CHR, 0xAD); break; // Щ
                    case 0x042A: lcd_send(LCD_CHR, 0xAE); break; // Ъ
                    case 0x042B: lcd_send(LCD_CHR, 0xAF); break; // Ы
                    case 0x042C: lcd_send(LCD_CHR, 0xB0); break; // Ь
                    case 0x042D: lcd_send(LCD_CHR, 0xB1); break; // Э
                    case 0x042E: lcd_send(LCD_CHR, 0xB2); break; // Ю
                    case 0x042F: lcd_send(LCD_CHR, 0xB3); break; // Я
                    case 0x0406: lcd_send(LCD_CHR, 0x49); break; // І
                    case 0x0407: lcd_send(LCD_CHR, 0xEA); break; // Ї
                    default:     lcd_send(LCD_CHR, '?');  break;
                }
            }
            // ==================== РЕЖИМ EN_EN (ЧИСТА ЛАТИНЬ + CGRAM МАСКИ СИМВОЛІВ) ====================
            else { 
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
                    case 0x0423: case 0x0443: lcd_send(LCD_CHR, 'Y'); break;
                    
                    // ОБ'ЄДНАНО: І, і, Ї, ї -> виводяться як англійська 'I'
                    case 0x0406: case 0x0456: case 0x0407: case 0x0457: 
                        lcd_send(LCD_CHR, 'I'); break; 

                    case 0x0411: case 0x0431: lcd_send(LCD_CHR, 0x00); break; // Б
                    
                    // ОБ'ЄДНАНО: Г, г, Ґ, ґ -> виводяться кастомним символом 0x01
                    case 0x0413: case 0x0433: case 0x0490: case 0x0491: 
                        lcd_send(LCD_CHR, 0x01); break; 

                    case 0x0414: case 0x0434: lcd_send(LCD_CHR, 0x02); break; // Д
                    case 0x0416: case 0x0436: lcd_send(LCD_CHR, 0x03); break; // Ж
                    case 0x0418: case 0x0438: case 0x0419: case 0x0439: 
                                              lcd_send(LCD_CHR, 0x04); break; // И / Й
                    
                    // ЛОГІКА ДЛЯ ЛІТЕРИ Л: використовує звільнену кастомну маску 0x05
                    case 0x041B: case 0x043B: lcd_send(LCD_CHR, 0x05); break; // Л
                    
                    // ЛОГІКА ДЛЯ ЛІТЕРИ П: заміщається вбудованим символом "пі" (код 0xF7)
                    case 0x041F: case 0x043F: lcd_send(LCD_CHR, 0xF7); break; // П
                    
                    case 0x0424: case 0x0444: lcd_send(LCD_CHR, 0x06); break; // Ф
                    case 0x042E: case 0x044E: lcd_send(LCD_CHR, 0x07); break; // Ю

                    case 0x0426: case 0x0446: lcd_send(LCD_CHR, 249);  break; // Ц
                    case 0x0417: case 0x0437: lcd_send(LCD_CHR, '3'); break; // З -> 3
                    case 0x0427: case 0x0447: lcd_send(LCD_CHR, '4'); break; // Ч -> 4
                    case 0x042C: case 0x044C: lcd_send(LCD_CHR, 'b'); break; // Ь -> b

                    default:                  lcd_send(LCD_CHR, '?'); break;
                }
            } 
            printed_count++;
        } else {
            // ОБРОБКА СТАНДАРТНОЇ ЛАТИНИЦІ / ASCII СИМВОЛІВ
            uint8_t ch = *p++;
            if (force_upper_lat && ch >= 'a' && ch <= 'z') {
                ch = ch - 'a' + 'A'; // Автоматичний Caps Lock для латинських літер
            }
            lcd_send(LCD_CHR, ch);
            printed_count++;
        }
    }
    return printed_count;
}
