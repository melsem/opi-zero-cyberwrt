 /*
 * LCD monitor for HD44780 over I2C by PCF8574 bus expander.
 * W1-gpio thermostat-thermometer 4-sensors or digitemp-usb thermometer.
 * help(RU) http://www.cyber-place.ru/showthread.php?t=2164
 * Pins of PCF8574:   P7  P6  P5  P4  P3   P2  P1  P0
 * Pins of HD44780:   D7  D6  D5  D4  LED  E   RW  RS
 */

#include <asm/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <linux/i2c-dev.h>
#include <linux/types.h>

void lcd_init(void);
void write_CMD(char);
void write_dat(char);
void stringout_lcd_Wide0(char*);
void digitalWrite_i2c(char); 
void setclock(void);
void readd_content_file(void);
void read_ffile(void);

void print_lcd(void);

/* commands */
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02

#define LCD_SETSTROKA1 0x80	// курсор в начало первой строки
#define LCD_SETSTROKA2 0xC0	// курсор в начало второй строки
#define LCD_SETSTROKA3 0x94	// курсор в начало третьей строки
#define LCD_SETSTROKA4 0xD4	// курсор в начало четвертой строки

#define LCD_SETCGRAMADDR 0x40	// Load user-defined symbol into the CGRAM

#define En 0B00000100  // Enable bit
//#define Rw 0B00000010  // Read bit/
#define Rw 0B00000000 // Write bit
#define Rs 0B00000001  // Register select bit

#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08

#define LCD_SENIORCMD 0x04
#define LCD_JUNIORCMD 0x00

#define LCD_SENIORDATA 0x05
#define LCD_JUNIORDATA 0x01

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

/* end commands */
#define LCD_PROBELPOZYTYV 0B00100000
#define LCD_PROBELNEGATIV 0B11111111
#define LCD_CGRAM0 0
#define LCD_CGRAM1 1
#define LCD_CGRAM2 2
#define LCD_CGRAM3 3
#define LCD_CGRAM4 4
#define LCD_CGRAM5 5
#define LCD_CGRAM6 6
#define LCD_CGRAM7 7

int	errFl=0,
	prmb=0,
	file,
	stat_data_termo,
	tfail = 0,
	kollw_sensor = 0,
	ds_id = 0,
	fl_es = 0,
	colo = 0x00,
	led = LCD_NOBACKLIGHT,
//	rw = 0x02,
	iia = 0,
	ds_nik = 0,
	gpioPin_est = 0,
	i_nick = 0,
	alarm1=0,
	histe1=0,
	invers1=0,
	alarm2=0,
	histe2=0,
	invers2=0,
	alarm3=0,
	histe3=0,
	invers3=0,
	alarm4=0,
	histe4=0,
	invers4=0,
	nGp1_out=68,
	nGp2_out=68,
	nGp3_out=68,
	nGp4_out=68,
  lcd_number=16,
  lcd_n = 0,

  pos_right_shift=0,
  lcd_l = 0,
  blink_bit=0,

	// **************************************************************************************************************** //
	/*  Входной параметр wide_simvol = 0 - это символ на две строки двух-строчного LCD
	    Входной параметр wide_simvol = 2 - это символ на две строки четырех-строчного LCD, на первой и второй строках.
	    Входной параметр wide_simvol = 3 - это символ на две строки четырех-строчного LCD, на третья и четвертая строках.
	    Входной параметр wide_simvol = 4 - это символ на четыре строки четырех-строчного LCD.	*/  
	// **************************************************************************************************************** //
  wide_simvol = '0';	// wide_simvol == 52 - <<если char strdup(optarg) будет 4, то int будет 52 >>
	

char
	read_file_ds18_path[256],
	dev_path[256],
	tempraw[256],
	setlcd_patch[20],
	set_ch[20],
	ds0_path[15],
	ds1_path[15],
	ds2_path[15],
	ds3_path[15],
	*w1_path,
	*ds18b20_sensor,
	*nickname_sensor,
	*stryng_thermoAllarm,
	temp_sensor_data[7],
	temp_sensor_txt[10],

	temp_gpOut[256],
	tst_gpOut[3],
	temp_str_gpOut[256],
	pin_gpOut[2],
	stroka1[21],
	stroka2[21],
	stroka3[21],
	stroka4[21],
  bufclock[17],
  c_clock[8];

FILE *gpioTmp;


#define BIT_WHILE setlcd_patch[0]-48
#define BIT_CLOCK setlcd_patch[1]-48
#define BIT_LED setlcd_patch[2]-48

#define BIT_STRING setlcd_patch[3]-48

#define BIT_one_w1 setlcd_patch[4]-48	/* "1" - w1 */
#define BIT_digitemp setlcd_patch[4]-48	/* "2" - digitemp  */

#define bit_typ1 setlcd_patch[5]-48	/* sensor1 "0"-"1"-"2" - TIP dev */
#define bit_typ2 setlcd_patch[6]-48	/* sensor2 "0"-"1"-"2" - TIP dev */
#define bit_typ3 setlcd_patch[7]-48	/* sensor3 "0"-"1"-"2" - TIP dev */
#define bit_typ4 setlcd_patch[8]-48	/* sensor4 "0"-"1"-"2" - TIP dev */


// ******************************************** //

#include "initSegment.h"
#include "printDigits.h"
#include "stringout_lcd_WideW1.h"
#include "setclock.h"


// *********************************************************** //
void error_log(char *err) {  
   time_t t;
   time(&t);
   FILE *f;
   f = fopen("/tmp/Err_i2c_hd44780.log", "w"); 
   fprintf(f, "%s. ", err);
   fprintf(f, "==  %s", ctime( &t));  
   printf("Err_i2c_hd44780.log\n");
   fclose(f);
  if (errFl != 0) {
//	system("killall w1_dem");
	system("killall digitemp_dem");
	exit(EXIT_FAILURE);
  }
}
// *********************************************************** //


// *********************************************************** //
void add_tempFile() {
   FILE *f;
	f = fopen(read_file_ds18_path, "r"); 
	if(f == NULL) 
	{
		f = fopen(read_file_ds18_path, "w"); 
		printf("Error. Add nev file\n");
	}
	fclose(f);
}
// *********************************************************** //


