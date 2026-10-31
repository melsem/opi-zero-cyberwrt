#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>

// Экспорт глобальных атомарных флагов для основного цикла
extern volatile sig_atomic_t reload_config_requested;
extern volatile sig_atomic_t terminate_requested;

// Инициализация перехвата сигналов
void setup_signals(void);

#endif // SIGNALS_H
