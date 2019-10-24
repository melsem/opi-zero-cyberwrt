
// ***************************************************** //
void write_cgram(char c, const char *d) {
	int i;
	c &= 0x7; // we only have 8 c 0-7
	write_CMD(LCD_SETCGRAMADDR | (c << 3)); usleep(50);
	for (i = 0; i < 8; i++) { write_dat(d[i]); usleep(50); }
}
// ***************************************************** //
void lcd_init_CGRAM() { // сегменты для построения чисел и символов B CGRAM
char	Segm0[8] = { 0B11111, 0B11111, 0B00000, 0B00000, 0B00000, 0B00000, 0B00000, 0B00000 },
	Segm1[8] = { 0B00000, 0B00000, 0B00000, 0B00000, 0B00000, 0B00000, 0B11111, 0B11111 },
	Segm2[8] = { 0B11111, 0B11111, 0B00000, 0B00000, 0B00000, 0B00000, 0B11111, 0B11111 },
	Segm3[8] = { 0B11111, 0B11111, 0B11111, 0B11111, 0B11111, 0B11111, 0B11110, 0B11100 },
	Segm4[8] = { 0B11100, 0B11110, 0B11111, 0B11111, 0B11111, 0B11111, 0B11111, 0B11111 },
	Segm5[8] = { 0B00111, 0B01111, 0B11111, 0B11111, 0B11111, 0B11111, 0B11111, 0B11111 },
	Segm6[8] = { 0B11111, 0B11111, 0B11111, 0B11111, 0B11111, 0B11111, 0B01111, 0B00111 },
	Segm7[8] = { 0B00000, 0B00000, 0B00000, 0B00000, 0B00000, 0B00111, 0B00111, 0B00111 };

	write_cgram(LCD_CGRAM0, Segm0);
	write_cgram(LCD_CGRAM1, Segm1);
	write_cgram(LCD_CGRAM2, Segm2);
	write_cgram(LCD_CGRAM3, Segm3);
	write_cgram(LCD_CGRAM4, Segm4);
	write_cgram(LCD_CGRAM5, Segm5);
	write_cgram(LCD_CGRAM6, Segm6);
	write_cgram(LCD_CGRAM7, Segm7);  // ТОЧКА
}
// ***************************************************** //