// *********************************************************** //
// ********************  Вариант №1  ************************* //
// ****** ADD pin out через bash - системными командами ****** //
// *********************************************************** //
void add_gpioOut() {	//	init add power gpio-out	//

      if ((bit_typ1 != 0) && (nGp1_out)) {
	sprintf(temp_gpOut, "echo %d > /sys/class/gpio/export", nGp1_out);
	system(temp_gpOut);
	sprintf(temp_gpOut, "echo out > /sys/class/gpio/gpio%d/direction", nGp1_out);
	system(temp_gpOut);
      }

      if ((bit_typ2 != 0) && (nGp2_out)) {
	sprintf(temp_gpOut, "echo %d > /sys/class/gpio/export", nGp2_out);
	system(temp_gpOut);
	sprintf(temp_gpOut, "echo out > /sys/class/gpio/gpio%d/direction", nGp2_out);
	system(temp_gpOut);
      }

      if ((bit_typ3 != 0) && (nGp3_out)) {
	sprintf(temp_gpOut, "echo %d > /sys/class/gpio/export", nGp3_out);
	system(temp_gpOut);
	sprintf(temp_gpOut, "echo out > /sys/class/gpio/gpio%d/direction", nGp3_out);
	system(temp_gpOut);
      }

      if ((bit_typ4 != 0) && (nGp4_out)) {
	sprintf(temp_gpOut, "echo %d > /sys/class/gpio/export", nGp4_out);
	system(temp_gpOut);
	sprintf(temp_gpOut, "echo out > /sys/class/gpio/gpio%d/direction", nGp4_out);
	system(temp_gpOut);
      }
}
// ******************** end Вариант №1  ********************** //
// *********************************************************** //


// *********************************************************** //
// ********************  Вариант №2  ************************* //
// ** ADD pin out через открытие-запись в фаил устройства **** //
// *********************************************************** //
/*
void add_gpioOut() {	//	init add power gpio-out	//

	sprintf(temp_gpOut, "/sys/class/gpio/export");
	sprintf(tst_gpOut, "out");
//	sprintf(tst_gpOut, "in");

	if (bit_typ1 != 0) {
	   if (nGp1_out) {
		gpioTmp = fopen(temp_gpOut, "w");
		sprintf(pin_gpOut, "%d", nGp1_out);
	  	fputs(pin_gpOut, gpioTmp); // записать "nomer-gpio" в файл
	  	    fclose (gpioTmp);
		sprintf(temp_str_gpOut, "/sys/class/gpio/gpio%d/direction", nGp1_out);
		if ((gpioTmp = fopen(temp_str_gpOut, "a")) != NULL) {
	  	    fputs(tst_gpOut, gpioTmp); // записать "out" в файл
	  	    fclose (gpioTmp);
		}
	   }
	}
	if (bit_typ2 != 0) {
	   if (nGp2_out) {
		gpioTmp = fopen(temp_gpOut, "w");
		sprintf(pin_gpOut, "%d", nGp2_out);
	  	fputs(pin_gpOut, gpioTmp); // записать "nomer-gpio" в файл
	  	    fclose (gpioTmp);
		sprintf(temp_str_gpOut, "/sys/class/gpio/gpio%d/direction", nGp2_out);
		if ((gpioTmp = fopen(temp_str_gpOut, "a")) != NULL) {
	  	    fputs(tst_gpOut, gpioTmp); // записать "out" в файл
	  	    fclose (gpioTmp);
		}
	   }
	}
	if (bit_typ3 != 0) {
	   if (nGp3_out) {
		gpioTmp = fopen(temp_gpOut, "w");
		sprintf(pin_gpOut, "%d", nGp3_out);
	  	fputs(pin_gpOut, gpioTmp); // записать "nomer-gpio" в файл
	  	    fclose (gpioTmp);
		sprintf(temp_str_gpOut, "/sys/class/gpio/gpio%d/direction", nGp3_out);
		if ((gpioTmp = fopen(temp_str_gpOut, "a")) != NULL) {
	  	    fputs(tst_gpOut, gpioTmp); // записать "out" в файл
	  	    fclose (gpioTmp);
		}
	   }
	}
	if (bit_typ4 != 0) {
	   if (nGp4_out) {
		gpioTmp = fopen(temp_gpOut, "w");
		sprintf(pin_gpOut, "%d", nGp4_out);
	  	fputs(pin_gpOut, gpioTmp); // записать "nomer-gpio" в файл
	  	    fclose (gpioTmp);
		sprintf(temp_str_gpOut, "/sys/class/gpio/gpio%d/direction", nGp4_out);
		if ((gpioTmp = fopen(temp_str_gpOut, "a")) != NULL) {
	  	    fputs(tst_gpOut, gpioTmp); // записать "out" в файл
	  	    fclose (gpioTmp);
		}
	   }
	}
}
*/
// ******************** end Вариант №2  ********************** //
// *********************************************************** //


// ******************************************** //
// 	stryng_thermoAllarm init, read, setup	//
//	4 - №-gpio, 4 - allarm, 4 - hister	//
void set_thermoAllarm() {
int
	i=0,
	str_i=0,
	for_i=0,
	th_i=0,
	statHund=0,
	statDec=0,
	statUnit=0;
  if (gpioPin_est == 1) {
    for (for_i=0; for_i<16; for_i++) {
char	t_thermoAllarm[3];
	th_i=0;
	statHund=0;
	statDec=0;
	statUnit=0;

	while (stryng_thermoAllarm[str_i] != '/') t_thermoAllarm[i++] = stryng_thermoAllarm[str_i++];
	if ((i == 3) || (i == 2) || (i == 1)) {
		if (i == 3) statHund = (t_thermoAllarm[th_i++] - 48)*100;
		if (i >= 2) statDec = (t_thermoAllarm[th_i++] - 48)*10;
		if (i >= 1) statUnit = (t_thermoAllarm[th_i++] - 48)*1;
	}
		if (for_i == 0) nGp1_out = (statHund + statDec + statUnit); 
		if (for_i == 1) nGp2_out = (statHund + statDec + statUnit); 
		if (for_i == 2) nGp3_out = (statHund + statDec + statUnit); 
		if (for_i == 3) nGp4_out = (statHund + statDec + statUnit); 

		if (for_i == 4) alarm1 = (statHund + statDec + statUnit); 
		if (for_i == 5) alarm2 = (statHund + statDec + statUnit); 
		if (for_i == 6) alarm3 = (statHund + statDec + statUnit); 
		if (for_i == 7) alarm4 = (statHund + statDec + statUnit); 

		if (for_i == 8) histe1 = (statHund + statDec + statUnit); 
		if (for_i == 9) histe2 = (statHund + statDec + statUnit); 
		if (for_i == 10) histe3 = (statHund + statDec + statUnit); 
		if (for_i == 11) histe4 = (statHund + statDec + statUnit); 

		if (for_i == 12) invers1 = (statHund + statDec + statUnit); 
		if (for_i == 13) invers2 = (statHund + statDec + statUnit); 
		if (for_i == 14) invers3 = (statHund + statDec + statUnit); 
		if (for_i == 15) invers4 = (statHund + statDec + statUnit);
	str_i++;
	i=0;
    }
  }
}
// ******************************************** //


// ******************************************** //
void read_id_ds18() {
  int
	ai=0,
	aai=0,
	ia;

	while (ai < kollw_sensor) {
		if (ds18b20_sensor[aai] == 0x5C) aai++;
		for (ia=0; ia<15; ia++) {
			if (ai == 0) ds0_path[ia] = ds18b20_sensor[aai];
			if (ai == 1) ds1_path[ia] = ds18b20_sensor[aai];
			if (ai == 2) ds2_path[ia] = ds18b20_sensor[aai];
			if (ai == 3) ds3_path[ia] = ds18b20_sensor[aai];
			 aai++;
		}
		ai++;
	}
}
// ******************************************** //



