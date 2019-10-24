// ******************************************** //
void probel(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA2 + pos); write_dat(LCD_PROBELPOZYTYV);
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA4+ pos); write_dat(LCD_PROBELPOZYTYV);
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA2 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA4 + pos); write_dat(LCD_PROBELPOZYTYV);
	}
   }
}
// ******************************************** //
void simvol_Gradus(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); write_dat(0x6f);
		write_CMD(LCD_SETSTROKA2 + pos); write_dat(LCD_PROBELPOZYTYV);
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(0x6f);
		write_CMD(LCD_SETSTROKA4 + pos); write_dat(LCD_PROBELPOZYTYV);
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); write_dat(0x4f);
		write_CMD(LCD_SETSTROKA2 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA4 + pos); write_dat(LCD_PROBELPOZYTYV);
	}
   }
}
// ******************************************** //
void simvol_Minus(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); write_dat(LCD_CGRAM1);
		write_CMD(LCD_SETSTROKA2 + pos); write_dat(LCD_PROBELPOZYTYV);
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(LCD_CGRAM1);
		write_CMD(LCD_SETSTROKA4 + pos); write_dat(LCD_PROBELPOZYTYV);
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA2 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(LCD_CGRAM1);
		write_CMD(LCD_SETSTROKA4 + pos); write_dat(LCD_PROBELPOZYTYV);
	}
   }
}
// ******************************************** //
void great_0() { write_dat(LCD_CGRAM5); write_dat(LCD_CGRAM0); write_dat(LCD_CGRAM0); }
void great_1() { write_dat(LCD_CGRAM5); write_dat(LCD_CGRAM0); write_dat(LCD_CGRAM4); }
void great_2() { write_dat(LCD_CGRAM6); write_dat(LCD_CGRAM1); write_dat(LCD_CGRAM3); }
void great_3() { write_dat(LCD_PROBELPOZYTYV); write_dat(LCD_PROBELPOZYTYV); write_dat(LCD_PROBELNEGATIV); }
void great_4() { write_dat(LCD_CGRAM2); write_dat(LCD_CGRAM2); write_dat(LCD_CGRAM4); }
void great_5() { write_dat(LCD_CGRAM1); write_dat(LCD_CGRAM1); write_dat(LCD_CGRAM3); }
void great_6() { write_dat(LCD_CGRAM5); write_dat(LCD_CGRAM2); write_dat(LCD_CGRAM4); }
void great_7() { write_dat(LCD_CGRAM5); write_dat(LCD_CGRAM2); write_dat(LCD_CGRAM2); }
void great_8() { write_dat(LCD_PROBELPOZYTYV); write_dat(LCD_PROBELNEGATIV); write_dat(LCD_PROBELPOZYTYV); }
void great_9() { write_dat(LCD_PROBELNEGATIV); write_dat(LCD_PROBELPOZYTYV); write_dat(LCD_PROBELNEGATIV); }
void great_10() { write_dat(LCD_CGRAM0); write_dat(LCD_CGRAM4); write_dat(LCD_PROBELPOZYTYV); }
void great_11() { write_dat(LCD_CGRAM1); write_dat(LCD_PROBELNEGATIV); write_dat(LCD_CGRAM1); }
void great_12() { write_dat(LCD_PROBELNEGATIV); write_dat(LCD_PROBELPOZYTYV); write_dat(LCD_PROBELPOZYTYV); }
void great_13() { write_dat(LCD_CGRAM6); write_dat(LCD_CGRAM1); write_dat(LCD_PROBELNEGATIV); }
void great_14() { write_dat(LCD_PROBELNEGATIV); write_dat(LCD_CGRAM1); write_dat(LCD_CGRAM1); }
void great_15() { write_dat(LCD_PROBELPOZYTYV); write_dat(LCD_CGRAM1); write_dat(LCD_CGRAM3); }
void great_16() { write_dat(LCD_CGRAM6); write_dat(LCD_CGRAM1); write_dat(LCD_CGRAM1); }
void great_17() { write_dat(LCD_CGRAM0); write_dat(LCD_CGRAM0); write_dat(LCD_CGRAM4); }
void great_18() { write_dat(LCD_PROBELPOZYTYV); write_dat(LCD_CGRAM5); write_dat(LCD_PROBELPOZYTYV); }
// ******************************************** //

