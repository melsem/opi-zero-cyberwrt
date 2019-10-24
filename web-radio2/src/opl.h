
// ******************************************** //
// РАБОТА С ФАЙЛОМ. ОТКРЫТИЕ - ЧТЕНИЕ ВО ВРЕМЕННЫЙ "tempraw" РЕГ. - ЗАКРЫТИЕ
void open_play_list (char *play_list, int tfail) {

  FILE *tempfile;
  char	*tempbuf;
  int	size;

	// fopen() открывает файл, имя которого указано аргументом konf_file_path, и  возвращает связанный с ним указатель. 
	if ((tempfile = fopen(play_list, "rd")) == NULL)
//	if ((tempfile = fopen("/etc/config/web-radio2", "rd")) == NULL)
	{
//		tfail = 1; // флаг отсутствия файла (Error Code #1)
		if (tfail == 0) {
			printf ("Error read play_list\n");
			error_log ("Error read play_list");
		}
		
			blop=1;
	} else if (tfail == 0) {
//		tfail = 0; // флаг присутствия файла
//		printf ("open_play_list - %s\n", play_list);
		
		// Get content of file to variable
	  fseek (tempfile, 0, SEEK_END);	/* fseek() устанавливает указатель положения в файле, связанном со 
					 stream, в соответствии со значениями offset и origin. */
	  size = ftell (tempfile);	/* fftell определяет текущую позицию в потоке данных, на  который 
					 указывает аргумент tempfile.  */
	  rewind (tempfile);	 /* rewind()  перемещает указатель положения в файле на начало указанного потока. */
	  tempbuf = calloc (size + 1, 1); /* calloc выделяет только указанный массив и плюс к этому обнуляет все внутри него. */
	  fread (tempbuf, 1, size, tempfile); /* fread считывает массив размером — size элементов,  каждый из
						  которых имеет размер size байт, из потока, */

		// Close file stream
	  fclose (tempfile);	 /* fclose закрывает и разъединяет файл tempfile ,  связанный с потоком.  */
	  strcpy (tempraw, tempbuf); /* strcpy(str1, str2) используется для копирования содержимого str2 в str1.  Аргумент str2
					 должен быть указателем на строку, оканчивающуюся нулем. */
	  free(tempbuf); /* free() возвращает память, на которую указывает параметр tempbuf, назад  в кучу. В результате
					 эта память может выделяться снова */
	}
}
// ******************************************** //


