#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define FILE_PATH "/tmp/hh"

// Константи регістрів TDA7313
const int REG_VOLUME   = 0;
const int REG_BASS     = 96;  // 0x60
const int REG_TREBLE   = 112; // 0x70
const int REG_SPEC_LF  = 128; // 0x80
const int REG_SPEC_RF  = 160; // 0xA0
const int REG_SPEC_LR  = 192; // 0xC0
const int REG_SPEC_RR  = 224; // 0xE0
const int REG_SWITCH   = 64;  // 0x40

const int LOUD_OFF     = 4;
const int LOUD_ON      = 0;
const int LOUD_ON0db   = 24;
const int LOUD_ON1db   = 16;
const int LOUD_ON2db   = 8;
const int LOUD_ON3db   = 0;
const int MUTE_VAL     = 31;

// Структура для зберігання стану конфігурації
typedef struct {
    int volume;
    int subtrebl;
    int subbas;
    int bass;
    int treble;
    int input;
    int loundst;
    int mute;
} TDAState;

// Глобальні змінні з базовими значеннями (якщо в UCI параметри відсутні)
int i2c_adres = 0x44;
int i2c_dev = 0;

// Функція запису байта в I2C
void write_CMD(unsigned char hh) {
    char dev_name[20];
    sprintf(dev_name, "/dev/i2c-%d", i2c_dev);
    int file = open(dev_name, O_RDWR);
    if (file < 0) return;
    
    if (ioctl(file, I2C_SLAVE, i2c_adres) >= 0) {
        if (write(file, &hh, 1) != 1) {
            // Помилка запису (ігноруємо або логуємо)
        }
    }
    close(file);
}

// Функція для отримання текстового значення з UCI OpenWrt
void get_uci_value(const char *command, char *output, size_t max_len) {
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        output[0] = '\0';
        return;
    }
    if (fgets(output, max_len, fp) != NULL) {
        size_t len = strlen(output);
        if (len > 0 && output[len - 1] == '\n') {
            output[len - 1] = '\0';
        }
    } else {
        output[0] = '\0';
    }
    pclose(fp);
}

// Запис стану у файл /tmp/hh
void save_state(TDAState *state) {
    FILE *f = fopen(FILE_PATH, "w");
    if (!f) return;
    fprintf(f, "volume %d\nsubtrebl %d\nsubbas %d\nbass %d\ntreble %d\ninput %d\nloundst %d\nmute %d\n",
            state->volume, state->subtrebl, state->subbas, state->bass, state->treble, state->input, state->loundst, state->mute);
    fclose(f);
}

// Читання стану з файлу /tmp/hh
int load_state(TDAState *state) {
    FILE *f = fopen(FILE_PATH, "r");
    if (!f) return 0;
    
    char key[20];
    int val;
    
    state->volume = 40; state->subtrebl = 20; state->subbas = 15;
    state->bass = 12; state->treble = 12; state->input = 1;
    state->loundst = 1; state->mute = 1;

    while (fscanf(f, "%19s %d", key, &val) == 2) {
        if (strcmp(key, "volume") == 0) state->volume = val;
        else if (strcmp(key, "subtrebl") == 0) state->subtrebl = val;
        else if (strcmp(key, "subbas") == 0) state->subbas = val;
        else if (strcmp(key, "bass") == 0) state->bass = val;
        else if (strcmp(key, "treble") == 0) state->treble = val;
        else if (strcmp(key, "input") == 0) state->input = val;
        else if (strcmp(key, "loundst") == 0) state->loundst = val;
        else if (strcmp(key, "mute") == 0) state->mute = val;
    }
    fclose(f);
    return 1;
}

// Розрахунок параметрів тонкомпенсації
void lound_CMD(int loundst, int *lost, int *lodb) {
    switch (loundst) {
        case 1: *lost = LOUD_OFF; *lodb = LOUD_ON0db; break;
        case 2: *lost = LOUD_ON;  *lodb = LOUD_ON0db; break;
        case 3: *lost = LOUD_ON;  *lodb = LOUD_ON1db; break;
        case 4: *lost = LOUD_ON;  *lodb = LOUD_ON2db; break;
        case 5: *lost = LOUD_ON;  *lodb = LOUD_ON3db; break;
        default: *lost = LOUD_OFF; *lodb = LOUD_ON0db; break;
    }
}