// ******************************************** //
// Вывод на LCD БОЛЬШИХ СИМВОЛОВ ДИСПЛЕЙ LCD WN2004
// использует сегменты для построения номерa 0
void simvol_0(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); great_1();
		write_CMD(LCD_SETSTROKA2 + pos); great_2();
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); great_1();
		write_CMD(LCD_SETSTROKA4 + pos); great_2();
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); great_1();
		write_CMD(LCD_SETSTROKA2 + pos); great_9();
		write_CMD(LCD_SETSTROKA3 + pos); great_9();
		write_CMD(LCD_SETSTROKA4 + pos); great_2();
	}
   }
}

// использует сегменты для построения номерa 1
void simvol_1(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); great_10();
		write_CMD(LCD_SETSTROKA2 + pos); great_11();
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); great_10();
		write_CMD(LCD_SETSTROKA4 + pos); great_11();
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); great_10();	// " "
		write_CMD(LCD_SETSTROKA2 + pos); great_8();
		write_CMD(LCD_SETSTROKA3 + pos); great_8();
		write_CMD(LCD_SETSTROKA4 + pos); great_11();
	}
   }
}

// использует сегменты для построения номерa 2
void simvol_2(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); great_4();
		write_CMD(LCD_SETSTROKA2 + pos); great_16();
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); great_4();
		write_CMD(LCD_SETSTROKA4 + pos); great_16();
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); great_1();
		write_CMD(LCD_SETSTROKA2 + pos); great_15();
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(LCD_CGRAM5); write_dat(LCD_PROBELPOZYTYV); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA4 + pos); great_14();
	}
   }
}

// использует сегменты для построения номерa 3
void simvol_3(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); great_4();
		write_CMD(LCD_SETSTROKA2 + pos); great_5();
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); great_4();
		write_CMD(LCD_SETSTROKA4 + pos); great_5();
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); great_1();
		write_CMD(LCD_SETSTROKA2 + pos); great_15();
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(LCD_PROBELPOZYTYV); write_dat(LCD_PROBELPOZYTYV); write_dat(LCD_CGRAM4);
		write_CMD(LCD_SETSTROKA4 + pos); great_2();
	}
   }
}


// использует сегменты для построения номерa 4
void simvol_4(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); great_13();
		write_CMD(LCD_SETSTROKA2 + pos); great_3();
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); great_13();
		write_CMD(LCD_SETSTROKA4 + pos); great_3();
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); great_9();
		write_CMD(LCD_SETSTROKA2 + pos); great_13();
		write_CMD(LCD_SETSTROKA3 + pos); great_3();
		write_CMD(LCD_SETSTROKA4 + pos); great_3();
	}
   }
}

// использует сегменты для построения номерa 5
void simvol_5(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); great_7();
		write_CMD(LCD_SETSTROKA2 + pos); great_5();
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); great_7();
		write_CMD(LCD_SETSTROKA4 + pos); great_5();
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); write_dat(LCD_PROBELNEGATIV); write_dat(LCD_CGRAM0); write_dat(LCD_CGRAM0);
		write_CMD(LCD_SETSTROKA2 + pos); great_14();
		write_CMD(LCD_SETSTROKA3 + pos); great_3();
		write_CMD(LCD_SETSTROKA4 + pos); great_2();
	}
   }
}

// использует сегменты для построения номерa 6
void simvol_6(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); great_7();
		write_CMD(LCD_SETSTROKA2 + pos); great_2();
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); great_7();
		write_CMD(LCD_SETSTROKA4 + pos); great_2();
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); great_1();
		write_CMD(LCD_SETSTROKA2 + pos); great_12();
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(LCD_PROBELNEGATIV); write_dat(LCD_CGRAM0); write_dat(LCD_CGRAM4);
		write_CMD(LCD_SETSTROKA4 + pos); great_2();
	}
   }
}

// использует сегменты для построения номерa 7
void simvol_7(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); great_17();
		write_CMD(LCD_SETSTROKA2 + pos); great_18();
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); great_17();
		write_CMD(LCD_SETSTROKA4 + pos); great_18();
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); write_dat(LCD_PROBELNEGATIV); write_dat(LCD_CGRAM0); write_dat(LCD_PROBELNEGATIV);
		write_CMD(LCD_SETSTROKA2 + pos); great_3();
		write_CMD(LCD_SETSTROKA3 + pos); great_3();
		write_CMD(LCD_SETSTROKA4 + pos); great_3();
	}
   }
}

