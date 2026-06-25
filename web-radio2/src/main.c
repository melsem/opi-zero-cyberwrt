#include <string.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <sys/socket.h>
#include <sys/wait.h> 
#include <fcntl.h>  
#include <termios.h>  
#include <stdio.h>  
#include <signal.h>  
#include <unistd.h>  
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <errno.h>
#include <linux/types.h>

#define BUFSIZE 2048

char *play_url    = NULL;
char *name_url    = NULL;
char *play_file   = NULL;
char *play_folder = NULL;
char play_list[256]; 

int buff1 = 1;   
int nameBit = 0; 

void helpComand(char *argv[]);
void play(char *tmpplay, int menbit);

int main(int argc, char *argv[]) {
    int inputs;
    int urlBit = 0, fileBit = 0, folderBit = 0, listBit = 0, helpBit = 0;

    while((inputs = getopt(argc, argv, "u:f:l:?:h:n:p:")) != -1) {
        switch(inputs) {
            case 'u': 
                play_url = strdup(optarg);
                urlBit = 1;
                listBit = 1;
                break;

            case 'f': 
                play_file = strdup(optarg);
                fileBit = 1;
                listBit = 1;
                break;

            case 'l': 
                play_folder = strdup(optarg);
                folderBit = 1;
                listBit = 1;
                break;

            case '?':
            case 'h': 
                helpBit = 1;
                listBit = 1;
                break;

            case 'n': 
                name_url = strdup(optarg);
                nameBit = 1;
                break;

            case 'p': 
                buff1 = atoi(optarg); 
                break;
        }
    }

    strncpy(play_list, "/etc/config/web-radio2", sizeof(play_list) - 1);
    play_list[sizeof(play_list) - 1] = '\0';

    if (listBit == 0) {
        play(play_list, 0);
    } 
    else if (urlBit == 1 && play_url != NULL) {
        play(play_url, 1);
        play(play_list, 0);
    } 
    else if (fileBit == 1 && play_file != NULL) {
        play(play_file, 2);
    } 
    else if (folderBit == 1 && play_folder != NULL) {
        play(play_folder, 3);
    } 
    else if (helpBit == 1) {
        helpComand(argv);
    }

    return 0;
}
