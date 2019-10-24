

// *************************************************** //
void error_log(char *err) 
 {  
   time_t t;
   time(&t);
   FILE *f;
   f = fopen("/tmp/Error_play_list", "a"); 
   fprintf(f, "%s. ", err);
   fprintf(f, "== %s", ctime( &t));  
   printf("Write to Error_play_list\n");
   fclose(f);
   exit(0);
 }
// *************************************************** //