// использует сегменты для построения номерa 8
void simvol_8(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); great_6();
		write_CMD(LCD_SETSTROKA2 + pos); great_2();
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); great_6();
		write_CMD(LCD_SETSTROKA4 + pos); great_2();
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); great_1();
		write_CMD(LCD_SETSTROKA2 + pos); great_2();
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(LCD_CGRAM5); write_dat(LCD_PROBELPOZYTYV); write_dat(LCD_CGRAM4);
		write_CMD(LCD_SETSTROKA4 + pos); great_2();
	}
   }
}

// использует сегменты для построения номерa 9
void simvol_9(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); great_6();
		write_CMD(LCD_SETSTROKA2 + pos); great_5();
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); great_6();
		write_CMD(LCD_SETSTROKA4 + pos); great_5();
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); great_1();
		write_CMD(LCD_SETSTROKA2 + pos); great_13();
		write_CMD(LCD_SETSTROKA3 + pos); great_3();
		write_CMD(LCD_SETSTROKA4 + pos); great_2();
	}
   }

}
// ******************************************** //
// ******************************************** //
void simvol_C(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); great_0();
		write_CMD(LCD_SETSTROKA2 + pos); great_16();
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); great_0();
		write_CMD(LCD_SETSTROKA4 + pos); great_16();
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); great_1();
		write_CMD(LCD_SETSTROKA2 + pos); great_12();
		write_CMD(LCD_SETSTROKA3 + pos); great_12();
		write_CMD(LCD_SETSTROKA4 + pos); great_2();
	}
   }
}
// ******************************************** //

// ******************************************** //
void simvol_Tocka(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA2 + pos); write_dat(LCD_CGRAM7);	// TOCKA
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA4 + pos); write_dat(LCD_CGRAM7);	// TOCKA
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA2 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA4 + pos); write_dat(LCD_CGRAM7);	// TOCKA
	}
   }
}
// ******************************************** //
void simvol_2Tocka(int pos) {
   if (lcd_l == 1) {
		// символ на две строки
	if (wide_simvol == '1') {
		write_CMD(LCD_SETSTROKA1 + pos); write_dat(LCD_CGRAM7);
		write_CMD(LCD_SETSTROKA2 + pos); write_dat(LCD_CGRAM7);	// TOCKA
	} else if (wide_simvol == '2') {
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(LCD_CGRAM7);
		write_CMD(LCD_SETSTROKA4 + pos); write_dat(LCD_CGRAM7);	// TOCKA
	} else if ((wide_simvol == '3') || (wide_simvol == '4')) { // символ на четыре строки
		write_CMD(LCD_SETSTROKA1 + pos); write_dat(LCD_PROBELPOZYTYV);
		write_CMD(LCD_SETSTROKA2 + pos); write_dat(LCD_CGRAM7);	// TOCKA
		write_CMD(LCD_SETSTROKA3 + pos); write_dat(LCD_CGRAM7);	// TOCKA
		write_CMD(LCD_SETSTROKA4 + pos); write_dat(LCD_PROBELPOZYTYV);
	}
   }
}
// ******************************************** //

// ******************************************** //
void printDigits(char digits, int pos){
  // utility function for digital clock display: prints preceding colon and leading 0
  switch (digits) {
   case '0':  
    simvol_0(pos);
   break;
   case '1':  
    simvol_1(pos);
   break;
   case '2':  
    simvol_2(pos);
   break;
   case '3':  
    simvol_3(pos);
   break;
   case '4':  
    simvol_4(pos);
   break;
   case '5':  
    simvol_5(pos);
   break;
   case '6':  
    simvol_6(pos);
   break;
   case '7':  
    simvol_7(pos);
   break;
   case '8':  
    simvol_8(pos);
   break;
   case '9':  
    simvol_9(pos);
   break;

   case '.':  
    simvol_Tocka(pos);
   break;

   case ':':  
	if (lcd_number == 16) {pos-=1; pos_right_shift-=1;}	// lcd 1602
	if (blink_bit == 0) { simvol_2Tocka(pos); blink_bit = 1; }
	else if (blink_bit == 1) { probel(pos); blink_bit = 0; }
// simvol_2Tocka(pos);
   break;

   case ' ':  
    probel(pos);
   break;

   case 'o':  
    simvol_Gradus(pos);
   break;

   case 'O':  
    simvol_Gradus(pos);
   break;

   case 'C':  
    simvol_C(pos);
   break;

   case '-':  
    simvol_Minus(pos);
   break;
  }
}
// ******************************************** //

