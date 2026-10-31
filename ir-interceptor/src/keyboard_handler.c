#include "keyboard_handler.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_CMD_LEN 256

void handle_keyboard_event(const char *marker, const char *command) {
    printf("[KBD EVENT] %s: %s\n", marker, command);
    char bg_command[MAX_CMD_LEN + 8];
    snprintf(bg_command, sizeof(bg_command), "%s &", command);
    system(bg_command);
}
