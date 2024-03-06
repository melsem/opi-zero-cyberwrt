

// ******************************************** //
// Print output string
void stringout_lcd_WideW1(char *c, int a, int b) {
//	char c[6]={'-', '2', '6', '.', '8', 'C'};		// Test!!!
	int i; pos_right_shift=0;
//	if (c[a] == '-') { simvol_Minus(pos_right_shift); pos_right_shift+=1; probel(pos_right_shift); pos_right_shift+=1; a=1;
//		if (sizeof(c) < 5) { probel(pos_right_shift); pos_right_shift+=1; } }	// lcd 200x
//	else if ((lcd_number == 20) && (c[a] != '-')) { probel(pos_right_shift); pos_right_shift+=1; }	// lcd 200x	// lcd 200x
	if (sizeof(c) < 4) { probel(pos_right_shift); pos_right_shift+=1; }
	for (i=a; i<b; i++) {
//	for (i=a; i<sizeof(c); i++) {			// Test!!!
	    if (c[i] != ' ') {
		if (c[i] == '.') printDigits (c[i], pos_right_shift -= 1);
		else printDigits (c[i], pos_right_shift);

		int z = 1;
		if ((c[i] >= '0') && (c[i] <= '9')) z=3;

		if (c[i] != 'C') {
			pos_right_shift += z;
			probel(pos_right_shift);
			pos_right_shift += 1;
		}
	    }
	}
}
// ******************************************** //
