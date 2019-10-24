

// *************************************************** //
void error_log(char *err) 
 {  
   time_t t;
   time(&t);
   FILE *f;
   f = fopen("/tmp/Error_log_port_listen", "a"); 
   fprintf(f, "%s. ", err);
   fprintf(f, "== %s", ctime( &t));  
   printf("Write to Error_log_port_listen\n");
   fclose(f);
   exit(0);
 }
// *************************************************** //

