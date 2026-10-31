#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <poll.h>
#include <dirent.h>
#include <errno.h>
#include <linux/input.h>

#define CONF_FILE "/etc/config/ir-interceptor"
#define MAX_CMD_LEN 256
#define MAX_KEY_CODE 1024
#define MAX_DEVICES 16

// Глобальные настройки
int debounce_press_ms = 200;
int debounce_hold_ms = 800;
int ch_timeout_ms = 500;
char num_prefix[32] = "NUM";
char ch_prefix[32] = "CH";

int fds[MAX_DEVICES];
int fd_count = 0;

long long last_execution_ms[MAX_KEY_CODE] = {0};
long long last_rel_execution_ms = 0; 
char ch_buffer[32] = "";

long long get_current_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)(ts.tv_sec) * 1000 + (ts.tv_nsec / 1000000);
}

void load_settings() {
    debounce_press_ms = 200;
    debounce_hold_ms = 800;
    ch_timeout_ms = 500;
    strcpy(num_prefix, "NUM");
    strcpy(ch_prefix, "CH");

    FILE *fp = fopen(CONF_FILE, "r");
    if (!fp) return;

    char line[MAX_CMD_LEN + 128], key[60], val[60];
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;
        if (sscanf(line, "SET %59s %59s", key, val) == 2) {
            if (strcmp(key, "DEBOUNCE_PRESS") == 0) debounce_press_ms = atoi(val);
            else if (strcmp(key, "DEBOUNCE_HOLD") == 0) debounce_hold_ms = atoi(val);
            else if (strcmp(key, "CH_TIMEOUT") == 0) ch_timeout_ms = atoi(val);
            else if (strcmp(key, "NUM_PREFIX") == 0) strncpy(num_prefix, val, sizeof(num_prefix) - 1);
            else if (strcmp(key, "CH_PREFIX") == 0) strncpy(ch_prefix, val, sizeof(ch_prefix) - 1);
        }
    }
    fclose(fp);
}

void close_all_devices() {
    for (int i = 0; i < fd_count; i++) {
        if (fds[i] >= 0) close(fds[i]);
    }
    fd_count = 0;
}

void add_device_by_name(const char *name) {
    if (fd_count >= MAX_DEVICES) return;
    char path[64];
    snprintf(path, sizeof(path), "/dev/input/%s", name);
    int fd = open(path, O_RDONLY | O_NONBLOCK);
    if (fd >= 0) {
        fds[fd_count++] = fd;
    }
}

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

void trigger_channel() {
    if (strlen(ch_buffer) == 0) return;
    load_settings();

    FILE *fp = fopen(CONF_FILE, "r");
    if (!fp) return;

    char line[MAX_CMD_LEN + 128], prefix[60], channel_num[60], command[MAX_CMD_LEN];
    int file_event;

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r' || strncmp(line, "SET", 3) == 0) continue;
        // Ограничиваем считывание строки %[^\n] до MAX_CMD_LEN - 1 во избежание переполнения буфера
        if (sscanf(line, "%59s %59s %d %255[^\n]", prefix, channel_num, &file_event, command) == 4) {
            if (strcmp(prefix, ch_prefix) == 0 && strcmp(channel_num, ch_buffer) == 0 && file_event == 1) {
                printf("[IR CH] Виконую канал %s: %s\n", ch_buffer, command);
                char bg_command[MAX_CMD_LEN + 8];
                snprintf(bg_command, sizeof(bg_command), "%s &", command);
                system(bg_command);
                break;
            }
        }
    }
    fclose(fp);
    ch_buffer[0] = '\0';
}

