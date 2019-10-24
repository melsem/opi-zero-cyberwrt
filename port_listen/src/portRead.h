

void portRead() {

 int bytes = 0;

	if((bytes = read(fd, bRead, BUFSIZE))==-1) {
		printf("Error_Read_from_Arduino\n");
		error_log("Error_Read_from_Arduino");
	} else {	 // read-usb
		unsigned int i;
		for(i = 0; i <= strlen(bRead); i++) {
			if(bRead[i] == '\n') { // 0x0B - символ gorizont tab
				bRead[i] = 0; 
				break;
			}
			if((hortab == 0) && (bRead[i] == '\t')) bRead[i] = '\n';
		}
	   edit_file();
	}
}