// Ініціалізація аудіопроцесора з Soft Fade-in за допомогою usleep()
void tda7313_init() {
    TDAState state = {40, 20, 15, 12, 12, 1, 1, 1};
    save_state(&state);

    write_CMD(REG_VOLUME + 63); // Повна тиша спочатку

    write_CMD(REG_SPEC_LF + (31 - 20));
    write_CMD(REG_SPEC_RF + (31 - 20));
    write_CMD(REG_SPEC_LR + (31 - 15));
    write_CMD(REG_SPEC_RR + (31 - 15));
    write_CMD(REG_BASS + (23 - 12));
    write_CMD(REG_TREBLE + (23 - 12));
    write_CMD(REG_SWITCH + 1 + LOUD_OFF + LOUD_ON0db);

    // Плавний старт (Soft Fade-in)
    int current_atten = 63;
    int target_atten = 63 - state.volume;
    while (current_atten >= target_atten) {
        write_CMD(REG_VOLUME + current_atten);
        current_atten--;
        usleep(15000); // Чисті 15 мілісекунд затримки
    }
}

int calc_tembr(int reg_base, int vlm) {
    if (vlm <= 7) return reg_base + vlm;
    else return reg_base + (23 - vlm);
}

void print_help() {
    printf("\n\ttda7313 init\t# initialization\n\n\ttda7313 <type> <value>\n");
    printf("\t\ttype:\tinit, volume, bass, treble, streble, sbass, switch, LF, RF, LR, RR, lound\n");
    printf("\t\tvalue:\t+, -, 0-63\n\n");
    printf("\tExample:\n\t tda7313 volume +\n");
}

