#include <stdio.h>
#include <string.h>
#include "opl.h"

int get_station_by_index(const char *playlist_path, int target_index, char *out_name, char *out_url) {
    FILE *f = fopen(playlist_path, "r");
    if (!f) return 0; 

    char line[2048];
    char search_pattern[64];
    
    snprintf(search_pattern, sizeof(search_pattern), "#EXTINF:-%d,", target_index);

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;

        if (strncmp(line, search_pattern, strlen(search_pattern)) == 0) {
            
            char *start = strchr(line, '"');
            if (start) {
                start++; 
                char *end = strrchr(start, '"');
                if (end) *end = '\0'; 
                strcpy(out_name, start);
            } else {
                strcpy(out_name, line + strlen(search_pattern));
            }

            if (fgets(line, sizeof(line), f)) {
                line[strcspn(line, "\r\n")] = 0;
                strcpy(out_url, line);
                fclose(f);
                return 1; 
            }
        }
    }

    fclose(f);
    return 0; 
}
