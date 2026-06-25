#include <stdio.h>
#include <time.h>
#include "error_log.h"

void error_log(const char *err) {  
   time_t t;
   time(&t);
   FILE *f = fopen("/tmp/Error_play_list", "a"); 
   if (f) {
       fprintf(f, "%s. == %s", err, ctime(&t));  
       fclose(f);
   }
}
