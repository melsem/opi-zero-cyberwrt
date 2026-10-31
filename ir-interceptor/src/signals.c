#include "signals.h"
#include <stddef.h>

// Определение глобальных переменных
volatile sig_atomic_t reload_config_requested = 0;
volatile sig_atomic_t terminate_requested = 0;

// Внутренний обработчик сигналов ОС
static void handle_signal(int sig) {
    if (sig == SIGHUP) {
        reload_config_requested = 1;
    } else if (sig == SIGTERM || sig == SIGINT) {
        terminate_requested = 1;
    }
}

// Инициализация перехвата сигналов
void setup_signals(void) {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; // POLLIN/poll() прервется мгновенно для горячего обновления

    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
}