// ******************************************** //
void clock_plus_ds18() {
	if (kollw_sensor < 4 && BIT_CLOCK == 1) {
		write_CMD(LCD_SETSTROKA1);	// Set cursor to stroka-4  позиция - 0
		setclock();
	}
}
// ******************************************** //


// ******************************************** //
// LCD initialization
void lcd_init() {
	usleep(5000);
	write_CMD(0x03);
	usleep(5000);
	write_CMD(LCD_RETURNHOME);
	usleep(5000);
	write_CMD(0x28);
	usleep(5000);
	write_CMD(0x2C);
	usleep(5000);
	write_CMD(0x0C);
	usleep(5000);
	write_CMD(LCD_CLEARDISPLAY);
	usleep(5000);
	write_CMD(0x06);
	usleep(5000);
	write_CMD(LCD_RETURNHOME);
	usleep(5000);
	lcd_init_CGRAM();
}
// ******************************************** //



// ******************************************** //
// сложение и write команд для дисплей
void write_CMD(char c) {
	int senior = c & 0xF0, junior = (c<<4) & 0xF0; // побитный сдвиг влево на 4-разряда === left shift then and with 0xF0
	digitalWrite_i2c(LCD_SENIORCMD+senior+led+Rw);
	digitalWrite_i2c(LCD_JUNIORCMD+senior+led+Rw);
	digitalWrite_i2c(LCD_SENIORCMD+junior+led+Rw);
	digitalWrite_i2c(LCD_JUNIORCMD+junior+led+Rw);
}

// сложение и write данных для дисплей
void write_dat(char c) {
	int senior = c & 0xF0, junior = (c<<4) & 0xF0; // побитный сдвиг влево на 4-разряда === left shift then and with 0xF0
	digitalWrite_i2c(LCD_SENIORDATA+senior+led+Rw);
	digitalWrite_i2c(LCD_JUNIORDATA+senior+led+Rw);
	digitalWrite_i2c(LCD_SENIORDATA+junior+led+Rw);
	digitalWrite_i2c(LCD_JUNIORDATA+junior+led+Rw);
}
// Вывод на дисплей
void digitalWrite_i2c(char buf) {
	usleep(50);
	if (write(file,&buf,1) != 1) {
		fprintf(stderr,"Write Error : %s (%d)\n",strerror(errno),errno);
		errFl = 1;
		sprintf(temp_gpOut,"Write Error: %s (%d)",strerror(errno),errno);
		error_log(temp_gpOut);
	}
	usleep(50);
}
// ******************************************** //


// ******************************************** //
// Print output string
void stringout_lcd_Wide0(char * c) {
	int n=0;
	// Print characters
	while(c[n]!=0) {
		write_dat(c[n++]);
	}
}
// ******************************************** //


// ******************************************** //
// TERMOSTAT
void thermo_stat() {

/*

i2c_HD44780 -d 0 -a 0x27 -f NNN -u 111011100 -b 28-000002d0797a\28-000002d07b76\\\\ -c '   Dom:'/'  Dom2:'/// -p 21/22///325/305/9/366/321/298/14/360/
*/

int t_allarm;
int t_histe;
int t_invers;

if (gpioPin_est == 1) {
	prmb=0;
	if ((iia == 0) && (bit_typ1 != 0)) {
		sprintf(read_file_ds18_path, "/sys/class/gpio/gpio%d/value", nGp1_out);
		t_allarm = alarm1;
		t_histe = histe1;
		t_invers = invers1;

	    if (t_allarm == t_histe) histe1 -= 1;
		prmb=1;
	}
	if ((iia == 1) && (bit_typ2 != 0)) {
		sprintf(read_file_ds18_path, "/sys/class/gpio/gpio%d/value", nGp2_out);
		t_allarm = alarm2;
		t_histe = histe2;
		t_invers = invers2;

	    if (t_allarm == t_histe) histe2 -= 1;
		prmb=1;
	}
	if ((iia == 2) && (bit_typ3 != 0)) {
		sprintf(read_file_ds18_path, "/sys/class/gpio/gpio%d/value", nGp3_out);
		t_allarm = alarm3;
		t_histe = histe3;
		t_invers = invers3;

	    if (t_allarm == t_histe) histe3 -= 1;
		prmb=1;
	}
	if ((iia == 3) && (bit_typ4 != 0)) {
		sprintf(read_file_ds18_path, "/sys/class/gpio/gpio%d/value", nGp4_out);
		t_allarm = alarm4;
		t_histe = histe4;
		t_invers = invers4;

	    if (t_allarm == t_histe) histe4 -= 1;
		prmb=1;
	}


		readd_content_file();	//*	read GPIO	* /
	if (prmb == 1) {		//*	OK read GPIO	* /
	  if (t_invers == 0) {
	  	    //******************************************** /
	  	    //	echo 0-1 > /sys/class/gpio/gpio%d/value	//
	  	    //******************************************** /
	    if (t_allarm >= t_histe) {	//*	THERMOSTAT	* /
	  	    	gpioTmp = fopen(read_file_ds18_path, "w");
		if ((tempraw[0]-48) == 0) {
	  		if (stat_data_termo < t_allarm) fputs("0", gpioTmp); 	// записать "0" в gpio
	  		else fputs("1", gpioTmp); 				// записать "1" в gpio
		} else {
	  		if (stat_data_termo <= t_histe) fputs("0", gpioTmp); 	// записать "0" в gpio
	  		else fputs("1", gpioTmp); 				// записать "1" в gpio
		}
	  	    	fclose (gpioTmp);

	    } else {			//*	CONDITIONING	* /
	  	    	gpioTmp = fopen(read_file_ds18_path, "w");
		if ((tempraw[0]-48) == 0) {
	  		if (stat_data_termo > t_allarm) fputs("0", gpioTmp); 	// записать "0" в gpio
	  		else fputs("1", gpioTmp); 				// записать "1" в gpio
		} else {
	  		if (stat_data_termo >= t_histe) fputs("0", gpioTmp); 	// записать "0" в gpio
	  		else fputs("1", gpioTmp); 				// записать "1" в gpio
		}
	  	    	fclose (gpioTmp);
	  	    //******************************************** /
	    }
	  } else if (t_invers == 1) {
	  	    //******************************************** /
	  	    //	echo 0-1 > /sys/class/gpio/gpio%d/value	//
	  	    //******************************************** /
	    if (t_allarm >= t_histe) {	//*	THERMOSTAT	* /
	  	    	gpioTmp = fopen(read_file_ds18_path, "w");
		if ((tempraw[0]-48) == 1) {
	  		if (stat_data_termo < t_allarm) fputs("1", gpioTmp); 	// записать "1" в gpio
	  		else fputs("0", gpioTmp); 				// записать "0" в gpio
		} else {
	  		if (stat_data_termo <= t_histe) fputs("1", gpioTmp); 	// записать "1" в gpio
	  		else fputs("0", gpioTmp); 				// записать "0" в gpio
		}
	  	    	fclose (gpioTmp);

	    } else {			//*	CONDITIONING	* /
	  	    	gpioTmp = fopen(read_file_ds18_path, "w");
		if ((tempraw[0]-48) == 1) {
	  		if (stat_data_termo > t_allarm) fputs("1", gpioTmp); 	// записать "1" в gpio
	  		else fputs("0", gpioTmp); 				// записать "0" в gpio
		} else {
	  		if (stat_data_termo >= t_histe) fputs("1", gpioTmp); 	// записать "1" в gpio
	  		else fputs("0", gpioTmp); 				// записать "0" в gpio
		}
	  	    	fclose (gpioTmp);
	  	    //******************************************** /
	    }
	  }
	}
	prmb=0;
 	usleep(100000);
}


}
// ******************************************** //




