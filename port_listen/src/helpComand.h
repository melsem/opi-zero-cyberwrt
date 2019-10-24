
void helpComand(char *argv[]) {
   time_t t;
   time(&t);
   printf("  ________________________________________\n");
   printf("||      port_listen  HELP:                ||\n");
   printf("||       -d <deice port>                  ||\n");
   printf("||       -b <baud rate>                   ||\n");
   printf("||       -f <file_ardu>                   ||\n");
   printf("||     Not arguments - setup DEFOLT:      ||\n");
   printf("||    -d USB0 -b 115200 -f tmp.txt        ||\n");
   printf("||         Example command line:          ||\n");
   printf("|| %s -d USB0 -b 57600 -f tmp.txt||\n", argv[0]);
   printf("||________________________________________||\n");
   printf("        %s", ctime( &t));
   printf("       Error-Not argumets! Read HELP\n\n");
   error_log("Error-Not argumets! Read HELP");
}