int main(int argc, char *argv[]) {
    char uci_buf[32];

    // Зчитуємо UCI. Якщо параметри відсутні — змінні зберігають дефолтні 0x44 та 0
    get_uci_value("uci -q get tda7313.@tda7313.i2c_adres", uci_buf, sizeof(uci_buf));
    if (strlen(uci_buf) > 0) {
        i2c_adres = (int)strtol(uci_buf, NULL, 0); 
    }

    get_uci_value("uci -q get tda7313.@tda7313.i2c_dev", uci_buf, sizeof(uci_buf));
    if (strlen(uci_buf) > 0) {
        i2c_dev = atoi(uci_buf);
    }

    if (argc < 2) {
        print_help();
        return 1;
    }

    char *cmd = argv[1];
    char *val = (argc > 2) ? argv[2] : "";

    if (strcmp(cmd, "init") == 0 || strcmp(cmd, "tda7313_init") == 0) {
        tda7313_init();
        return 0;
    }

    TDAState state;
    if (!load_state(&state)) {
        tda7313_init();
        load_state(&state);
    }

    // MUTE
    if (strcmp(cmd, "mute") == 0) {
        if (state.mute == 1) {
            write_CMD(MUTE_VAL);
            state.mute = 0;
            state.volume = state.volume - 1;
            save_state(&state);
        }
    }
    // SWITCH
    else if (strcmp(cmd, "switch") == 0 || strcmp(cmd, "aux") == 0 || strcmp(cmd, "input") == 0) {
        int input_val = atoi(val);
        if (input_val >= 0 && input_val <= 3) {
            int lost, lodb;
            lound_CMD(state.loundst, &lost, &lodb);
            write_CMD(REG_SWITCH + input_val + lost + lodb + 32); // Анти-клік Mute
            usleep(20000);
            write_CMD(REG_SWITCH + input_val + lost + lodb);
            state.input = input_val;
            save_state(&state);
        }
    }
    // LOUND
    else if (strcmp(cmd, "lound") == 0) {
        if (strcmp(val, "+") == 0) {
            state.loundst++;
            if (state.loundst == 6) state.loundst = 1;
            int lost, lodb;
            lound_CMD(state.loundst, &lost, &lodb);
            write_CMD(REG_SWITCH + state.input + lost + lodb);
            save_state(&state);
        }
    }
    // VOLUME
    else if (strcmp(cmd, "volume") == 0) {
        if (state.mute == 0) state.mute = 1;
        if (strcmp(val, "+") == 0) {
            if (state.volume < 63) state.volume++;
        } else if (strcmp(val, "-") == 0) {
            if (state.volume > 0) state.volume--;
        } else if (argc > 2) {
            int v = atoi(val);
            if (v >= 0 && v <= 63) state.volume = v;
        }
        write_CMD(REG_VOLUME + (63 - state.volume));
        save_state(&state);
    }
    // TREBLE
    else if (strcmp(cmd, "treble") == 0) {
        if (strcmp(val, "+") == 0) {
            if (state.treble < 15) state.treble++;
        } else if (strcmp(val, "-") == 0) {
            if (state.treble > 0) state.treble--;
        } else if (argc > 2) {
            int v = atoi(val);
            if (v >= 0 && v <= 15) state.treble = v;
        }
        write_CMD(calc_tembr(REG_TREBLE, state.treble));
        save_state(&state);
    }
    // BASS
    else if (strcmp(cmd, "bass") == 0) {
        if (strcmp(val, "+") == 0) {
            if (state.bass < 15) state.bass++;
        } else if (strcmp(val, "-") == 0) {
            if (state.bass > 0) state.bass--;
        } else if (argc > 2) {
            int v = atoi(val);
            if (v >= 0 && v <= 15) state.bass = v;
        }
        write_CMD(calc_tembr(REG_BASS, state.bass));
        save_state(&state);
    }
    // SBASS & STREBLE
    else if (strcmp(cmd, "sbass") == 0 || strcmp(cmd, "streble") == 0) {
        int *vlm_ptr = (strcmp(cmd, "sbass") == 0) ? &state.subbas : &state.subtrebl;
        if (strcmp(val, "+") == 0) {
            if (*vlm_ptr < 31) (*vlm_ptr)++;
        } else if (strcmp(val, "-") == 0) {
            if (*vlm_ptr > 0) (*vlm_ptr)--;
        } else if (argc > 2) {
            int v = atoi(val);
            if (v >= 0 && v <= 31) *vlm_ptr = v;
        }
        int reg_val = 31 - (*vlm_ptr);
        if (strcmp(cmd, "sbass") == 0) {
            write_CMD(REG_SPEC_LR + reg_val);
            write_CMD(REG_SPEC_RR + reg_val);
        } else {
            write_CMD(REG_SPEC_LF + reg_val);
            write_CMD(REG_SPEC_RF + reg_val);
        }
        save_state(&state);
    }
    // LF, RF, LR, RR
    else if (strcmp(cmd, "LF") == 0 || strcmp(cmd, "RF") == 0 || strcmp(cmd, "LR") == 0 || strcmp(cmd, "RR") == 0) {
        int reg_base = 0;
        if (strcmp(cmd, "LF") == 0) reg_base = REG_SPEC_LF;
        else if (strcmp(cmd, "RF") == 0) reg_base = REG_SPEC_RF;
        else if (strcmp(cmd, "LR") == 0) reg_base = REG_SPEC_LR;
        else if (strcmp(cmd, "RR") == 0) reg_base = REG_SPEC_RR;
        int v = atoi(val);
        if (v >= 0 && v <= 31) write_CMD(reg_base + (31 - v));
    } else {
        print_help();
    }

    /*
    printf("\n  reg+vol - %s: %d\n", cmd, REG_VOLUME + (63 - state.volume));
    printf("  State: vol:%d subtrebl:%d subbas:%d bass:%d treble:%d input:%d\n", 
           state.volume, state.subtrebl, state.subbas, state.bass, state.treble, state.input);
    */
    return 0;
}

