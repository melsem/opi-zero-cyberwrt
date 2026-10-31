#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <poll.h>
#include <dirent.h>
#include <errno.h>
#include <sys/file.h> // Додано для роботи з flock()
#include <linux/input.h>

#include "signals.h"
#include "ir_handler.h"
#include "gpio_handler.h"
#include "keyboard_handler.h"

#define CONF_FILE "/etc/config/ir-interceptor"
#define PID_FILE  "/var/run/ir-interceptor.pid" // Шлях до PID-файлу в OpenWrt
#define MAX_CMD_LEN 256
#define MAX_KEY_CODE 1024
#define MAX_DEVICES 16
#define MAX_RULES 256  // Максимальна кількість строк з командами в конфігу

// Глобальний масив правил в ОЗУ та його поточний розмір (експортується в ir_handler.c)
ConfigRule rules[MAX_RULES];
int rules_count = 0;
int pid_fd = -1; // Дескриптор файла блокування

// Глобальні налаштування
int debounce_press_ms = 200;
int debounce_hold_ms = 800;
int ch_timeout_ms = 500;
char num_prefix[32] = "NUM";
char ch_prefix[32] = "CH";

// Масив дескрипторів пристроїв
int fds[MAX_DEVICES];
int fd_count = 0;

// Буфери для збереження станів дребезгу
long long last_execution_ms[MAX_KEY_CODE] = {0};
long long last_rel_execution_ms = 0; 

// Отримання поточного часу в мс
long long get_current_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)(ts.tv_sec) * 1000 + (ts.tv_nsec / 1000000);
}

// Функція перевірки унікальності процесу (захист від подвійного запуску)
int check_and_create_pid() {
    pid_fd = open(PID_FILE, O_RDWR | O_CREAT, 0640);
    if (pid_fd < 0) {
        fprintf(stderr, "[MAIN ERROR] Не вдалося відкрити PID-файл %s\n", PID_FILE);
        return -1;
    }

    // Намагаємось заблокувати файл. LOCK_NB означає не блокувати потік, якщо зайнято
    if (flock(pid_fd, LOCK_EX | LOCK_NB) < 0) {
        if (errno == EWOULDBLOCK) {
            fprintf(stderr, "[MAIN WARNING] Перехоплювач ВЖЕ запущено в системі! Вихід.\n");
        } else {
            fprintf(stderr, "[MAIN ERROR] Помилка блокування PID-файлу.\n");
        }
        close(pid_fd);
        return -1;
    }

    // Якщо заблокувати вдалося — записуємо поточний PID процесу у файл
    char pid_str[16];
    snprintf(pid_str, sizeof(pid_str), "%d\n", getpid());
    if (ftruncate(pid_fd, 0) == 0) { // Очищаємо старий вміст, якщо залишився
        unsigned int written = write(pid_fd, pid_str, strlen(pid_str));
        (void)written; // Скасовуємо попередження про невикористану змінну
    }
    return 0;
}

// Функція очищення PID-файлу (викликається при виході)
void cleanup_pid() {
    if (pid_fd >= 0) {
        flock(pid_fd, LOCK_UN); // Знімаємо блокування
        close(pid_fd);
        unlink(PID_FILE); // Видаляємо файл із системи
    }
}

/* Завантаження всього конфігу в ОЗУ (викликається при старті та по SIGHUP)
   з підтримкою групових кодів `коди,через,кому` */
