

   char tempraw[3200];

void read_conf(int rf) {
      open_file();
  int buff = strlen(tempraw);
   if (tfail != 1) {
		//	printf("tempraw: %s\n", tempraw);
		//	printf("buff: %d\n", buff);
     char *sp="t_file";
     int i, tst;

     if (rf==0) sp="speed";		/* speed */
     else if (rf==1) sp="t_file";	/* t_file */
     else if (rf==2) sp="devPort";	/* devPort */
     tst = strlen(sp);
// printf("-- strlen(sp) --: %d\n", tst);
// printf("-- SP ??? --: %s\n", sp);

	for (i=0; i<buff; i++) {
		int st=0, ii=i;
		while ((tempraw[ii] == sp[st]) && (st < tst)) {
			 st+=1; ii+=1; 
		}

		if (st == tst) { /* READ conf_file: speed - t_file - devPort */

			while ((tempraw[ii] == ' ') || (tempraw[ii] == 0x27)) ii++;

			    i=ii;
			    char c=0;
			while (isalnum(tempraw[ii])) {
				ii++;
				c++;
			}
			    char otttp[c];
			    ii=i;
			if (c<2) goto raz;
			    c=0;
			while (isalnum(tempraw[ii])) {
				otttp[c]=tempraw[ii];
				ii++;
				c++;
				if (rf!=0) otttp[c]=0;
			}
			goto dva;
raz:		  	if (rf==1) file_ardu="arduino";		 // ****** Default filename ****** //
			else if (rf==2) device="USB0";		 // ****** Default Порт ********** //
			goto tri;

dva:			if (rf==0) speedBaudRate=atoi(otttp);	 // http://www.c-cpp.ru/content/atoi
			else if (rf==1) file_ardu=strdup(otttp); // http://www.c-cpp.ru/content/strdup-fstrdup
			else if (rf==2) device=strdup(otttp);    // http://www.c-cpp.ru/content/strdup-fstrdup

tri:			i=buff; /* end for */ 
		}
	}
   usleep(2);
   }
}
// ******************************************** //

void open_file(void) {

  FILE *tempfile;
  char	*tempbuf;
  int	size;

	// fopen() открывает файл, имя которого указано аргументом konf_file_path, и  возвращает связанный с ним указатель. 
	if ((tempfile = fopen(konf_file_path, "rd")) == NULL)
//	if ((tempfile = fopen("/etc/config/port_listen", "rd")) == NULL)
	{
		tfail = 1; // флаг отсутствия файла (Error Code #1)
		printf("Error read file\n");
		error_log("Error read file");


	} else {
			tfail = 0; // флаг присутствия файла
		
		// Get content of file to variable
	  fseek(tempfile, 0, SEEK_END);	/* fseek() устанавливает указатель положения в файле, связанном со 
					 stream, в соответствии со значениями offset и origin. */
	  size = ftell(tempfile);	/* fftell определяет текущую позицию в потоке данных, на  который 
					 указывает аргумент tempfile.  */
	  rewind(tempfile);	 /* rewind()  перемещает указатель положения в файле на начало указанного потока. */
	  tempbuf = calloc(size + 1, 1); /* calloc выделяет только указанный массив и плюс к этому обнуляет все внутри него. */
	  fread(tempbuf, 1, size, tempfile); /* fread считывает массив размером — size элементов,  каждый из
						  которых имеет размер size байт, из потока, */

		// Close file stream
	  fclose(tempfile);	 /* fclose закрывает и разъединяет файл tempfile ,  связанный с потоком.  */
	  strcpy(tempraw, tempbuf); /* strcpy(str1, str2) используется для копирования содержимого str2 в str1.  Аргумент str2
					 должен быть указателем на строку, оканчивающуюся нулем. */
	  free(tempbuf); /* free() возвращает память, на которую указывает параметр tempbuf, назад  в кучу. В результате
					 эта память может выделяться снова */
	}
}