// ************************************************************** //
// ************************************************************** //
// read w1_ds18 . ОТКРЫТИЕ - ЧТЕНИЕ ВО ВРЕМЕННЫЙ РЕГ. - ЗАКРЫТИЕ
// ************************************************************** //
void conv_w1_ds18() {
   int
	ascii=48,		/*	ASCII	*/
	iu=0,
	uz=0,
	zu=0,
	i=0,
	re_read=0,
	icount=0,	// * колличество прочитанных символов с ds18 БЕЗ ЗНАКА минус '-' минусовая температуура * /
	ib=0;		// * колличество символов с ds18 CO ЗНАК минус '-' минусовая температуура * /

   readd_content_file();

	// *		Обработка ошибок при чтении датчика		* //
		if ((tempraw[36] == 'Y') &&
				(tempraw[36] == 'E') && 
					(tempraw[36] == 'S')) tfail = 0;

		// valid CRC (recheck cycle removed because it increased the load)
		if (tempraw[36] != 'Y') {
				// Auto re-read parameter sensor ('-r')
				while (tempraw[36] == 'N' && re_read < 3) {
					readd_content_file();
					re_read++;
				}
		// false CRC Error Code
		} else if (tempraw[33] == '0' && tempraw[34] == '0') {
			tfail = 2;
			printf("Read Sensor-%d CRC-code Error:\n", (iia+1));
			sprintf(temp_gpOut," Error: Read Sensor-%d CRC-code", (iia));
			error_log(temp_gpOut);
		}
	// ************************************************************** //


	// ************************************************************** //
 if (tfail != 1) {

	if (tempraw[69] == '-') {			/* проверим есть ли "-" минус */
		/* да есть "-" минус */
		temp_sensor_data[0]=tempraw[69];
		i++;
		ib=i;
		zu=1;
	} else {					/* нету  "-" минус */
		temp_sensor_data[0]=' ';
		i++;
		ib=0;
	}
		iu=ib;
	while (tempraw[69 + iu] != '\n') {		/* подщитаем колличество символов с ds18 БЕЗ ЗНАКА минус '-' минусовая температуура */
		iu++;
		icount++;				/* колличество символов с ds18 БЕЗ ЗНАКА минус '-' минусовая температуура */
	}
			//			printf("icount:  %d \n", icount);    /* konsol */
	// ************************************************************** //


	// ************************************************************** //
	// *	Чтение с переменной датчика в переменную для LCD	* //
	// *	Последние два разряда упускаем.				* //
	// *	Считанные с датчика данные, ставим точку		* //
	// *	 и показываем только один разряд после точки.		* //
	// *	Гасим не значащие нули					* //
	// ************************************************************** //
	uz = icount - 2;	// * Последние два разряда упускаем * //

	int ik;
	if (icount > 3) {
	    for (ik=0; ik<=uz; ik++) {
		 if (ik==uz-1) {
		temp_sensor_data[i]='.';
		} else {
		temp_sensor_data[i]=tempraw[69 + ib];
		ib++;
		}
		i++;
	    }
	} else if (icount == 3) {
		temp_sensor_data[i++]='0';
		temp_sensor_data[i++]='.';
		temp_sensor_data[i++]=tempraw[69 + ib];

	} else if (icount < 3) {
		i=0;
		temp_sensor_data[i++]=' ';
		temp_sensor_data[i++]='0';
		temp_sensor_data[i++]='.';
		temp_sensor_data[i++]='0';
	} 

	temp_sensor_data[i++]='C';
	temp_sensor_data[i++]=' ';
	// ************************************************************** //
 

	// ************************************************************** //
	// *	Чтение с переменной датчика в переменную для термостата	* //
	// *	Считанные данные, ПОРАЗРЯДНО переводим в ASCII		* //
	// *	ПОРАЗРЯДНО умножаем на 1000, 100, 10, 1 и их сумируем	* //
	// *	для дальнейшей обработки в термостате			* //
	// ************************************************************** //
	if ((icount == 6) || (icount == 5) || (icount == 4) || (icount == 3)) {
		if (icount == 6) 
			stat_data_termo = ((tempraw[69 + zu++] - ascii)*1000 + 
						(tempraw[69 + zu++] - ascii)*100 + 
							(tempraw[69 + zu++] - ascii)*10 + 
								(tempraw[69 + zu++] - ascii)*1); 
		if (icount == 5) 
			stat_data_termo = ((tempraw[69 + zu++] - ascii)*100 + 
						(tempraw[69 + zu++] - ascii)*10 + 
							(tempraw[69 + zu++] - ascii)*1); 
		if (icount == 4) 
			stat_data_termo = ((tempraw[69 + zu++] - ascii)*10 + 
						(tempraw[69 + zu++] - ascii)*1); 
		if (icount == 3) 
			stat_data_termo = (tempraw[69 + zu++] - ascii)*1;
	} else if (icount < 3) stat_data_termo = 0;
	// ************************************************************** //


	// **************  Проверим GPIO ТЕРМОРЕГУЛЯТОРА  ************** //
	   if ((bit_typ1 == 1) || (bit_typ1 == 2) ||
			(bit_typ2 == 1) || (bit_typ2 == 2) ||
				(bit_typ3 == 1) || (bit_typ3 == 2) ||
					(bit_typ4 == 1) || (bit_typ4 == 2)) thermo_stat();	//	thermo_REGULUS	//
	// ************************************************************** //

 }
}
// ************************************************************** //
// ************************************************************** //



// ******************************************** //
// read nickname sensor
void read_nickname() {
  int i=0;
	while (nickname_sensor[i_nick] != '/') {
		if ( i < 10) temp_sensor_txt[i++] = nickname_sensor[i_nick];
		i_nick++;
	}
	if (i == 0) sprintf(temp_sensor_txt, "Sensor-%d: ", (iia+1));
	else while (i < 10) temp_sensor_txt[i++] = ' ';
	i_nick++;
}
// ******************************************** //



