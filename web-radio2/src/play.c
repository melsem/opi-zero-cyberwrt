#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "play.h"
#include "opl.h"
#include "error_log.h"

void kiledd() {
    system("killall -9 curl wget madplay 2>/dev/null");
}

void write_play_name(const char *name) {
    FILE *f = fopen("/tmp/play_name", "w");
    if (f) {
        fputs(name, f);
        fclose(f);
    } else {
        error_log("Failed to write /tmp/play_name");
    }
}

int is_madplay_running() {
    FILE *fp = popen("pidof madplay", "r");
    if (fp == NULL) return 0;
    
    char pid_buf[16];
    if (fgets(pid_buf, sizeof(pid_buf), fp) != NULL) {
        pclose(fp);
        return 1; 
    }
    pclose(fp);
    return 0; 
}

void play(char *tmpplay, int menbit) {
    char *cmd_play = NULL;
    kiledd();

/*  const char *curl_opts = "--silent --fail --connect-timeout 4 --max-time 8 --speed-limit 1000 --speed-time 10";  */
    const char *curl_opts = "--silent --fail --connect-timeout 15 --speed-limit 1000 --speed-time 20";

    if (menbit == 1) { 
        if (nameBit == 0) write_play_name(tmpplay);
        else if (nameBit == 1) write_play_name(name_url);

        if (asprintf(&cmd_play, "curl %s %s | madplay - -Q --no-tty-control", curl_opts, tmpplay) != -1) {
            system(cmd_play);
            free(cmd_play);
        }
    } 
    else if (menbit == 2) { 
        write_play_name(tmpplay);
        if (asprintf(&cmd_play, "madplay -Q --no-tty-control \"%s\" &", tmpplay) != -1) {
            system(cmd_play);
            free(cmd_play);
        }
    } 
    else if (menbit == 3) { 
        write_play_name(tmpplay);
        if (asprintf(&cmd_play, "madplay -Q --no-tty-control %s/*.mp3 &", tmpplay) != -1) {
            system(cmd_play);
            free(cmd_play);
        }
    } 
    else if (menbit == 0) { 
        char station_name[BUFSIZE];
        char station_url[BUFSIZE];

        while (1) {
            kiledd();
            usleep(100000); 

            if (get_station_by_index(tmpplay, buff1, station_name, station_url)) {
                write_play_name(station_name);
                
/*              if (asprintf(&cmd_play, "curl %s %s | madplay - -Q --no-tty-control &", curl_opts, station_url) != -1) {  */
                if (asprintf(&cmd_play, "curl %s %s | madplay - -Q --no-tty-control", curl_opts, station_url) != -1) {
                    system(cmd_play);
                    free(cmd_play); // Освобождаем память сразу после вызова system
                }

                usleep(2500000); 
                
                while (is_madplay_running()) {
                    usleep(500000); 
                }
                
                buff1++; 
            } 
            else {
                buff1 = 1;
                if (!get_station_by_index(tmpplay, buff1, station_name, station_url)) {
                    error_log("Playlist empty or missing.");
                    break;
                }
            }
        }
    }
}

