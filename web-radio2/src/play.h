
void kiledd ()
{
   system ("killall -9 curl&> /dev/null &> /dev/null");
   system ("killall -9 wget &> /dev/null &> /dev/null");
   system ("killall -9 madplay &> /dev/null &> /dev/null");
}

void play (char *tmpplay, int menbit)
{
   char cmd_play[BUFSIZE];
   kiledd ();

	if (menbit==1) { /* воспроизвести только один url-адрес станции */
		   if (nameBit==0) sprintf (cmd_play, "printf %s > /tmp/play_name", tmpplay);
		   else if (nameBit==1) sprintf (cmd_play, "printf %s > /tmp/play_name", name_url);
		   system (cmd_play); // отправить url в /tmp/фаил -- printf "$url_play" > /tmp/play_name
		   sprintf (cmd_play, "curl --silent %s | madplay - -Q --no-tty-control", tmpplay);
		   system (cmd_play);
	} else if (menbit==2) { /* воспроизвести только один аудио-фаил */
		   sprintf (cmd_play, "printf %s > /tmp/play_name", tmpplay); 
		   system (cmd_play);  // отправить file_play в /tmp/фаил -- printf "$file_play" > /tmp/play_name
		   sprintf (cmd_play, "cat %s | madplay - -Q --no-tty-control &", tmpplay);
		   system (cmd_play);
	} else if (menbit==3) { /* воспроизвести все аудио-фаилы в папке */
		   sprintf (cmd_play, "printf %s/*.mp3 > /tmp/play_name", tmpplay); 
		   system (cmd_play);  // отправить file_play в /tmp/фаил -- printf "$file_play" > /tmp/play_name
		   sprintf (cmd_play, "madplay - -Q --no-tty-control %s/*.mp3 &", tmpplay);
		   system (cmd_play);
	} 

	if (menbit==0) { /* воспроизвести по одному url-адресу из play_list */

	   sprintf (cmd_play, "c=$(sed -n '$=' %s); let 'c /= 2'; printf $c > /tmp/nl", tmpplay);
	   system (cmd_play);
	   sprintf (cmd_play, "/tmp/nl");
	   open_play_list (cmd_play, 0); //колличество строк в char tempraw 

		int buff = atoi(tempraw), // кoвертировали колличество строк char в int
		    ii;

	    while (1) {
		kiledd ();
		   usleep(500);
		   sprintf (cmd_play, "cat %s | awk -F , '/#EXTINF:'-%d',/ {print $2}' > /tmp/play_name", tmpplay, buff1);
		   system (cmd_play); // отправить название в /tmp/фаил
		   usleep(500);
		   sprintf (cmd_play, "url=$(sed -n '/#EXTINF:'-%d'/{n;p;q;}' %s); printf $url > /tmp/nl", buff1, tmpplay);
		   system (cmd_play); // отправить url в /tmp/фаил
		   sprintf (cmd_play, "/tmp/nl");

		   open_play_list (cmd_play, 0); //READ url iz /tmp/фаил в tempraw
		   sprintf (cmd_play, "curl --silent %s | madplay - -Q --no-tty-control &", tempraw);
		   system (cmd_play);
	           usleep(1000000);

			blop=1;
		    while (blop == 1) {
			blop=0;
			sprintf (cmd_play, "/tmp/nl2");
			remove(cmd_play);
			usleep(500000);
			sprintf (cmd_play, "t=`pidof madplay`; if [ -z $t ]; then printf 0 > /tmp/nl2; fi &"); 
			system (cmd_play); // отправить pidof в /tmp/фаил ili net
			usleep(500);
			sprintf (cmd_play, "/tmp/nl2");
			open_play_list (cmd_play, 2);
		    }
			usleep(500);
			
		    if (buff1<=buff) { buff1++; }
		    else buff1=1;
		}
	}
}

