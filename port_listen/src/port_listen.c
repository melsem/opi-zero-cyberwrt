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


//////////////////////////////

#define BUFSIZE 2048

char bRead[BUFSIZE] = {0,}, 
     str_iz_file[BUFSIZE] = {0,},
     com_Port[10], 
     folderFile_ardu[128],
     send_out_port[128],
     konf_file_path[256],

     *file_konf="port_listen",
     *device,
     *file_ardu;
unsigned long int speedBaudRate = 115200;	// ****** Default - 115200 ** //

int fd,
    tfail = 0,
    byte_w_d_s=0, hortab=0;

#define byt_Mac_wr	B00000001	// 1
#define byt_Alarm_wr	B00000010	// 
#define byt_Typ_wr	B00000100	// 
#define byt_LCD_wr	B00001000	// 
#define byt_speed_wr	B00010000	// 

// *************************************************** //
 
#include "edit_file.h"
#include "error_log.h"
#include "helpComand.h"
#include "portRead.h"
#include "portSet.h"

#include "read_conf.h"

// *************************************************** //

// *************************************************** //
int main(int argc, char *argv[]) {

    int zapusk, ic=0, dv=0, spbr=0, filar=0, flkf=0;
		// ****************** input arguments ********************* //
	while((zapusk = getopt(argc, argv, "a:d:b:f:c:?:")) != -1) {
		switch(zapusk) {
			case 'a':
				hortab=1;
			break;

			case 'd': /* Порт роутера к которому подключен Arduino - USB, ACM, ATH, S */
				device = strdup(optarg); // http://www.c-cpp.ru/content/strdup-fstrdup
				dv=1;
			break;

			case 'b': // преобразовать введенное строковое число в unsigned long
				speedBaudRate = atoi(optarg); /* скорость порта роутера */

				spbr=1;
			break;

			case 'f': /* TEMP-фаил куда пишет прога прочитанные данные с порта */
				file_ardu = strdup(optarg); // http://www.c-cpp.ru/content/strdup-fstrdup
				filar=1;
			break;

			case 'c': /* фаил конфигурации для проги */
				file_konf = strdup(optarg); // http://www.c-cpp.ru/content/strdup-fstrdup
				flkf=1;
			break;

			case '?':
				ic=1;
			break;

		}
	}

   if (ic==1) helpComand(argv);
   sprintf(konf_file_path, "/etc/config/%s", file_konf);		// полный путь к konfig-фаилу //

   if (spbr==0) read_conf(0);
   if (filar==0) read_conf(1);
   if (dv==0) read_conf(2);

   sprintf(com_Port, "/dev/tty%s", device);			// полный путь к порту //

   sprintf(folderFile_ardu, "/tmp/%s", file_ardu);		// полный путь к TEMP-фаилу //

   open_port();   // * init_port * //
   sleep(2);
   tcflush(fd, TCIFLUSH);

//   usleep(500000);

    while(1) {
	 portRead();
    }

    return (0);

}  // END main()
// *************************************************** //



