// Чиста функція обрізання пробілів та переносів рядків з обох боків
static void trim_spaces_pure(char *str) {
    int len = strlen(str);
    while (len > 0 && (isspace((unsigned char)str[len - 1]) || str[len - 1] == '\r' || str[len - 1] == '\n')) {
        str[len - 1] = '\0';
        len--;
    }
    int start = 0;
    while (str[start] && isspace((unsigned char)str[start])) {
        start++;
    }
    if (start > 0) {
        memmove(str, &str[start], len - start + 1);
    }
}

// Функція покрокового зчитування рядків конфігів (точний аналог sed)
static int get_line_from_file_pure(const char *filename, int line_num, char *buffer, int max_len) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    int current_line = 1;
    int found = 0;
    memset(buffer, 0, max_len);
    while (fgets(buffer, max_len, f)) {
        if (current_line == line_num) {
            trim_spaces_pure(buffer);
            found = 1;
            break;
        }
        current_line++;
    }
    fclose(f);
    return (found && strlen(buffer) > 0);
}

// Головна функція обробки оверлеїв (ВИПРАВЛЕНО: тепер char *argv[])
void dt_overlays(int over_lays, char *argv[]) {
    FILE *fd_check = fopen(tmp_dts, "r");
    FILE *fd_spi_check = fopen(tmp_SPI, "r");

    if (fd_spi_check == NULL && fd_check == NULL) {
        if (fd_spi_check) fclose(fd_spi_check); if (fd_check) fclose(fd_check);
        test_dts(TEST_DTS_MMC, DTB_TO_DTS);
    } else if (fd_spi_check != NULL && fd_check == NULL) {
        if (fd_spi_check) fclose(fd_spi_check); if (fd_check) fclose(fd_check);
        test_dts(TEST_DTS_SPI, DTB_TO_DTS);
    }
    if (fd_spi_check) fclose(fd_spi_check);
    if (fd_check) fclose(fd_check);

    // Рахуємо кількість модулів
    int nmbstr = 0;
    char dummy[520]; 
    FILE *f_mod = fopen(f_moduls, "r");
    if (f_mod) {
        while (fgets(dummy, sizeof(dummy), f_mod)) {
            trim_spaces_pure(dummy);
            if (strlen(dummy) > 0) nmbstr++;
        }
        fclose(f_mod);
    } else {
        exit(EXIT_FAILURE);
    }

    // Читаємо весь робочий DTS файл у пам'ять для швидкої інлайн-заміни
    FILE *f_r = fopen(tmp_dts, "r");
    if (!f_r) exit(EXIT_FAILURE);
    fseek(f_r, 0, SEEK_END);
    long dts_size = ftell(f_r);
    rewind(f_r);

    char *dts_content = malloc(dts_size + 2048); // даємо запас під аліаси
    if (!dts_content) { fclose(f_r); exit(EXIT_FAILURE); }
    long r_bytes = fread(dts_content, 1, dts_size, f_r);
    dts_content[r_bytes] = '\0';
    fclose(f_r);

    // Цикл дописування аліасів перед назвами модулів
    for (int i = 1; i <= nmbstr; i++) {
        char local_moduls[520] = {0};   
        char local_aliases[520] = {0};  

        if (!get_line_from_file_pure(f_moduls, i, local_moduls, sizeof(local_moduls))) continue;
        if (!get_line_from_file_pure(f_aliases, i, local_aliases, sizeof(local_aliases))) continue;

        // Шукаємо точний блок модуля у тексті (наприклад, "i2c@1c28400 {")
        char target_find[560];
        snprintf(target_find, sizeof(target_find), "%s {", local_moduls);

        char *pos = strstr(dts_content, target_find);
        if (!pos) continue; // Якщо модуля в цьому DTS немає — просто йдемо далі


/*        printf("[LOG] ----------------------------------------\n");
        printf("[LOG] Модифікований рядок:  %s %s {\n", local_aliases, local_moduls);
        printf("[LOG] ----------------------------------------\n");
*/
        // Робимо миттєву інлайн-заміну в буфері пам'яті (вставляємо аліас перед назвою модуля)
        long offset = pos - dts_content;
        char *new_content = malloc(dts_size + 2048);
        if (new_content) {
            // Копіюємо все, що йшло ДО цього модуля
            memcpy(new_content, dts_content, offset);
            new_content[offset] = '\0';

            // Додаємо аліас із пробілом
            strcat(new_content, local_aliases);
            strcat(new_content, " ");

            // Дописуємо сам модуль із фігурною дужкою та всім іншим хвостом файла
            strcat(new_content, pos);

            free(dts_content);
            dts_content = new_content;
            dts_size = strlen(dts_content);
        }
    }

    // Записуємо змінений у пам'яті текст назад у робочий файл
    FILE *f_w = fopen(tmp_dts, "w");
    if (f_w) {
        fputs(dts_content, f_w);
        fclose(f_w);
    }
    free(dts_content);

    // Дописуємо файл оверлею в самий кінець (Аналог cat >>)
    char overlay_path[560];
    if (over_lays == OVERLAYS)
        snprintf(overlay_path, sizeof(overlay_path), "/etc/dt-overlays/%s_overlays.dts", argv[2 + ofset]);
    else if (over_lays == OVERLAYS_ERASE)
        snprintf(overlay_path, sizeof(overlay_path), "/etc/dt-overlays/%s-del_overlays.dts", argv[2 + ofset]);

    FILE *f_dest = fopen(tmp_dts, "a"); 
    FILE *f_src = fopen(overlay_path, "r");
    int append_success = 0;

    if (f_dest && f_src) {
        int ch;
        while ((ch = fgetc(f_src)) != EOF) {
            fputc(ch, f_dest);
        }
        append_success = 1;
    }
    if (f_dest) fclose(f_dest);
    if (f_src) fclose(f_src);

    if (!append_success) {
        if (over_lays == OVERLAYS)
            printf("   Error: < %s_overlays.dts > is missing\n   or check the name you entered.\n", argv[2 + ofset]);
        else if (over_lays == OVERLAYS_ERASE)
            printf("   Error: < %s-del_overlays.dts > is missing\n   or check the name you entered.\n", argv[2 + ofset]);
        
        remove(dtb_tmp);
        remove(tmp_dts);
        remove(tmp_SPI);
        exit(EXIT_FAILURE);
    } else {
        if (over_lays == OVERLAYS_ERASE) 
            printf("   Overlay module < %s > has been removed from the dts file.\n", argv[2 + ofset]);
        else if (over_lays == OVERLAYS) 
            printf("   Added < %s > overlay module to dts file.\n", argv[2 + ofset]);

        FILE *fd_final;
        if (((fd_final = fopen(tmp_SPI, "r")) == NULL) && ((fd_final = fopen(tmp_dts, "r")) != NULL)) {
            fclose(fd_final);
            test_dts(DTS_TO_DTB_MMC, OVERLAYS);
        }
        else if (((fd_final = fopen(tmp_SPI, "r")) != NULL) && ((fd_final = fopen(tmp_dts, "r")) != NULL)) {
            fclose(fd_final);
            test_dts(DTS_TO_DTB_SPI, OVERLAYS);
        }
    }
    exit(EXIT_FAILURE); 
}