// ******************************************** //
// РАБОТА С ФАЙЛОМ. ОТКРЫТИЕ - ЧТЕНИЕ ВО ВРЕМЕННЫЙ РЕГ. - ЗАКРЫТИЕ
void readd_content_file(void) {

  FILE *tempfile;
  char
	*tempbuf,
	strtemp[16];
  int
	size,
	secnd = 0;

	// Check for w1_slave read failure (device removed)
	if ((tempfile = fopen(read_file_ds18_path, "r")) == NULL) {	// fopen() открывает файл, имя которого указано аргументом fname, и  возвращает связанный с ним указатель. 
		
	     if (prmb != 1) {
			tfail = 1; // флаг отсутствия файла (Error Code #1)
			if ((BIT_one_w1 == 1) && (ds_id == 1) && (BIT_WHILE == 1)) {
				printf("Error open gpio-W1 ds1820 - Sensor-'%d' \n", (iia+1));
				sprintf(temp_gpOut,"Error open gpio-W1 ds1820 - Sensor-'%d' \n", (iia));
				error_log(temp_gpOut);

			} else if (BIT_STRING == 1) {
				printf("Error open ds1820 - UART-USB:\n");
				error_log("Error open ds1820 - UART-USB");
			} else if (BIT_WHILE == 0) {
				printf("Error read file\n");
				error_log("Error read file");
			}
	     } else if (prmb == 1) {
			int ji=iia+1;
			printf("Error: Nou pin.GPIO-OUT 'Sensor-%d'\n", ji);
			prmb=0;
			sprintf(temp_gpOut,"Error: Nou pin.GPIO-OUT 'Sensor-%d'", ji);
			error_log(temp_gpOut);
	     }


	} else {
		tfail = 0; // зброс флаг присутствия файла
		
		// Get content of w1_slave to variable
		fseek(tempfile, 0, SEEK_END);	// fseek() устанавливает указатель положения в файле, связанном со  stream, в соответствии со значениями offset и origin.
		size = ftell(tempfile);		// fftell определяет текущую позицию в потоке данных, на  который указывает аргумент tempfile. 
		rewind(tempfile);		// rewind()  перемещает указатель положения в файле на начало указанного потока.
		tempbuf = calloc(size + 1, 1);	// calloc выделяет только указанный массив и плюс к этому обнуляет все внутри него.
		fread(tempbuf, 1, size, tempfile); // fread считывает массив размером — size элементов,  каждый из которых имеет размер size байт, из потока,

		// Close file stream
		fclose(tempfile); // fclose закрывает и разъединяет файл tempfile ,  связанный с потоком. 
		strcpy(tempraw, tempbuf); // strcpy() используется для копирования содержимого str2 в str1.  Аргумент str2 должен быть указателем на строку, оканчивающуюся нулем.
		free(tempbuf); // free() возвращает память, на которую указывает параметр tempbuf, назад  в кучу. В результате эта память может выделяться снова
	}
}
// ******************************************** //


// ******************************************** //
// Get current read_file
void read_ffile(void) {
  int
	ii = 0,	// расположение символа в буфере
	iu = 0,	// расположение символа в stroka
	fl = 0,	// номер строки
	buff;	// kолличество символов записанных в буфер

	readd_content_file();
			
	if (tfail != 1) {
			// find_boom - delete "\xEF\xBB\xBF" utf8
		if ((tempraw[0] == 0xFFFFFFEF) && (tempraw[1] == 0xFFFFFFBB) && (tempraw[2] == 0xFFFFFFBF)) {
			ii=3;
			buff = strlen(tempraw) - 3;
		}	// end find_boom
		else buff = strlen(tempraw);

		int i = 0;
		for (i=0; i<buff; i++) {

			   if (tempraw[ii] == 0x0D) { // если есть символ <<возврат каретки>> в utf8, пропустим.
				ii++; 		      // пропустим.
				buff -= 1;
			   }
			   if (tempraw[ii] == 0x09) tempraw[ii] = 0x20; // если есть табуляция, заменим на пробел

			if (tempraw[ii] != 0x0A) {

			   if ((iu < 20) && (tempraw[ii] >= 0x20) && (tempraw[ii] < 0x7F)) {
				if (fl == 0) stroka1[iu++] = tempraw[ii];
				else if (fl == 1) stroka2[iu++] = tempraw[ii];
				else if (fl == 2) stroka3[iu++] = tempraw[ii];
				else if (fl == 3) stroka4[iu++] = tempraw[ii];
			   }

			   ii++;

			} else {			// 0x0A  символ переноса - новая строка.

			    int cou_probe = 20-iu;	// прочитанное и записанное количество символов из tempraw в stroka
			    if (cou_probe != 20) {
				int hi;
				for(hi=0; hi < cou_probe; hi++) {
					if (fl == 0) stroka1[iu++] = 0x20;	// 0x20  символ пробел 
					if (fl == 1) stroka2[iu++] = 0x20;	// 0x20  символ пробел 
					if (fl == 2) stroka3[iu++] = 0x20;	// 0x20  символ пробел 
					if (fl == 3) stroka4[iu++] = 0x20;	// 0x20  символ пробел 
				}
			    }

			    ii++;
			    iu=0;
			    fl++;
			}
		}
	}
}
// ******************************************** //



// ******************************************** //
// Send output string to LCD
void print_lcd(void) {

   if(BIT_WHILE == 1) {
	if(BIT_CLOCK == 1) write_CMD(LCD_SETSTROKA2);
	else write_CMD(LCD_SETSTROKA1);
   }
   else write_CMD(LCD_SETSTROKA1);
	usleep(500);
	stringout_lcd_Wide0(stroka1);

   if(BIT_WHILE == 1) {
	if(BIT_CLOCK == 1) write_CMD(LCD_SETSTROKA3);
	else write_CMD(LCD_SETSTROKA2);
   }
   else write_CMD(LCD_SETSTROKA2);
	usleep(500);
	stringout_lcd_Wide0(stroka2);

   if(BIT_WHILE == 1) {
	if(BIT_CLOCK == 1) write_CMD(LCD_SETSTROKA4);
	else write_CMD(LCD_SETSTROKA3);
   }
   else write_CMD(LCD_SETSTROKA3);
	usleep(500);
	stringout_lcd_Wide0(stroka3);

	if(BIT_CLOCK != 1) {
	write_CMD(LCD_SETSTROKA4);
	usleep(500);
	stringout_lcd_Wide0(stroka4);
	}
}
// ******************************************** //

