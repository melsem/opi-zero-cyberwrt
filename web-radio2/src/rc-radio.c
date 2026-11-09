#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>

#define PLAYLIST "/etc/config/web-radio2"
#define TFILE "/tmp/play_name"
#define LAST_VOL_FILE "/tmp/radio_last_vol"
#define DEFAULT_VOL 59
#define MAX_LINE 512

// Задержка микросекунд между шагами громкости (1000 мкс = 1 мс)
// 5000 мкс (5 мс) на шаг при громкости 59 даст полное затухание/нарастание примерно за 0.3 секунды.
// Сделайте меньше (например, 3000), чтобы звук менялся быстрее, или больше для более медленного эффекта.
// #define FADE_DELAY_US 5000 
#define FADE_DELAY_US 3000 

// --- Вспомогательные функции звука (бывший fade.sh) ---

int get_current_volume() {
    FILE *fp;
    char path[1034];
    int vol = DEFAULT_VOL;

    // Читаем громкость через amixer
    fp = popen("amixer -c 0 get 'DAC' 2>/dev/null", "r");
    if (fp == NULL) return vol;

    while (fgets(path, sizeof(path), fp) != NULL) {
        if (strstr(path, "Front Left:") != NULL) {
            char *p = strchr(path, '[');
            if (p) {
                p++;
                int v = atoi(p);
                if (v >= 0 && v <= 100) {
                    vol = v;
                    break;
                }
            }
        }
    }
    pclose(fp);
    return vol;
}

void set_volume(int vol) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "amixer -c 0 -q set 'DAC' %d", vol);
    system(cmd);
}

// Поправленная функция sound_off, чтобы она принудительно перезаписывала сохраненный уровень
void sound_off() {
    int current_dac = get_current_volume();
    if (current_dac > DEFAULT_VOL) current_dac = DEFAULT_VOL;

    // Всегда сохраняем текущий уровень перед выключением, если он выше 0.
    // Если он уже 0 (например, было Mute), то сохранять 0 не имеет смысла, оставим старое значение в файле.
    if (current_dac > 0) {
        FILE *f = fopen(LAST_VOL_FILE, "w");
        if (f) {
            fprintf(f, "%d\n", current_dac);
            fclose(f);
        }

        // Плавное затухание
        for (int vol = current_dac; vol >= 0; vol--) {
            set_volume(vol);
            usleep(FADE_DELAY_US); // 5 мс на каждый шаг (при громкости 59 затухание займет ~0.3 сек)
        }
    }
    set_volume(0);
}

void sound_on() {
    int target_dac = DEFAULT_VOL;
    FILE *f = fopen(LAST_VOL_FILE, "r");
    if (f) {
        if (fscanf(f, "%d", &target_dac) != 1) {
            target_dac = DEFAULT_VOL;
        }
        fclose(f);
    }
    if (target_dac > DEFAULT_VOL || target_dac <= 0) target_dac = DEFAULT_VOL;

    // Плавное нарастание
    for (int vol = 0; vol <= target_dac; vol++) {
        set_volume(vol);
        usleep(FADE_DELAY_US); // Пауза 5 мс
    }
    set_volume(target_dac);
    unlink(LAST_VOL_FILE); // Удаляем файл
}

// --- Управление процессами ---

void kill_processes() {
    // Заключаем в фигурные скобки и глушим абсолютно весь вывод, 
    // чтобы Bash не рапортовал о завершении фоновых задач
    system("{ killall madplay curl wget; } >/dev/null 2>&1");
    usleep(50000); // Пауза 50 мс
    system("{ killall -9 madplay curl wget; } >/dev/null 2>&1");
}

void kill_webradio() {
    system("{ killall -9 web-radio2; } >/dev/null 2>&1");
}

int is_webradio_running() {
    return (system("pidof web-radio2 &> /dev/null") == 0);
}