void load_settings() {
    debounce_press_ms = 200;
    debounce_hold_ms = 800;
    ch_timeout_ms = 500;
    strcpy(num_prefix, "NUM_");
    strcpy(ch_prefix, "CH");
    rules_count = 0; 

    FILE *fp = fopen(CONF_FILE, "r");
    if (!fp) return;

    char line[MAX_CMD_LEN + 128], key[60], val[60];
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        // 1. Парсимо глобальні налаштування SET
        if (sscanf(line, "SET %59s %59s", key, val) == 2) {
            if (strcmp(key, "DEBOUNCE_PRESS") == 0) debounce_press_ms = atoi(val);
            else if (strcmp(key, "DEBOUNCE_HOLD") == 0) debounce_hold_ms = atoi(val);
            else if (strcmp(key, "CH_TIMEOUT") == 0) ch_timeout_ms = atoi(val);
            else if (strcmp(key, "NUM_PREFIX") == 0) strncpy(num_prefix, val, sizeof(num_prefix) - 1);
            else if (strcmp(key, "CH_PREFIX") == 0) strncpy(ch_prefix, val, sizeof(ch_prefix) - 1);
            continue;
        }

        // 2. Парсимо правила зіставлення кнопок
        char marker[60], code_str[128], command[MAX_CMD_LEN];
        int event_val;
        
        // Читаємо рядок як текстові блоки
        int parsed = sscanf(line, "%59s %127s %d %255[^\n]", marker, code_str, &event_val, command);
        if (parsed >= 2) {
            // Перевіряємо, чи це віртуальний динамічний канал (Секція 2)
            if (strcmp(marker, ch_prefix) == 0) {
                if (rules_count < MAX_RULES) {
                    ConfigRule *r = &rules[rules_count];
                    strcpy(r->marker, marker);
                    r->code = 0; // Для шаблону каналу точний код не потрібен
                    r->event = event_val;
                    if (parsed == 4) strcpy(r->command, command);
                    else r->command[0] = '\0';
                    rules_count++;
                }
                continue;
            }

            // Очищення зворотних кавычок `, якщо вони є
            char *clean_code = code_str;
            if (clean_code[0] == '`') clean_code++;
            size_t len = strlen(clean_code);
            if (len > 0 && clean_code[len - 1] == '`') clean_code[len - 1] = '\0';

            // Ріжемо рядок кодингу по запятій (наприклад: "11" або "11,0xbf03,0x80ff00")
            char *token = strtok(clean_code, ",");
            while (token != NULL) {
                if (rules_count < MAX_RULES) {
                    ConfigRule *r = &rules[rules_count];
                    strcpy(r->marker, marker);
                    
                    // Обробка шістнадцяткових (0x) або десяткових кодів кнопок
                    if (strncmp(token, "0x", 2) == 0 || strncmp(token, "0X", 2) == 0) {
                        r->code = (int)strtol(token, NULL, 16);
                    } else {
                        r->code = atoi(token);
                    }
                    
                    r->event = event_val;
                    if (parsed == 4) strcpy(r->command, command);
                    else r->command[0] = '\0';
                    
                    rules_count++;
                }
                token = strtok(NULL, ",");
            }
        }
    }
    fclose(fp);
    printf("[MAIN] Завантажено %d розгорнутих правил в ОЗУ.\n", rules_count);
}

// Закриття всіх відкритих пристроїв
void close_all_devices() {
    for (int i = 0; i < fd_count; i++) {
        if (fds[i] >= 0) close(fds[i]);
    }
    fd_count = 0;
}

// Додавання конкретного пристрою за коротким ім'ям
void add_device_by_name(const char *name) {
    if (fd_count >= MAX_DEVICES) return;
    char path[64];
    snprintf(path, sizeof(path), "/dev/input/%s", name);
    int fd = open(path, O_RDONLY | O_NONBLOCK);
    if (fd >= 0) {
        fds[fd_count++] = fd;
    }
}

// Автоматичне сканування директорії /dev/input/
void auto_scan_devices() {
    DIR *dir = opendir("/dev/input");
    if (!dir) return;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, "event", 5) == 0) {
            add_device_by_name(entry->d_name);
        }
    }
    closedir(dir);
}

// Ініціалізація підключень пристроїв (з конфігу або автоскан)
void init_devices() {
    close_all_devices();
    FILE *fp = fopen(CONF_FILE, "r");
    int has_manual_devices = 0;

    if (fp) {
        char line[MAX_CMD_LEN + 128], key[60], val[60];
        while (fgets(line, sizeof(line), fp)) {
            if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;
            if (sscanf(line, "SET %59s %59s", key, val) == 2) {
                if (strcmp(key, "DEVICE") == 0) {
                    add_device_by_name(val);
                    has_manual_devices = 1;
                }
            }
        }
        fclose(fp);
    }

    if (!has_manual_devices || fd_count == 0) {
        auto_scan_devices();
    }
}

// Швидка обробка подій по базі даних в ОЗУ БЕЗ звернення до диску
void handle_event(int code, int value, int is_rel) {
    long long now = get_current_ms();

    // Захист від брязкіту контактів (Debounce)
    if (is_rel) {
        if (now - last_rel_execution_ms < debounce_press_ms) return;
    } else {
        if (code < 0 || code >= MAX_KEY_CODE) return;
        int limit = (value == 1) ? debounce_press_ms : debounce_hold_ms;
        if (now - last_execution_ms[code] < limit) return;
    }

    int key_found = 0;

    // Лінійний пошук правила в ОЗУ за наносекунди
    for (int i = 0; i < rules_count; i++) {
        if (rules[i].code == code && rules[i].event == (is_rel ? 1 : value)) {
            key_found = 1;
            if (is_rel) last_rel_execution_ms = now;
            else last_execution_ms[code] = now;

            // МАРШРУТИЗАЦІЯ ПО ОКРЕМІХ МОДУЛЯХ
            if (!is_rel && strncmp(rules[i].marker, num_prefix, strlen(num_prefix)) == 0) {
                handle_ir_digit(rules[i].marker); // Модуль ІК (збір цифр каналу)
            } else {
                if (strlen(ch_buffer) > 0) ch_buffer[0] = '\0'; // Скидаємо буфер чисел, якщо натиснуто іншу кнопку
                if (strlen(rules[i].command) > 0) {
                    if (strncmp(rules[i].marker, "GPIO", 4) == 0 || strncmp(rules[i].marker, "ENC", 3) == 0 || strncmp(rules[i].marker, "BTN", 3) == 0) {
                        handle_gpio_event(rules[i].marker, rules[i].command); // Модуль GPIO/Енкодера
                    } else if (strncmp(rules[i].marker, "KEY", 3) == 0 || strncmp(rules[i].marker, "KBD", 3) == 0 || strncmp(rules[i].marker, "VOL", 3) == 0 || strncmp(rules[i].marker, "SYS", 3) == 0) {
                        handle_keyboard_event(rules[i].marker, rules[i].command); // Модуль Клавіатури
                    } else {
                        // Дефолтний запуск фонової команди
                        char bg_command[MAX_CMD_LEN + 8];
                        snprintf(bg_command, sizeof(bg_command), "%s &", rules[i].command);
                        system(bg_command);
                    }
                }
            }
            break; // Знайшли перше збігання — зупиняємо пошук по ОЗУ
        }
    }

    if (!key_found && !is_rel && value == 1) {
        printf("[MAIN] Незареєстрований код кнопки: %d (0x%03x)\n", code, code);
    } else if (!key_found && is_rel) {
        handle_encoder_movement(code); // Сповіщаємо GPIO модуль про невідомий крок
    }
}

