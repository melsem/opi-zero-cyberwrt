#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

#define BUFFER_FILE     "/dev/shm/thd_digit_buffer"
#define LOCK_FILE       "/dev/shm/thd_digit_lock"
#define CMD_FILE        "/dev/shm/thd_digit_cmd"
#define LAST_TIME_FILE  "/dev/shm/thd_last_press_time"
#define HOLD_FLAG_FILE  "/dev/shm/thd_hold_flag"

#define DEFAULT_CE_PRESS      200
#define DEFAULT_DEBOUNCE_HOLD 800
#define DEFAULT_CH_TIMEOUT    1000

long long get_current_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)(tv.tv_sec) * 1000 + (tv.tv_usec / 1000);
}

int main(int argc, char **argv) {
    char digit = '\0';
    char *k_command = NULL;
    char *l_command = NULL;
    int state = 0;
    
    int ce_press = DEFAULT_CE_PRESS;
    int debounce_hold = DEFAULT_DEBOUNCE_HOLD;
    int ch_timeout = DEFAULT_CH_TIMEOUT;
    
    int opt;

    while ((opt = getopt(argc, argv, "c:k:l:p:h:t:s:")) != -1) {
        switch (opt) {
            case 's':
                state = atoi(optarg);
                break;
            case 'c':
                if (strlen(optarg) > 0) {
                    digit = optarg[0];
                }
                break;
            case 'k':
                k_command = optarg;
                break;
            case 'l':
                l_command = optarg;
                break;
            case 'p':
                ce_press = atoi(optarg);
                break;
            case 'h':
                debounce_hold = atoi(optarg);
                break;
            case 't':
                ch_timeout = atoi(optarg);
                break;
            default:
                return 1;
        }
    }

    if (digit < '0' || digit > '9' || k_command == NULL || (state != 1 && state != 2)) {
        printf("[LOG] Перевірка параметрів НЕ ПРОЙШЛА! Виліт. (digit=%c, state=%d)\n", digit, state); fflush(stdout);
        return 1;
    }

    // --- ОБРОБКА СТАНУ 2 (УТРИМАННЯ КНОПКИ) ---
    if (state == 2) {
        printf("[LOG] Зайшли в блок STATE 2 (Утримання)\n"); fflush(stdout);
        FILE *f_hf = fopen(HOLD_FLAG_FILE, "w");
        if (f_hf) { 
            fflush(f_hf);
            fclose(f_hf); 
        }

        long long current_time = get_current_ms();
        long long first_press_time = 0;
        char temp_d = '\0';
        
        FILE *f_t = fopen(LAST_TIME_FILE, "r");
        if (f_t) {
            if (fscanf(f_t, "%lld %c", &first_press_time, &temp_d) == 2 && digit == temp_d) {
                if ((current_time - first_press_time) >= debounce_hold) {
                    fclose(f_t);
                    printf("[LOG] Спрацював поріг утримання >= %d мс. Очищаємо буфери.\n", debounce_hold); fflush(stdout);

                    unlink(BUFFER_FILE);
                    unlink(LOCK_FILE);
                    unlink(CMD_FILE);
                    unlink(LAST_TIME_FILE);

                    if (l_command != NULL && strlen(l_command) > 0) {
                        FILE *f_time_reset = fopen(LAST_TIME_FILE, "w");
                        if (f_time_reset) {
                            fprintf(f_time_reset, "%lld X", current_time);
                            fflush(f_time_reset);
                            fclose(f_time_reset);
                        }

                        char final_l_cmd[512];
                        snprintf(final_l_cmd, sizeof(final_l_cmd), "%s %c", l_command, digit);
                        printf("[LOG] Виконуємо команду утримання (-l): %s\n", final_l_cmd); fflush(stdout);
                        system(final_l_cmd);
                    }
                    return 0;
                }
            }
            fclose(f_t);
        }
        // ВИПРАВЛЕНО: Перед виходом з короткого утримання обов'язково чистимо сміттєвий статус
        unlink(HOLD_FLAG_FILE);
        printf("[LOG] Потік STATE 2 завершився без виконання команди\n"); fflush(stdout);
        return 0;
    }

    // --- ОБРОБКА СТАНУ 1 (КОРОТКИЙ КЛІК) ---
    else if (state == 1) {
        long long current_time = get_current_ms();

        FILE *f_time = fopen(LAST_TIME_FILE, "r");
        if (f_time) {
            long long last_time = 0;
            char last_digit = '\0';
            if (fscanf(f_time, "%lld %c", &last_time, &last_digit) == 2) {
                if (digit == last_digit && (current_time - last_time) < ce_press) {
                    printf("[LOG] Спрацював захист від брязкоту (-p). Ігноруємо клік.\n"); fflush(stdout);
                    fclose(f_time);
                    return 0; 
                }
            }
            fclose(f_time);
        }
        
        f_time = fopen(LAST_TIME_FILE, "w");
        if (f_time) {
            fprintf(f_time, "%lld %c", current_time, digit);
            fflush(f_time);
            fclose(f_time);
        }

        printf("[LOG] Додаємо цифру '%c' в буфер накопичення\n", digit); fflush(stdout);
        FILE *f_buf = fopen(BUFFER_FILE, "a+");
        if (f_buf) {
            fputc(digit, f_buf);
            fflush(f_buf);
        }

        FILE *f_cmd = fopen(CMD_FILE, "w+");
        if (f_cmd) {
            fprintf(f_cmd, "%s", k_command);
            fflush(f_cmd);
        }

        pid_t my_pid = getpid();
        FILE *f_lock = fopen(LOCK_FILE, "w+");
        if (f_lock) {
            fprintf(f_lock, "%d", my_pid);
            fflush(f_lock); 
            printf("[LOG] МІТКА: LOCK_FILE успешно создано. Записано PID: %d\n", my_pid); fflush(stdout);
        } else {
            printf("[LOG] МІТКА ПОМИЛКИ: Не вдалося створити LOCK_FILE на запис!\n"); fflush(stdout);
        }

        printf("[LOG] Лягаємо спати на таймаут: %d мс...\n", ch_timeout); fflush(stdout);
        usleep(ch_timeout * 1000);
        printf("[LOG] Прокинулися після сну! Перевіряємо маркери...\n"); fflush(stdout);

        if (f_lock) {
            struct stat st;
            if (stat(LOCK_FILE, &st) == 0) {
                int read_pid = 0;
                rewind(f_lock); 
                if (fscanf(f_lock, "%d", &read_pid) == 1) {
                    printf("[LOG] МІТКА ДІАГНОСТИКИ: LOCK_FILE на диску Є! Вміст PID=%d, Розмір файла=%ld байт\n", read_pid, (long)st.st_size); fflush(stdout);
                }
            } else {
                printf("[LOG] МІТКА ДІАГНОСТИКИ: LOCK_FILE фізично ВІДСУТНІЙ на диску після сну!\n"); fflush(stdout);
            }
        }

        if (access(HOLD_FLAG_FILE, F_OK) == 0) {
            printf("[LOG] ЗНАЙДЕНО МАРКЕР УТРИМАННЯ! Звичайна команда СКАСОВУЄТЬСЯ.\n"); fflush(stdout);
            if (f_lock) {
                pid_t latest_pid = 0;
                rewind(f_lock);
                if (fscanf(f_lock, "%d", &latest_pid) == 1 && my_pid == latest_pid) {
                    printf("[LOG] Ми останні в черзі. Повністю зачищаємо файли.\n"); fflush(stdout);
                    unlink(HOLD_FLAG_FILE);
                    unlink(BUFFER_FILE);
                    unlink(CMD_FILE);
                    unlink(LAST_TIME_FILE);
                    if (f_buf) fclose(f_buf);
                    if (f_cmd) fclose(f_cmd);
                    fclose(f_lock);
                    unlink(LOCK_FILE);
                } else {
                    if (f_buf) fclose(f_buf);
                    if (f_cmd) fclose(f_cmd);
                    fclose(f_lock);
                }
            } else {
                if (f_buf) fclose(f_buf);
                if (f_cmd) fclose(f_cmd);
                unlink(HOLD_FLAG_FILE);
            }
            return 0;
        }

        if (f_lock) {
            pid_t latest_pid = 0;
            rewind(f_lock);
            if (fscanf(f_lock, "%d", &latest_pid) == 1 && my_pid == latest_pid) {
                printf("[LOG] Перевірка PID успішна: ми є останнім кліком. Готуємо команду.\n"); fflush(stdout);

                if (f_buf && f_cmd) {
                    char raw_channel[32] = {0};
                    char exec_base[256] = {0};

                    rewind(f_buf); 
                    rewind(f_cmd); 

                    if (fgets(raw_channel, sizeof(raw_channel) - 1, f_buf) &&
                        fgets(exec_base, sizeof(exec_base) - 1, f_cmd)) {
                        
                        long channel = strtol(raw_channel, NULL, 10);
                        printf("[LOG] Зчитано з буфера: канал=%ld, базова команда='%s'\n", channel, exec_base); fflush(stdout);

                        fclose(f_buf);
                        fclose(f_cmd);
                        fclose(f_lock);

                        unlink(BUFFER_FILE);
                        unlink(CMD_FILE);
                        unlink(LAST_TIME_FILE);
                        unlink(HOLD_FLAG_FILE);
                        unlink(LOCK_FILE);

                        char final_cmd[512];
                        snprintf(final_cmd, sizeof(final_cmd), "%s %ld", exec_base, channel);
                        
                        printf("[LOG] ЗАПУСК КОМАНДИ ЧЕРЕЗ system(): %s\n", final_cmd); fflush(stdout);
                        system(final_cmd);
                        return 0;
                    } else {
                        printf("[LOG] Помилка: fgets не зміг прочитати дані з утримуваних файлів!\n"); fflush(stdout);
                    }
                } else {
                    printf("[LOG] Помилка: Файли буфера або команди не утримувалися в пам'яті!\n"); fflush(stdout);
                }
                
                if (f_buf) fclose(f_buf);
                if (f_cmd) fclose(f_cmd);
                fclose(f_lock);
                unlink(BUFFER_FILE);
                unlink(CMD_FILE);
                unlink(LAST_TIME_FILE);
                unlink(HOLD_FLAG_FILE);
                unlink(LOCK_FILE);
            } else {
                printf("[LOG] Ми НЕ останні в черзі (PID не збігається). Залишаємо виконання наступному потоку.\n"); fflush(stdout);
                if (f_buf) fclose(f_buf);
                if (f_cmd) fclose(f_cmd);
                fclose(f_lock);
            }
        } else {
            printf("[LOG] Помилка: Не вдалося перевірити фінального PID через відсутній f_lock!\n"); fflush(stdout);
            if (f_buf) fclose(f_buf);
            if (f_cmd) fclose(f_cmd);
        }
    }

    return 0;
}
