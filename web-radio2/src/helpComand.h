
void helpComand(char *argv[]) {
   time_t t;
   time(&t);
   printf("  ____________________________________________________\n");
   printf("||          -u - URL                                  ||\n");
   printf("||          -n - Name                                 ||\n");
   printf("||          -p - №                                    ||\n");
   printf("||          -f - /Folder/audio/file.mp3               ||\n");
   printf("||          -l - Folder audio                         ||\n");
   printf("||          -? - help mini                            ||\n");
   printf("||          -h - help mini                            ||\n\n");
   printf("||_____________   Example:   _________________________||\n");
   printf("            Read play-list\n");
   printf("%s\n", argv[0]);
   printf("            play - URL\n");
   printf("%s -u http://chanson.hostingradio.ru:8041/chanson-uncensored128.mp3\n", argv[0]);
   printf("            play - URL + Name\n");
   printf("%s -u http://chanson.hostingradio.ru:8041/chanson-uncensored128.mp3 -n Chanson-uncensored\n", argv[0]);
   printf("            play - URL + Name + №\n");
   printf("%s -u http://chanson.hostingradio.ru:8041/chanson-uncensored128.mp3 -n Chanson-uncensored -p 2\n", argv[0]);
   printf("            play - FILE\n");
   printf("%s -f /home/085.BANYA.mp3\n", argv[0]);
   printf("            play - FOLDER\n");
   printf("%s -l /home\n", argv[0]);
   printf("||____________________________________________________||\n\n");
   printf("        %s\n", ctime( &t));
}