int main() {
    // ЗАХИСТ ВІД ПОДВІЙНОГО ЗАПУСКУ: Перевіряємо, чи не запущено вже один такий процес
    if (check_and_create_pid() < 0) {
        return 1; // Завершуємо роботу, якщо копія вже працює в системі
    }

    setup_signals(); // Активуємо перехоплення сигналів SIGHUP/SIGTERM
    load_settings(); // Завантажуємо конфігурацію в ОЗУ один раз на старті
    init_devices();

    if (fd_count == 0) {
        fprintf(stderr, "Помилка: Жодного пристрою введення не знайдено в /dev/input/\n");
        cleanup_pid();
        return 1;
    }

    printf("[MAIN] Перехоплювач успішно запущено. Слухаю %d пристроїв.\n", fd_count);

    struct pollfd pfd[MAX_DEVICES];
    struct input_event ev;

    while (!terminate_requested) { 
        // Обробка гарячого перезавантаження конфігурації по сигналу SIGHUP
        if (reload_config_requested) {
            reload_config_requested = 0;
            printf("[MAIN] Отримано сигнал SIGHUP. Гаряче оновлення конфігурації в ОЗУ...\n");
            load_settings();
            init_devices();
            if (fd_count == 0) {
                fprintf(stderr, "Помилка після релоаду: пристрої зникли.\n");
                cleanup_pid();
                return 1;
            }
        }

        // Заповнюємо структуру poll перед кожним опитуванням
        for (int i = 0; i < fd_count; i++) {
            pfd[i].fd = fds[i];
            pfd[i].events = POLLIN;
            pfd[i].revents = 0;
        }

        // Динамічний таймаут: якщо набирається канал — чекаємо ch_timeout_ms, інакше — спимо вічно (-1)
        int timeout = (strlen(ch_buffer) > 0) ? ch_timeout_ms : -1;
        int ret = poll(pfd, fd_count, timeout);

        if (ret < 0) {
            if (errno == EINTR) continue; // poll було перервано сигналом — йдемо в початок while перевіряти прапори
            break; // Критична помилка poll
        }

        if (ret == 0) {
            trigger_channel(); // Закінчився таймаут набору каналу (виклик модуля ІК)
        } else {
            for (int i = 0; i < fd_count; i++) {
                // Захист від раптового відключення девайсу на льоту
                if (pfd[i].revents & (POLLERR | POLLHUP)) {
                    printf("[MAIN] Пристрій %d відключено або стався збій.\n", pfd[i].fd);
                    close(fds[i]);
                    for (int j = i; j < fd_count - 1; j++) {
                        fds[j] = fds[j + 1];
                    }
                    fd_count--;
                    i--;
                    continue;
                }

                if (pfd[i].revents & POLLIN) {
                    // Вичищаємо весь неблокуючий буфер подій пристрою повністю
                    while (1) {
                        ssize_t bytes = read(pfd[i].fd, &ev, sizeof(struct input_event));
                        if (bytes == sizeof(struct input_event)) {
                            if (ev.type == EV_KEY && (ev.value == 1 || ev.value == 2)) {
                                handle_event(ev.code, ev.value, 0); // Кнопка (клік або утримання)
                            } else if (ev.type == EV_REL) {
                                handle_event(ev.value, 1, 1); // Ротарі-енкодер (відносна вісь)
                            }
                        } else if (bytes < 0) {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) break; // Дані закінчились
                            break; // Помилка читання
                        } else {
                            break; // bytes == 0 (Кінець файлу / пристрій закрито)
                        }
                    }
                }
            }
        }
    }

    printf("[MAIN] Завершення роботи програми за сигналом. Очищення ресурсів...\n");
    close_all_devices();
    
    cleanup_pid(); // Видаляємо файл блокування /var/run/ir_interceptor.pid з системи
    return 0;
}
