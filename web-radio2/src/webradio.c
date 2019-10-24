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
//////////////////////

#include <asm/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <errno.h>
//#include <termio.h>
#include <linux/types.h>

#define BUFSIZE 2048

char *play_url,
     *name_url,
     *play_file,
     play_list[24],
     *play_folder,
     tempraw[BUFSIZE]; // сюда скoпируется play_list web-radio2 для дальнейшей обработки //

int blop, buff1=1, nameBit=0;
// *************************************************** //
#include "error_log.h"
#include "helpComand.h"
#include "opl.h"
#include "play.h"

//#include "edit_file.h"
//#include "portRead.h"
//#include "portSet.h"
//#include "read_conf.h"

// *************************************************** //
int main(int argc, char *argv[]) {

    int inputs, urlBit=0, fileBit=0, folderBit=0, listBit=0, helpBit=0, bf=0;
		// ****************** input arguments ********************* //
	while((inputs = getopt(argc, argv, "u:f:l:?:h:n:p:")) != -1) {
		switch(inputs) {
			case 'u': /* url */
				play_url = strdup(optarg); // http://www.c-cpp.ru/content/strdup-fstrdup
				urlBit=1;
				listBit=1;
			break;

			case 'f': /* file */
				play_file = strdup(optarg); // http://www.c-cpp.ru/content/strdup-fstrdup
				fileBit=1;
				listBit=1;
			break;

			case 'l': /* folder */
				play_folder = strdup(optarg); // http://www.c-cpp.ru/content/strdup-fstrdup
				folderBit=1;
				listBit=1;
			break;

			case '?':
				helpBit=1;
				listBit=1;
			break;

			case 'h':
				helpBit=1;
				listBit=1;
			break;

			case 'n':
				name_url = strdup(optarg); // http://www.c-cpp.ru/content/strdup-fstrdup
				nameBit=1;
			break;

			case 'p':
				buff1+=atoi(optarg); bf=1;	 // http://www.c-cpp.ru/content/atoi
			break;
		}
	}
//	printf("buff1 - d: %d\n", buff1);
/*
	Аргументы можно менять местами.
web-radio2 -h								# 
web-radio2 -?								# 
web-radio2								# воспроизводит URL c плей-листa
web-radio2 -l /home							# воспроизводит папку с аудио
web-radio2 -f /home/085.BANYA.mp3					# воспроизводит аудио-фаил
web-radio2 -u http://chanson.hostingradio.ru:8041/chanson256.mp3	# воспроизводит URL
web-radio2 -u http://chanson.hostingradio.ru:8041/chanson256.mp3 -n chanson256	# воспроизводит URL+ имя
web-radio2 -u http://chanson.hostingradio.ru:8041/chanson256.mp3 -n chanson256 -p 1	# воспроизводит URL+ имя + порядковый
*/
	sprintf (play_list, "/etc/config/web-radio2");

	if (listBit==0) {
		if (bf==1) buff1-=1; play (play_list, 0);
	} else if ((urlBit==1) && (play_url != 0)) {
		play (play_url, 1);
		play (play_list, 0);
	} else if ((fileBit==1) && (play_file != 0)) {
		play (play_file, 2);
	} else if ((folderBit==1) && (play_folder != 0)) {
		play (play_folder, 3);
	} else if (helpBit==1) helpComand(argv);

    return (0);

}  // END main()
// *************************************************** //





