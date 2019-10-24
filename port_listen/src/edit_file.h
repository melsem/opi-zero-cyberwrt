void edit_file() {
  FILE *f;

  if((f = fopen(folderFile_ardu, "w")) == NULL) {
	printf("Error-Write to file\n");
	error_log("Error-Write to file");
  } else {
	fprintf(f, "%s", bRead);	// ***  write file   *** //
	fclose(f);
//	system(konf_file_path);
//	printf("%s\n", bRead);		// *** write konsol *** //
//	memcpy(str_iz_file, bRead, sizeof(str_iz_file)); 
  }
}