void handle_event(int code, int value, int is_rel) {
    load_settings();
    long long now = get_current_ms();

    if (is_rel) {
        if (now - last_rel_execution_ms < debounce_press_ms) return;
    } else {
        if (code < 0 || code >= MAX_KEY_CODE) return;
        int limit = (value == 1) ? debounce_press_ms : debounce_hold_ms;
        if (now - last_execution_ms[code] < limit) return;
    }

    FILE *fp = fopen(CONF_FILE, "r");
    if (!fp) return;

    char line[MAX_CMD_LEN + 128], marker[60], command[MAX_CMD_LEN];
    int file_code, file_event, key_found = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r' || strncmp(line, "SET", 3) == 0) continue;
        int parsed = sscanf(line, "%59s %i %d %255[^\n]", marker, &file_code, &file_event, command);
        
        if (parsed >= 3 && file_code == code && file_event == (is_rel ? 1 : value)) {
            key_found = 1;
            if (is_rel) last_rel_execution_ms = now;
            else last_execution_ms[code] = now;

            if (!is_rel && strncmp(marker, num_prefix, strlen(num_prefix)) == 0) {
                char digit = marker[strlen(num_prefix)];
                if (digit >= '0' && digit <= '9') {
                    int len = strlen(ch_buffer);
                    if (len < (int)sizeof(ch_buffer) - 1) {
                        ch_buffer[len] = digit;
                        ch_buffer[len + 1] = '\0';
                        printf("[IR] Додано цифру. Буфер: %s\n", ch_buffer);
                    }
                }
            } else {
                if (strlen(ch_buffer) > 0) ch_buffer[0] = '\0'; 
                if (parsed == 4 && strlen(command) > 0) {
                    printf("[IR EVENT] %s: %s\n", marker, command);
                    char bg_command[MAX_CMD_LEN + 8];
                    snprintf(bg_command, sizeof(bg_command), "%s &", command);
                    system(bg_command);
                }
            }
            break;
        }
    }

    if (!key_found && !is_rel && value == 1) {
        printf("[IR] Незареєстрований код кнопки: %d (0x%03x)\n", code, code);
    } else if (!key_found && is_rel) {
        printf("[IR] Незареєстрований рух енкодера, код/напрямок: %d\n", code);
    }
    fclose(fp);
}

int main() {
    load_settings();
    init_devices();

    if (fd_count == 0) {
        fprintf(stderr, "Помилка: Жодного пристрою введення не знайдено в /dev/input/\n");
        return 1;
    }

    printf("[IR] Перехоплювач успішно запущено. Слухаю %d пристроїв.\n", fd_count);

    struct pollfd pfd[MAX_DEVICES];
    struct input_event ev;

    while (1) {
        for (int i = 0; i < fd_count; i++) {
            pfd[i].fd = fds[i];
            pfd[i].events = POLLIN;
            pfd[i].revents = 0;
        }

        int timeout = (strlen(ch_buffer) > 0) ? ch_timeout_ms : -1;
        int ret = poll(pfd, fd_count, timeout);

        if (ret < 0) {
            if (errno == EINTR) continue; // Прерывание системным сигналом — не ошибка
            break;
        }

        if (ret == 0) {
            trigger_channel(); 
        } else {
            for (int i = 0; i < fd_count; i++) {
                // Если устройство было отключено (HUP) или вызвало ошибку
                if (pfd[i].revents & (POLLERR | POLLHUP)) {
                    printf("[IR] Пристрій %d відключено або стався збій.\n", pfd[i].fd);
                    close(fds[i]);
                    // Сдвигаем массив дескрипторов, удаляя мертвый
                    for (int j = i; j < fd_count - 1; j++) {
                        fds[j] = fds[j + 1];
                    }
                    fd_count--;
                    i--;
                    continue;
                }

                if (pfd[i].revents & POLLIN) {
                    while (1) {
                        ssize_t bytes = read(pfd[i].fd, &ev, sizeof(struct input_event));
                        if (bytes == sizeof(struct input_event)) {
                            if (ev.type == EV_KEY && (ev.value == 1 || ev.value == 2)) {
                                handle_event(ev.code, ev.value, 0); 
                            } else if (ev.type == EV_REL) {
                                // Предаем ev.code (это номер оси, например REL_X = 0x00) или ev.value
                                handle_event(ev.value, 1, 1); 
                            }
                        } else if (bytes < 0) {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                                break; // Данные в буфере evdev закончились, выходим из цикла чистки буфера
                            }
                            // Другая ошибка чтения — выходим
                            break; 
                        } else {
                            // bytes == 0 означает EOF (устройство закрылось)
                            break;
                        }
                    }
                }
            }
        }
    }

    close_all_devices();
    return 0;
}

