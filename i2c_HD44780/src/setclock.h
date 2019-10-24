


// ******************************************** //
// Print output string
void stringout_lcd_WideClock(char * c, int a, int b) {
	int i; pos_right_shift=0;			// lcd 1602
	if ((lcd_number == 20) && (a != 0)) { probel(pos_right_shift); pos_right_shift=1; }	// lcd 200x
	for (i=a; i<b; i++) {
		if (bufclock[i] != c[i]) {
			printDigits (c[i], pos_right_shift);
//			bufclock[i] = c[i];
		} else if (c[i] == ':') printDigits (c[i], pos_right_shift);
		int z = 1;
		if ((c[i] >= '0') && (c[i] <= '9')) z = 3;
		   pos_right_shift += z;
//		if (bufclock[i] != c[i]) probel(pos_right_shift);
  probel(pos_right_shift);
		   pos_right_shift += 1;
  bufclock[i] = c[i];
	}
  probel(pos_right_shift);
 if (lcd_number == 16) { pos_right_shift += 1; probel(pos_right_shift); pos_right_shift += 1; probel(pos_right_shift); }
}
// ******************************************** //


// ******************************************** //
// Show the clock on the LCD
void setclock(void) {
	int i;
	time_t secs = time(0);
	struct tm *local = localtime(&secs);
	char strclock[17];
	memset(bufclock, 0, sizeof(bufclock)); //очищаем
	memset(strclock, 0, sizeof(strclock)); //очищаем
	const char *strmonth[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	// Convert time to string and send it to the LCD
	sprintf(strclock, "%02d %s  %02d:%02d:%02d", local->tm_mday, strmonth[local->tm_mon], local->tm_hour, local->tm_min, local->tm_sec);
//	sprintf(strclock, "%02d:%02d", local->tm_hour, local->tm_min); // stringout_lcd_WideClock(strclock,0,7); 

	if ((wide_simvol == '1') || (wide_simvol == '3')) stringout_lcd_WideClock(strclock,8,13);
	else if ((wide_simvol == '2') || (wide_simvol == '4')) {
		stringout_lcd_WideClock(strclock,8,13);
		blink_bit = 0;
		for (i=0; i<20; i++) { stringout_lcd_WideClock(bufclock,8,13); usleep(500000); }
	}
	else if (wide_simvol == '0') stringout_lcd_Wide0(strclock);
}
// ******************************************** //