// Очистка строки от кавычек (аналог sed "s/\"\|\'//g")
void strip_quotes(char *src, char *dst) {
    while (*src) {
        if (*src != '"' && *src != '\'') {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';
}

// Поиск номера станции в плейлисте (аналог sed-скрипта)
int find_station_number(const char *station_name) {
    FILE *fp = fopen(PLAYLIST, "r");
    if (!fp) return 1;

    char line[MAX_LINE];
    int numb = 1;

    // Ищем строку формата #EXTINF:-номер,"Имя Станции"
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "#EXTINF:-", 9) == 0) {
            char *comma = strchr(line, ',');
            if (comma) {
                char clean_name[MAX_LINE];
                // Выделяем имя после запятой, убирая кавычки и перенос строки
                char *name_ptr = comma + 1;
                // Удаляем \n на конце
                name_ptr[strcspn(name_ptr, "\r\n")] = 0;
                strip_quotes(name_ptr, clean_name);

                if (strcmp(clean_name, station_name) == 0) {
                    numb = atoi(line + 9);
                    fclose(fp);
                    return (numb > 0) ? numb : 1;
                }
            }
        }
    }

    fclose(fp);
    return 1;
}

int get_last_station_num() {
    FILE *fp = fopen(TFILE, "r");
    if (!fp) return 1;

    char raw_name[MAX_LINE];
    if (!fgets(raw_name, sizeof(raw_name), fp)) {
        fclose(fp);
        return 1;
    }
    fclose(fp);

    // Убираем перенос строки
    raw_name[strcspn(raw_name, "\r\n")] = 0;

    char clean_name[MAX_LINE];
    strip_quotes(raw_name, clean_name);

    if (strlen(clean_name) == 0) return 1;

    return find_station_number(clean_name);
}

void start_webradio(int station_num) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "web-radio2 -p %d &", station_num);
    system(cmd);
}

// --- Основные команды управления ---

void cmd_start() {
    sound_off();

    if (!is_webradio_running()) {
        kill_webradio();
        kill_processes();
        
        int station = get_last_station_num();
        start_webradio(station);
    } else {
        kill_webradio();
        kill_processes();
    }

    sound_on();
}

void cmd_stop() {
    sound_off();
    kill_webradio();
    kill_processes();
}

// Модифицированная функция reload
void cmd_reload(const char *arg) {
    // 1. Плавно тушим звук. Текущая громкость запишется в /tmp/radio_last_vol
    sound_off();

    // 2. Проверяем аргумент (номер станции)
    if (arg == NULL || strcmp(arg, "0") == 0 || strlen(arg) == 0) {
        // Если передали 0 или пусто — просто останавливаем процессы и звук НЕ возвращаем
        kill_processes();
    } else {
        // Если передали номер (например, "3")
        kill_webradio();
        kill_processes();
        
        // Запускаем новую станцию
        start_webradio(atoi(arg));
        
        // Короткая пауза 100мс, чтобы curl/madplay успели инициализироваться перед подачей звука
        usleep(100000); 

        // 3. Плавно возвращаем звук до уровня, сохраненного секунду назад в sound_off()
        sound_on();
    }
}

void cmd_restart(const char *arg) {
    if (arg != NULL && strcmp(arg, "0") == 0) {
        int current_dac = get_current_volume();
        if (current_dac > 0) {
            sound_off(); // Mute
        } else {
            sound_on();  // Unmute
        }
    } else {
        // Обычный перезапуск
        sound_off();
        kill_webradio();
        kill_processes();
        system("web-radio2 &");
        sound_on();
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s {start|stop|reload|restart|mute|unmute} [аргумент]\n", argv[0]);
        return 1;
    }

    char *command = argv[1];
    char *argument = (argc > 2) ? argv[2] : NULL;

    // --- ПРОВЕРКА: Если вместо команды ввели просто число (номер станции) ---
    // Проверяем, состоит ли первый аргумент целиком из цифр
    int is_numeric = 1;
    for (int i = 0; command[i] != '\0'; i++) {
        if (!isdigit(command[i])) {
            is_numeric = 0;
            break;
        }
    }
    
    if (is_numeric) {
        // Перенаправляем логику: аргументом становится само число, а командой — reload
        argument = command;
        command = "reload";
    }
    // -----------------------------------------------------------------------

    if (strcmp(command, "start") == 0) {
        cmd_start();
    } else if (strcmp(command, "stop") == 0) {
        cmd_stop();
    } else if (strcmp(command, "reload") == 0) {
        cmd_reload(argument);
    } else if (strcmp(command, "restart") == 0) {
        cmd_restart(argument);
    } else if (strcmp(command, "sound_off") == 0 || strcmp(command, "mute") == 0) {
        sound_off();
    } else if (strcmp(command, "sound_on") == 0 || strcmp(command, "unmute") == 0) {
        sound_on();
    } else {
        printf("Неизвестная команда: %s\n", command);
        return 1;
    }

    return 0;
}
