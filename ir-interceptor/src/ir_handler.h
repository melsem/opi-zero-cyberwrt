#ifndef IR_HANDLER_H
#define IR_HANDLER_H

#define MAX_CMD_LEN 256

// Дублируем структуру правил, чтобы этот модуль понимал формат данных
typedef struct {
    char marker[60];
    int code;
    int event;
    char command[MAX_CMD_LEN];
} ConfigRule;

// Экспортируем буфер и настройки
extern char ch_buffer[32];
extern int ch_timeout_ms;
extern char num_prefix[32];
extern char ch_prefix[32];

// Экспортируем массив из main.c для быстрого поиска по памяти
extern ConfigRule rules[];
extern int rules_count;

void handle_ir_digit(const char *marker);
void trigger_channel(void);

#endif // IR_HANDLER_H