// ******************************************** //
// ******************************************** //
// Main function
int main(int argc, char** argv) {
  int	bnr=0,
	errno,
	ret,
	i2c_addr,
	i,
	I2C_id=0,
	ic = 0,
	ccc = 0;
  char	*banner=0,
	*I2C_Device,
	*filename,
	*setlcd,
	ch;

	// *************** Get input arguments ************************************************************************* //
	while((ret = getopt(argc, argv, "d:a:f:u:b:c:p:n:l:s:")) != -1) {
		switch(ret) {
			case 'd':		//  /dev/i2c-"0"- d 
				I2C_Device = strdup(optarg);
				I2C_id=1;
				ic++;
				break;
			case 'a':		//  option 'address' '0x27' (" -a <address>  address of the PCF8574, e.g. '0x27' (required)\n")
				i2c_addr = strtoul(optarg, NULL, 0);
				ic++;
				break;
			case 'f':		//  
				filename = strdup(optarg);
				fl_es = 1;
				ic++;
				break;
			case 'u':		//  -u setup LCD 
				setlcd = strdup(optarg);
				break;

			case 'b':		//  option 'address' 1wir-ds1820. Example: 28-000002d0797a\28-000002d07b76\28-0000034b4dc6\28-0000034b309e
				ds18b20_sensor = strdup(optarg);
				ds_id = 1;
				break;

			case 'c':		//  
				nickname_sensor = strdup(optarg);
				ds_nik = 1;
				break;

			case 'p':	// 4 - №-gpio, 4 - allarm, 4 - hister: -p 21/22/1/2/375/280/200/200/370/275/195/195//
				stryng_thermoAllarm = strdup(optarg);
				gpioPin_est = 1;
				break;

			case 'n':	// banner
				banner = strdup(optarg);
				bnr = 1;
				break;

			case 'l':	// 
				wide_simvol = atoi(optarg) + 48;
				lcd_l = 1;
				break;

			case 's':	// 
				lcd_number = atoi(optarg);
				lcd_n = 1;
				break;
		}
	}
	// *************** end Get input arguments ********************************************************************* //


/*
###################################
#	i2c_HD44780 -d 0 -a 0x27 -f NNN -u 0x1x	#  первый с лева ноль (-u функция) старт-стоп  -  чтение файла на lcd с первой строки  -  LED-on 
#	i2c_HD44780 -d 0 -a 0x27 -f NNN -u 1110	#  по кругу  -  чтение файла  -  LED-on  -  на lcd с первой строки
#	i2c_HD44780 -d 0 -a 0x27 -f NNN -u 1111	#  по кругу  -  чтение файла  -  LED-on  -  на lcd со второй строки (пропуск первой)
#	i2c_HD44780 -d 0 -a 0x27 -f NNN -u 1010	#  по кругу  -      Часы     -        LED-on  -  Часы на первой строке
#	i2c_HD44780 -d 0 -a 0x27 -f NNN -u 1011	#  по кругу  -      Часы     -        LED-on  -  Часы на первой строке,,,  чтение файла на lcd со второй строки

хххх1 - чтение ds18B20 w1
хххх2 - чтение ds18B20 digitemp
-b параметр - серийники ds18
 Между серийниками ставить косую линию (\) или два символа (``) который на кнопке ё клавы .
 Пробелов Между серийниками и разделяющими их символами не должно быть.

 Команда для тестов в PUTTY с одним 18В20:
 часы выключены - LED включен
i2c_HD44780 -a 0x27 -f NNN -u 101010000 -b 28-000002d0797a
 часы включены- LED включен
i2c_HD44780 -a 0x27 -f NNN -u 111010000 -b 28-000002d0797a

 Команда для тестов в PUTTY с двумя:
i2c_HD44780 -a 0x27 -f NNN -u 101010000 -b 28-000002d0797a``28-0000034b309e
 или так:
i2c_HD44780 -a 0x27 -f NNN -u 101010000 -b 28-000002d0797a\28-0000034b309e

 Команда для тестов в PUTTY с четырьмя:
i2c_HD44780 -a 0x27 -f NNN -u 101010000 -b 28-000002d0797a``28-000002d07b76``28-0000034b4dc6``28-0000034b309e
 или так:
i2c_HD44780 -a 0x27 -f NNN -u 101010000 -b 28-000002d0797a\28-000002d07b76\28-0000034b4dc6\28-0000034b309e


 Команда для тестов в PUTTY с четырьмя + именами:
i2c_HD44780 -a 0x27 -f NNN -u 101010000 -b 28-000002d0797a\28-000002d07b76\28-0000034b4dc6\28-0000034b309e -c Dom1/Ulica_okno/DDDDD/ppppppppppp///
*/


	// *** Check for the minimal arguments is present (device, address) *** //
	if(ic<2) {
		printf("  \n\n");
		printf("I2C PCF8574 bus expander LCD HD44780\n");
		printf(" '------------   Info pin    --------------------'\n");
		printf("Pins of PCF8574:   P7  P6  P5  P4  P3   P2  P1  P0\n");
		printf("Pins of HD44780:   D7  D6  D5  D4  LED  E   RW  RS\n");
		printf(" '-----------------------------------------------'\n");
		printf("Usage: '%s <parameters>'\n", argv[0]);
		printf(" -d <device>   I2C bus selection, e.g. '/dev/i2c-0' '0' (required)\n");
		printf(" -a <address I2C device>  address of the PCF8574, e.g. '0x27' (required)\n");
		printf(" -f <read file>  address of the file NNN (required)\n");
		printf(" -u 01x0 - start->stop, read file LCD line-1\n");
		printf(" -u 11x0 - start while, read file LCD line-1\n");
		printf(" -u 11x1 - start while, read file LCD line-2, LCD line-1 pusto\n");
		printf(" -u 10x0 - start while, clock LCD line-1\n");
		printf(" -u 10x1 - start while, clock LCD line-1,  read file LCD line-2\n");
		printf(" -u xx1x - LED-on, xx0x - LED-off\n\n");
		printf("Usage: '%s -d <device> -a <address I2C device> -f <read file> -u <xxxx>'\n\n", argv[0]);
		printf("Example command line (while OFF): \n");
		printf(" %s -d 0 -a 0x27 -f NNN -u 0010\n\n", argv[0]);
		printf("         (while ON):     \n");
		printf(" %s -d 0 -a 0x27 -f NNN -u 1010\n\n", argv[0]);
		printf("           Clock LCD line-1\n");
		printf("Example command line: \n");
		printf(" %s -d 0 -a 0x27 -f NNN -u 1110\n\n", argv[0]);
		printf("          DS181B20 LCD line-1\n");
		printf("Example command line: \n");
		printf(" %s -d 0 -a 0x27 -f NNN -u 101010000 -b 28-000002d0797a`\`28-000002d07b76`\`28-0000034b309e\n\n", argv[0]);
		printf(" %s -d 0 -a 0x27 -f NNN -u 101010000 -b 28-000002d0797a`\`28-000002d07b76`\`28-0000034b309e`\`28-0000034b4dc6\n\n", argv[0]);
		printf(" %s -a 0x27 -f NNN -u 10101 -b 28-000002d0797a`\`28-000002d07b76`\`28-0000034b309e`\`28-0000034b4dc6\n\n", argv[0]);
		printf("           Clock LCD line-1\n");
		printf(" %s -a 0x27 -f NNN -u 111010000 -b 28-000002d0797a`\`28-000002d07b76`\`28-0000034b309e\n\n", argv[0]);
		printf("           Clock LCD line-1 sensor and nick\n");
		printf(" %s -a 0x27 -f NNN -u 111010000 -b 28-000002d0797a`\`28-000002d07b76`\`28-0000034b309e -c '  Dom: '/Ulica///\n\n", argv[0]);
		printf("Error-Not argumets! Read http://www.cyber-place.ru/showthread.php?t=2164\n\n");
		errFl = 1;
		error_log("Error-Not argumets! Read CONSOL-HELP");
	}


	// ************************************************************************************************************ //
	// ********* I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C ********* //

	sprintf(setlcd_patch, "%s", setlcd);

		// I2C device
	if (I2C_id == 1) sprintf(dev_path, "/dev/i2c-%s", I2C_Device);
	else sprintf(dev_path, "/dev/i2c-0");

	if ((file = open(dev_path, O_RDWR)) < 0) {
		errFl = 1;
		fprintf(stderr,"Open I2C_Dev Error: %s (%d)\n",strerror(errno),errno);
		sprintf(temp_gpOut,"Open I2C_Dev Error: %s (%d)",strerror(errno),errno);
		error_log(temp_gpOut);
	}

	// address
	if (ioctl(file,I2C_SLAVE,i2c_addr) < 0) {
		errFl = 1;
		fprintf(stderr,"I2C_SLAVE Error: %s (%d)\n",strerror(errno),errno);
		sprintf(temp_gpOut,"I2C_SLAVE address Error: %s (%d)",strerror(errno),errno);
		error_log(temp_gpOut);
	} 
	// ********* I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C  I2C ********* //
	// ************************************************************************************************************ //


//     if (BIT_LED == 0) led = 0x00;
//     else led = 0x08;
     if (BIT_LED == 1) led = LCD_BACKLIGHT;
     else led = LCD_NOBACKLIGHT;
     lcd_init();

	if (fl_es == 1) {
		// ******** banner ******** //
	    if ((bnr == 1) && (banner[0] == 49)) {
		sprintf(read_file_ds18_path, "echo Free mem: `df -h | awk '{print $4}' | sed 's/G//' | sed '5!d'` > /tmp/tmpo");
		system(read_file_ds18_path);
		usleep(500);
		sprintf(read_file_ds18_path, "grep machine /proc/cpuinfo | awk '{print $3, $4, $5}' >> /tmp/tmpo");
		system(read_file_ds18_path);
		usleep(500);
		sprintf(read_file_ds18_path, "grep 'system type' /proc/cpuinfo | awk '{print $4, $5, $6, $7}' >> /tmp/tmpo");
		system(read_file_ds18_path);
		sprintf(read_file_ds18_path, "/tmp/tmpo");
			read_ffile();
			print_lcd();
		sprintf(read_file_ds18_path, "rm /tmp/tmpo");
		system(read_file_ds18_path);
		sleep(2);
	    }	// ******** end banner ******** //
	    write_CMD(LCD_CLEARDISPLAY);
	    usleep(500);
	}

	if ((fl_es == 1) || (BIT_STRING == 1)) {
		if ((BIT_digitemp != 1) || (BIT_digitemp != 2)) {
		   FILE *f;
			sprintf(read_file_ds18_path, "/tmp/%s", filename);
			f = fopen(read_file_ds18_path, "r"); 
			if(f == NULL) {
				f = fopen(read_file_ds18_path, "w"); 
				printf("Error. Add nev file\n");
			}
			fclose(f);
		}
	}

	// ******** read thermoAllarm - init gpio-out ******** //
     if ((gpioPin_est == 1) && (ds_id == 1)) {
	set_thermoAllarm();	// *** READ setup thermoAllarm *** //
	add_gpioOut();		// *** init add power gpio-out *** //
     }	// ******** end read thermoAllarm - init gpio-out ******** //


     // ********* WHILE ********* //
     if (BIT_WHILE == 1) {

	// *** read gpio-w1 sensor and print LCD *** //
	// ************ ds18b20_sensor ************* //
	if ((BIT_one_w1 == 1) && (ds_id == 1) && (BIT_WHILE == 1)) {
		FILE 	*t_nick;
		char 	temp_str_str[256];

		if (strlen(ds18b20_sensor) < 15) {
			write_CMD(LCD_SETSTROKA1);
			stringout_lcd_Wide0("No w1 ds18.. ERR!");
			fprintf(stderr, "Sensor w1 Err: invalid device identifier\n");
			error_log("ensor w1 Err: invalid device identifier");
//			exit(EXIT_FAILURE);
		}
					//	printf("read ds18b20_sensor - s: %s\n", ds18b20_sensor);
			while (kollw_sensor != strlen(ds18b20_sensor)) kollw_sensor++;	//	vicislyaem kollwo datcikov

			kollw_sensor /= 15;
					//	printf("read kollw_sensor - d: %d\n", kollw_sensor);
			read_id_ds18();
							 //  ds18b20_sensor(); // преобразование с датчика и вывод на LCD - В ПЕРВУЮ СТРОКУ
							//	sprintf(read_file_ds18_path, "/sys/bus/w1/devices/28-000002d0797a/w1_slave");
							//	sprintf(read_file_ds18_path, "/sys/bus/w1/devices/28-000002d07b76/w1_slave");
							//	sprintf(read_file_ds18_path, "/sys/bus/w1/devices/28-0000034b4dc6/w1_slave");
							//	sprintf(read_file_ds18_path, "/sys/bus/w1/devices/28-0000034b309e/w1_slave");
									//  clock 			 : -u 10xxxxxx
		//	write_CMD(LCD_SETSTROKA1);	// Set cursor to stroka-1  позиция - 0
		//	setclock();

			i_nick = 0;
			for (iia=0; iia<kollw_sensor; iia++) {
				if (iia == 0) {
					if (BIT_CLOCK == 1 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA2);
					else if (BIT_CLOCK == 1 && kollw_sensor < 4 && wide_simvol == '1') write_CMD(LCD_SETSTROKA3);
					else if (BIT_CLOCK == 0 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA1);
				}

				if (iia == 1) {
					if (BIT_CLOCK == 1 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA3);
					else if (BIT_CLOCK == 1 && kollw_sensor < 4 && wide_simvol == '1') write_CMD(LCD_SETSTROKA4);
					else if (BIT_CLOCK == 0 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA2);
				}

				if (iia == 2) {
					if (BIT_CLOCK == 1 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA4);
					else if (BIT_CLOCK == 0 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA3);
				}

				if (iia == 3) if (BIT_CLOCK == 0 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA4);

				if (ds_nik != 1) sprintf(temp_sensor_txt, "Sensor-%d: ", (iia+1));
				else read_nickname();

				if (iia == 0) t_nick = fopen("/tmp/nick", "w");
				else t_nick = fopen("/tmp/nick", "a");

				sprintf(temp_str_str, "%s\n", temp_sensor_txt);
	  			fputs(temp_str_str, t_nick); // записать "nick" в файл
	  	  		fclose (t_nick);

				if ((wide_simvol == '0') || (wide_simvol == '1')) stringout_lcd_Wide0(temp_sensor_txt);
			}

 			while(1) {
					int wdr=0;
				for (iia=0; iia<kollw_sensor; iia++) {
					memset(temp_sensor_data, ' ', sizeof(temp_sensor_data)); //очищаем
					if (iia == 0) {
						clock_plus_ds18();
						sprintf(read_file_ds18_path, "/sys/bus/w1/devices/%s/w1_slave", ds0_path);
						if (BIT_CLOCK == 1 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA2+11);
						else if (BIT_CLOCK == 1 && kollw_sensor < 4 && wide_simvol == '1') write_CMD(LCD_SETSTROKA3+11);
						else if (BIT_CLOCK == 0 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA1+11);
					}

					if (iia == 1) {
						clock_plus_ds18();
						sprintf(read_file_ds18_path, "/sys/bus/w1/devices/%s/w1_slave", ds1_path);
						if (BIT_CLOCK == 1 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA3+11);
						else if (BIT_CLOCK == 1 && kollw_sensor < 4 && wide_simvol == '1') write_CMD(LCD_SETSTROKA4+11);
						else if (BIT_CLOCK == 0 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA2+11);
						else if (BIT_CLOCK == 0 && wide_simvol == '1') { wdr=1; }
					}

					if (iia == 2) {
						clock_plus_ds18();
						sprintf(read_file_ds18_path, "/sys/bus/w1/devices/%s/w1_slave", ds2_path);
						if (BIT_CLOCK == 1 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA4+11);
						else if (BIT_CLOCK == 0 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA3+11);
					}

					if (iia == 3) {
						sprintf(read_file_ds18_path, "/sys/bus/w1/devices/%s/w1_slave", ds3_path);
						if (BIT_CLOCK == 0 && kollw_sensor < 4 && wide_simvol == '0') write_CMD(LCD_SETSTROKA4+11);
					}

					conv_w1_ds18();
/*
 if (iia == 0) {		//
 temp_sensor_data[0]= '2';	//
 temp_sensor_data[1]= '7';	//
 temp_sensor_data[2]= '.';	//
 temp_sensor_data[3]= '6';	//
 } else if (iia == 1) {		//
 temp_sensor_data[0]= '-';	//
 temp_sensor_data[1]= '4';	//
 temp_sensor_data[2]= '.';	//
 temp_sensor_data[3]= '6';	//
 }				//
 tfail = 0;			*/

	if ((tfail == 1) && ((wide_simvol == '0') || (wide_simvol == '1'))) stringout_lcd_Wide0("Nope !!!");
	else if ((tfail == 2) && ((wide_simvol == '0') || (wide_simvol == '1'))) stringout_lcd_Wide0("CRC !!");
	else 

	if ((iia == 0) && (BIT_CLOCK == 0) && (wide_simvol == '1')) { stringout_lcd_WideW1(temp_sensor_data,0,sizeof(temp_sensor_data)); }
	else if ((iia == 0) && ((wide_simvol == '0') || (wide_simvol == '1'))) stringout_lcd_Wide0(temp_sensor_data);
	else if ((wide_simvol == '2') || (wide_simvol == '4')) { stringout_lcd_WideW1(temp_sensor_data,0,sizeof(temp_sensor_data)); sleep(5); }
	else if ((iia == 1) && (BIT_CLOCK == 0) && (wdr == 1)) { 
		wide_simvol = '2'; 
		stringout_lcd_WideW1(temp_sensor_data,0,sizeof(temp_sensor_data)); 
		wide_simvol = '1'; wdr=0; 
	}
	else if ((iia == 1) && ((wide_simvol == '0') || (wide_simvol == '1'))) stringout_lcd_Wide0(temp_sensor_data);
	else if ((wide_simvol == '2') || (wide_simvol == '4')) { stringout_lcd_WideW1(temp_sensor_data,0,sizeof(temp_sensor_data)); sleep(5); }
	else if (((iia == 2) || (iia == 3)) && (wide_simvol == '0')) stringout_lcd_Wide0(temp_sensor_data);
 usleep(500000);
					if (iia == 0) t_nick = fopen("/tmp/sd", "w");
					else t_nick = fopen("/tmp/sd", "a");

					if (tfail == 1) sprintf(temp_str_str, "Nope !!\n");
					else if (tfail == 2) sprintf(temp_str_str, "CRC !!\n");
					else sprintf(temp_str_str, "%s \n", temp_sensor_data);

	  				fputs(temp_str_str, t_nick); // записать "показаний" в файл
	  				fclose (t_nick);

				}

				system("cp -f /tmp/sd /tmp/d_s");

				if (BIT_WHILE == 0) return(0);
			}
	}	// * end read gpio-w1 sensor and print LCD * //


			// *********************************************************** //
	while(1) {
		if ((BIT_CLOCK == 1) && (BIT_WHILE == 1)) {		//  clock 
			write_CMD(LCD_SETSTROKA1);	// Set cursor to stroka-1  позиция - 0
			setclock();

			if ((fl_es == 1) && (BIT_STRING == 1)) {
				if (BIT_digitemp == 2) sprintf(read_file_ds18_path, "/tmp/dig_log");
				else sprintf(read_file_ds18_path, "/tmp/%s", filename);

				read_ffile();	// 
				print_lcd();
			} else if (BIT_digitemp == 2) {
				sprintf(read_file_ds18_path, "/tmp/dig_log");
				read_ffile();	// 
				print_lcd();
			}
//			sleep(1);
			if (wide_simvol == '0') sleep(1);
			else if ((wide_simvol == '1') || (wide_simvol == '2') || (wide_simvol == '3') || (wide_simvol == '4')) usleep(500000);

		}
		else if ((fl_es == 1) && (BIT_STRING == 1)) {
			if (BIT_digitemp == 2) sprintf(read_file_ds18_path, "/tmp/dig_log");
			else sprintf(read_file_ds18_path, "/tmp/%s", filename);

			read_ffile();	// Get current file
     			print_lcd();
 			sleep(1);
		} else if (BIT_digitemp == 2) {
			sprintf(read_file_ds18_path, "/tmp/dig_log");
			read_ffile();	// 
			print_lcd();
 			sleep(1);
		}
     	} // *** end WHILE *** //
			// *********************************************************** //

    } else if (fl_es == 1) {
	sprintf(read_file_ds18_path, "/tmp/%s", filename);
    	read_ffile();	// Get current file
	print_lcd();
    }

    return(0);
}

// ***************** END Main function ***************************************** //
// **************************************************************************** //



