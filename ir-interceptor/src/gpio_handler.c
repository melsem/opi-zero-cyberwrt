#include "gpio_handler.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_CMD_LEN 256

void handle_gpio_event(const char *marker, const char *command) {
    printf("[GPIO EVENT] %s: %s\n", marker, command);
    char bg_command[MAX_CMD_LEN + 8];
    snprintf(bg_command, sizeof(bg_command), "%s &", command);
    system(bg_command);
}

void handle_encoder_movement(int value) {
    // Вызывается из main.c только если движение энкодера (-1 или 1) не зарегистрировано в конфиге
    printf("[GPIO ENC WARNING] Незареєстрований рух енкодера, напрямок: %d\n", value);
}
