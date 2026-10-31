#include "ir_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char ch_buffer[32] = "";

void handle_ir_digit(const char *marker) {
    char digit = marker[strlen(num_prefix)];
    if (digit >= '0' && digit <= '9') {
        int len = strlen(ch_buffer);
        if (len < (int)sizeof(ch_buffer) - 1) {
            ch_buffer[len] = digit;
            ch_buffer[len + 1] = '\0';
            printf("[IR] Додано цифру. Буфер каналу: %s\n", ch_buffer);
        }
    }
}

// ОПТИМІЗОВАНО: Динамічна склейка шаблону команди з номером набраного каналу з ОЗУ
void trigger_channel(void) {
    if (strlen(ch_buffer) == 0) return;

    int channel_found = 0;

    // Шукаємо єдиний шаблонний рядок "CH" в ОЗУ
    for (int i = 0; i < rules_count; i++) {
        if (strcmp(rules[i].marker, ch_prefix) == 0 && rules[i].event == 1) {
            
            // Склеюємо шаблон (наприклад, "mpc play ") та набраний номер каналу (наприклад, "25")
            char dynamic_command[MAX_CMD_LEN + 32];
            snprintf(dynamic_command, sizeof(dynamic_command), "%s%s", rules[i].command, ch_buffer);
            
            printf("[IR CH DYNAMIC] Буфер: %s | Виконую: %s\n", ch_buffer, dynamic_command);
            
            if (strlen(dynamic_command) > 0) {
                char bg_command[MAX_CMD_LEN + 40];
                snprintf(bg_command, sizeof(bg_command), "%s &", dynamic_command);
                system(bg_command);
            }
            channel_found = 1;
            break;
        }
    }

    if (!channel_found) {
        printf("[IR CH ERROR] Шаблон для префіксу %s не знайдено в конфігу!\n", ch_prefix);
    }

    ch_buffer[0] = '\0'; // Очищаємо буфер для наступного набору каналів
}
