
/*   ТЕРМОРЕГУЛЯТОРЫ НА 4-ДАТЧИКА И ПЛЮС ЕЩЁ ТЕРМОМЕТРЫ ДО 6-ДАТЧИКОВ (ВСЕГО 10-ШТУК)
*
*  Опрос и считывание всех 10 датчиков с дискретностью 12 (точностью) занимает 1,2-1,5 секунд.
*
*    РАБОТАЕТ САМОСТОЯТЕЛЬНО, НО ЕСТЬ ВОЗМОЖНОСТЬ ПРОСМАТРИВАТЬ ПОКАЗАНИЯ И ИЗМЕНЯТЬ ВСЕ НАСТРОЙКИ ИЗ "ВЕБ-МОРДЫ РОУТЕРА"
*
*  скетч работает на ARDUINO UNO-R3-16мГц (usb ch340) конденсатор на RESET не вешал и порт работает в ОБЕ СТОРОНЫ
*  и тестировал с ARDUINO-PRO-MINI-16мГц через pl2303 
*/

/*  СЕРИЙНЫЕ НОМЕРА ДАТЧИКОВ ПРОГРАМА ПРОСКАНИРУЕТ И СОХРАНИТ САМА, ТОЛЬКО НАДО:
*  НАЖАТЬ И УДЕРЖИВАТЬ ОДНОВРЕМЕННО ДВЕ КНОПКИ "buttonMenu" И "buttonPLUS", ПОДАТЬ ПИТАНИЕ, ПОДОЖДАТЬ 5-6 СЕК
*  И ОТПУСТИТЬ КНОПКИ "buttonMenu" И "buttonPLUS" 
*  
*       ТЕРМО-РЕГУЛЯТОР: 
*  ПРИ ВЫХОДЕ ИЗ СТРОЯ ОДНОГО ДАТЧИКА: ЕГО ВЫХОД ДЛЯ УПРАВЛЕНИЯ СИЛОВЫМИ КЛЮЧАМИ ОТКЛЮЧИТСЯ
*  И МЕСТАМИ С ДРУГИМИ ДАТЧИКАМИ НЕ ПОМЕНЯЕТСЯ.
*  ПОСЛЕ ЗАМЕНЫ ДАТЧИКА НАСТРОЙКИ НАДО БУДЕТ ПРОИЗВОДИТЬ С НУЛЯ.
*  
*       ИЗ "MENU АРДУИНЫ" УСТАНАВЛИВАЕТСЯ:
* 1. РАЗДЕЛЬНО НА КАЖДОМ ДАТЧИКЕ ТЕМПЕРАТУРА ОТКЛЮЧЕНИЯ, ГИСТЕРЕЗИС И ТИП(ТЕРМОРЕГУЛЯТОР ИЛИ ПРОСТО ТЕРМОМЕТР) 
*    И ИСПОЛЬЗУЕМЫЙ LCD (НА ОДНУ, ПОСЛЕ ИЗМЕНЕНИЯ НАСТРОЕК ДЛЯ LCD БУДЕТ ПЕРЕЗАГРУЗКА ПРОГРАММЫ
*
*  ИЗ "ВЕБ-МОРДЫ РОУТЕРА" БУДЕТ НА МНОГО БОЛЬШЕ ВОЗМОЖНОСТЕЙ В НАСТРОЙКАХ. И ТАК ЖЕ ПРОСМОТР ТЕМПЕРАТУРЫ ОНЛАЙН
*
*       АВАРИЙНОЕ ВОССТАНОВЛЕНИЕ РАБОТОСПОСОБНОСТИ UART-232 И ВСЕХ НАСТРОЕК
* НАЖАТЬ И УДЕРЖИВАТЬ КНОПКУ "buttonPLUS" ПОДАТЬ ПИТАНИЕ, ПОДОЖДАТЬ 5-6 СЕК И ОТПУСТИТЬ КНОПКУ "buttonPLUS" 
*   НАЖАТАЯ КНОПКА "buttonPLUS" Reset to defaults: ВСЕХ НАСТРОЕК И ЗАПИШЕТ speed=115200;
* ИЛИ ПРИ ВКЛЮЧЕННОМ ПИТАНИИ НАЖАТЬ И УДЕРЖИВАТЬ КНОПКУ "buttonPLUS" НАЖАТЬ И ОТПУСТИТЬ КНОПКУ "RESET" НА ПЛАТЕ,
* ПОДОЖДАТЬ 5-6 СЕК И ОТПУСТИТЬ КНОПКУ "buttonPLUS" 
*
* НАЖАТАЯ КНОПКА "buttonMinus" Reset to defaults: ВСЕХ НАСТРОЕК И ЗАПИШЕТ speed=57600;
* НАЖАТЫЕ ВМЕСТЕ КНОПКИ "buttonPLUS" И "buttonMinus" Reset to defaults: ВСЕХ НАСТРОЕК И ЗАПИШЕТ speed=9600;
*/

/*********************************************************
*   EEPROM TABLE:
* EEPROM 0-15  ALLAR-HISTER              - 4шт
* EEPROM 16-19  TIP Termostat - kondicioner          - 4шт
* EEPROM 20-23  PIN ARDUINO OUT для управления силовымиключами термостата    - 4шт
* EEPROM 24-27  активный_уровень_gpio-выхода 0 ИЛИ 1         - 4шт
*
* EEPROM c 28 по 95 - свободно
*
* EEPROM 96-97  SPEED
* EEPROM 98-99  LCD
* EEPROM 100-180  СЕРИЙНЫЕ НОМЕРА 10-ти ДАТЧИКОВ 
* EEPROM 181-311  НАЗВАНИЕ 10 - ДАТЧИКОВ по 13-символов (лат)      - 10шт
* EEPROM 312-366  НАЗВАНИЕ ПРИБОРА (кирилица 27-символa a латынь 54-символa)
*
* EEPROM c 367 по 509 - свободно
*
* EEPROM 510  i2c_adres lcd-pcf8574 (HEX) https://github.com/marcoschwartz/LiquidCrystal_I2C
* EEPROM 511  W1 - PIN ARDUINO
* EEPROM 512  byt defaults
***********************************************************
*/

#include <OneWire.h>    // DS18
#include <Wire.h>               // I2C
#include <LiquidCrystal_I2C.h>  // LCD - https://github.com/marcoschwartz/LiquidCrystal_I2C
#include <EEPROM.h>
#include <avr/wdt.h>

/* ***************************************************************************** */

/* ***************************************************************************** */
/* ПЕРЕД << ПЕРВОЙ >> ПРОШИВКОЙ ARDUINO СМЕНИТЬ НА СВОИ gpio - выводы, cкорость порта и используемый LCD 
*  сразу же, и только после ПЕРВОЙ прошивки, эти данные запишутся ЕДИНО-РАЗОВО во флэш-память ардуины 
*  cледующие пере-прошивки эти данные записывать во флэш-память не будут!!
*  3аписать во флэш-память по новой возможно только через *АВАРИЙНОЕ ВОССТАНОВЛЕНИЕ РАБОТОСПОСОБНОСТИ UART-232 И ВСЕХ НАСТРОЕК*
*  Это будет типа сброса всех настроек (default) */

    /* gpio- выходы */
#define outPin1   5
#define outPin2   6
#define outPin3   7
#define outPin4   8

    /* W1 gpio- вход-выход */
#define W1Pin   11

    /* cкорость порта */
#define defaultSpeed  115200

    /* LCD 16x2 */
#define i2c_adres 0x27
#define lcd16xx   16
#define lcdxxx2   2
/* ***************************************************************************** */
/* ***************************************************************************** */

#define termometr 0
#define termoStat 1
#define kondicioner 2
#define pompa_Kotel 3

#define invers_LOW  0
#define invers_HIGH 1

#define eei2c_adres EEPROM.read(510)
#define w1_gpio   EEPROM.read(511)
#define lcd_symvolov  EEPROM.read(98)
#define UPDATE_TIME  1000 // обращаемся к датчикам раз в 1000 мс для одновременного старта температурного преобразования

byte  macAdrDs[80]={},
  tMac[80]={},

  macDs[8]={},
  inversByt[4]={},
  kollSensor=0,

  outGpio[4]={},  /*  */
  typT[4]={}, /*  */

  buttonMenu = 2, //кнопка пин 2
  buttonPLUS = 3, //кнопка пин 3
  buttonMinus = 4; //кнопка пин 4

int alAlarm[4]={},  /*  */
  histeresis[4]={}, /*  */
  whil=0,
  tmpT,
  bit_ok_crc_ds=0,
  posic,
  kollStrokLcd = EEPROM.read(99);

//long  tmpCelsiusDS18x20;
int16_t tmpCelsiusDS18x20;
unsigned long LastUpdateTime = 0;
float celsius;

/*******************************************************************************/

/*******************************************************************************/
 LiquidCrystal_I2C lcd(eei2c_adres, lcd_symvolov, kollStrokLcd);
 OneWire ds(w1_gpio);
 void setup(){
    wdt_disable();
  Wire.begin();
  lcd.init();
  lcd.clear();
  lcd.noBacklight(); // backlight - вкл. подсветку LCD (noBacklight - выкл)
  pinMode(buttonMenu,INPUT_PULLUP);
  pinMode(buttonPLUS,INPUT_PULLUP);
  pinMode(buttonMinus,INPUT_PULLUP);
   if (EEPROM.read(512)!=0) reset_defaults(115200); //только после ПЕРВОЙ прошивки ЕДИНО-РАЗОВО reset_defaults

   if (digitalRead(buttonMenu) == HIGH) {
    if ((digitalRead (buttonPLUS) == LOW) && (digitalRead (buttonMinus) == HIGH)) reset_defaults(115200); // Reset to defaults:
    else if ((digitalRead (buttonPLUS) == HIGH) && (digitalRead (buttonMinus) == LOW)) reset_defaults(57600);
    else if ((digitalRead (buttonPLUS) == LOW) && (digitalRead (buttonMinus) == LOW)) reset_defaults(9600);
   }

  if (lcd_symvolov == 20) posic = 4;
  else posic=0;
  unsigned int cAdr[2]={};
  cAdr[0] = EEPROM.read(96);
  cAdr[1] = (cAdr[0] << 8)&0xffffff00;
  cAdr[0] = EEPROM.read(97);
  cAdr[0]+=cAdr[1]; /* ВОССТАНАВЛИВАЕМ ЧИСЛО "speed" СЧИТАННОЕ ИЗ 1-БАЙТОВОГО EEPROM */
  unsigned long speed = cAdr[0];
    speed*=400;

  Serial.begin(speed);

  readMacEeprom(0);

  int st=16;
  for (int i=0; i<4; i++) {
    typT[i] = EEPROM.read(st);;
    st++;
  }
  for (int i=0; i<4; i++) {
    outGpio[i]=EEPROM.read(st);
    st++;
  }
  for (int i=0; i<4; i++) {
    inversByt[i]=EEPROM.read(st);
    st++;
  }

  lcd.init();
  lcd.clear();
  lcd.backlight(); // backlight
     if ((digitalRead(buttonMenu) == LOW) && (digitalRead(buttonPLUS) == LOW)) {
  poiskRom(0);
  if (kollSensor == 0) lcdInfoTx(1);  /* PRINT LCD "ERROR no Sensor" */
  else {
    lcdInfoTx(0);     /* PRINT LCD "SEARCH SENSOR" */
    lcdInfoTx(2);     /* PRINT LCD "FOUND: ?-pieces"  */
    lcdInfoTx(3);     /* PRINT LCD "SAVED MAC" */
      delay(100);
    poiskRom(1);
  }
    while ((digitalRead(buttonMenu) == LOW) && (digitalRead(buttonPLUS) == LOW)) { delay(50); wdt_reset(); }
    lcdInfoTx(5);     /* PRINT LCD "REBOOT" */
    asm volatile ("jmp 0x0000"); /* REBOOT */
     }
     else {
  lcdInfoTx(0);       /* PRINT LCD "SEARCH SENSOR" */
  poiskRom(0);
  if (kollSensor == 0) lcdInfoTx(1);  /* PRINT LCD "ERROR no Sensor" */
  else lcdInfoTx(2);      /* PRINT LCD "FOUND: ?-pieces"  */
      }

    pinOutOff(1); // инициализация gpio выходов начальная ТОЛЬКО НУЖНЫЕ ДЛЯ ТЕРМОСТАТА

     temperatureConv();
     delay(750);
     temperatureConv();
     wdt_enable (WDTO_4S); // Для тестов не рекомендуется устанавливать значение менее 8 сек.
     lcd.clear();
}
/*******************************************************************************/
/*******************************************************************************/

void loop(){
     if (whil==0) proga(0);
     else pinOutOff(1);   // переинициализация gpio после настроек полученных по UART

     if ((digitalRead(buttonMenu) == LOW) && (digitalRead(buttonPLUS) == LOW)) {
  pinOutOff(0);   // отключение всех gpio перед входом в меню кнопками
  lcdInfoTx(6);   /* PRINT LCD "MENU SETUP" */
  while ((digitalRead(buttonMenu) == LOW) || (digitalRead(buttonPLUS) == LOW)) { delay(50); wdt_reset(); }
  selectMenu(); delay(500); wdt_reset();
     }
     seriallUsbReadd();
     wdt_reset();
}
/*******************************************************************************/

/*******************************************************************************/
void macRead(int i) {
     int st=0;
     st=st+8*i;
     for (int i = 0; i < 8; i++){ macDs[i] = macAdrDs[st]; st++; }
}
/*******************************************************************************/

/*******************************************************************************/
void proga(int c) {

     temperatureConv(); /* даем команду пропуска ROM, для одновременного старта температурного преобразования */
     for (int i=0; i < kollStrokLcd; i++) {

  macRead(i);
  test_crc_ds(i);
  if (bitRead(bit_ok_crc_ds, i)==1) readSensor(macDs);
  test_crc_ds(i);

  if ((typT[i] != termometr) && (i<4)) {
       if (bitRead(bit_ok_crc_ds, i)==1) thermoStat(i); // TERMOSTAT   
       /* ПРИ ОТВАЛЕ ДАТЧИКА АВАРИЙНОЕ ОТКЛЮЧЕНИЕ ЕГО gpio ВЫХОДА, ТОЛЬКО ДЛЯ ТЕРМОСТАТА */       
       else if (inversByt[i] == invers_LOW) digitalWrite(outGpio[i], HIGH);
       else if (inversByt[i] == invers_HIGH) digitalWrite(outGpio[i], LOW);
       /* ****************************************************************************** */
  }

  if (i<4) printForLcd(bitRead(bit_ok_crc_ds, i), i); // На LCD

  if (c==1) {           // На Serial
    if (bitRead(bit_ok_crc_ds, i)==1) Serial.print(celsius, 1);
    else Serial.print("Err-CRC");
    Serial.print("\t");
  }
     }
     if (c==1) Serial.print("\n");
}
/*******************************************************************************/

/*******************************************************************************/
void selectMenu() {
int count_bitMenu=0, flagDs=0;
    for (int i=0; i < 4; i++) readSetupEeprom(0, i);
  lcd.clear();
    while (count_bitMenu < 13) {
  int i, d, c;
  if ((count_bitMenu==0) || (count_bitMenu==1) || (count_bitMenu==2)) i=0;
  if ((count_bitMenu==3) || (count_bitMenu==4) || (count_bitMenu==5)) i=1;
  if ((count_bitMenu==6) || (count_bitMenu==7) || (count_bitMenu==8)) i=2;
  if ((count_bitMenu==9) || (count_bitMenu==10) || (count_bitMenu==11)) i=3;

//        -------------------------------------------           //

  lcd.setCursor(0, 0);
      if (count_bitMenu != 12) {
    lcd.print("d:");
    lcd.print(d = i + 1);
      }

      if ((count_bitMenu == 0) || (count_bitMenu == 3) || (count_bitMenu == 6) || (count_bitMenu == 9)) {
    c=3; 
    if (typT[i]==termometr) lcd.print(" Termometr       ");    // ТИП ПРИБОРА Termometr
    else if (typT[i]==termoStat) lcd.print(" TermoStat       ");   // ТИП ПРИБОРА TermoStat
    else if (typT[i]==kondicioner) lcd.print(" Kondicioner     ");   // ТИП ПРИБОРА Kondicioner
    else if (typT[i]==pompa_Kotel) lcd.print(" Pompa-Kotel     ");   // ТИП ПРИБОРА Pompa-Kotel
//    else if (typT[i]==4) lcd.print(" ???????????     "); 
      } else if ((count_bitMenu == 1) || (count_bitMenu == 4) || (count_bitMenu == 7) || (count_bitMenu == 10)) { 
    c=0; 
    lcd.print(" off-on:"); 
    lcdprynt(alAlarm[i], c);
      } else if ((count_bitMenu == 2) || (count_bitMenu == 5) || (count_bitMenu == 8) || (count_bitMenu == 11)) { 
    c=1; 
    lcd.print(" Hister:"); 
    lcdprynt(histeresis[i], c);
      } else if (count_bitMenu == 12) { 
    c=2;
    lcdprynt(kollStrokLcd, c);
      }

      if ((digitalRead(buttonMinus)==LOW) || (digitalRead(buttonPLUS)==LOW)) {
    if ((c==0) && (typT[i]!=termometr)) {
      setAlarHist(alAlarm[i], c); // +- Allarm
      alAlarm[i]=tmpT;
      flagDs=1;
    } else if ((c==1) && (typT[i]!=termometr)) {
      setAlarHist(histeresis[i], c); // +- Histeresis
      histeresis[i]=tmpT;
      flagDs=2;
    } else if (c==2) {
      setAlarHist(kollStrokLcd, c); // +- kollStrokLcd
      kollStrokLcd=tmpT;
      flagDs=3;
    } else if (c==3) {
      if ((digitalRead(buttonMinus)==LOW) || (digitalRead(buttonPLUS)==LOW)) {
        if (tmpT < 3) tmpT += 1;  // 3 - ЭТО 4 ТИПА ПРИБОРА 
        else if (tmpT >= 3) tmpT = 0; // 3 - ЭТО 4 ТИПА ПРИБОРА 
      }
      while ((digitalRead(buttonMinus)==LOW) || (digitalRead(buttonPLUS)==LOW)) { delay(50);  wdt_reset(); }
      flagDs=4;
      typT[i]=tmpT;
    }
        }
//        -------------------------------------------           //

  if (digitalRead (buttonMenu) == LOW) {
      lcd.clear();
      lcd.setCursor(0, 0); 
    if (flagDs==1) {
      writeAlarmOffEeprom(i, c);
      lcd.print("S:");
        int b = i+1;
      lcd.print(b);
      lcd.print(" SAVE Alar OK");
    }
    else if (flagDs==2) {
      writeAlarmOffEeprom(i, c);
      lcd.print("S:");
        int b = i+1;
      lcd.print(b);
      lcd.print(" SAVE Hist OK");
    }
    else if (flagDs==3) {
      eepromWrite(kollStrokLcd, 99);
      lcd.print("Save LCD wn"); 
      lcd.print(lcd_symvolov); 
      lcd.print("x"); 
      lcd.print(kollStrokLcd); 
      if (kollStrokLcd > 1) lcd.setCursor(4, 1);
      else {
          delay(1500);
        lcd.clear();
        lcd.setCursor(4, 0); 
      }
      lcd.print("&& Apply"); 
        delay(2000);
      asm volatile ("jmp 0x0000"); /* REBOOT */
    }
    else if (flagDs==4) {
      writeTipEeprom(i);
      lcd.setCursor(6, 0); 
      lcd.print("Save"); 
    }
    while (digitalRead (buttonMenu) == LOW) { delay(250); wdt_reset(); }/* ждем отпускания кнопки */

    if (typT[i]!=termometr) count_bitMenu++;
    else if (typT[i]==termometr) count_bitMenu+=3;

    if (flagDs!=0) { 
      delay(1500);
      flagDs=0;
      pinOutOff(1); // переинициализация gpio после настроек из меню кнопками
    }

    lcd.clear();
    delay(50);
  }
   wdt_reset();
    }
   wdt_reset();
}
/*******************************************************************************/

/*******************************************************************************/

void setAlarHist(int i, int c) {  // УСТАНОВКА ТЕМПЕРАТУРЫ ОТКЛЮЧЕНИЯ И Histeresis BКЛЮЧЕНИЯ
   while ((digitalRead(buttonPLUS)==LOW) || (digitalRead(buttonMinus)==LOW)) {
  if (digitalRead(buttonPLUS)==LOW) {
       if ((c==0) || (c==1)) {
    if (c == 0) { if (i < 1000) i+=1; else i=1; }
    else if (c == 1) { if (i < 100) i+=1; else i=1; }

       } else if (c==2) { if (i<10) i+=1; else i=1; }
  }
  else if (digitalRead(buttonMinus)==LOW) {
       if ((c==0) || (c==1)) {
    if (c == 0) { if (i > 0 ) i-=1; else i=1000; }
    else if (c == 1) { if (i > 0) i-=1; else i=100; }

       } else if (c==2) { if (i > 1) i-=1; else i=10; }
  }
  lcdprynt(i, c);
  if (c==2) while ((digitalRead(buttonPLUS)==LOW) || (digitalRead(buttonMinus)==LOW)) { delay(50); wdt_reset(); }
  wdt_reset();
   }
   tmpT=i;
}
void lcdprynt(float i, int c) {
     if (c==2){ lcd.setCursor(0, 0); lcd.print("LCD wn"); lcd.print(lcd_symvolov); 
  lcd.print("x"); lcd.print(i, 0); lcd.print("  "); delay(250); }
     else { i/=10.0;
  lcd.setCursor(11, 0); lcd.print(i, 1); lcd.print("  "); delay(50); }
}
/*******************************************************************************/

/*******************************************************************************/
void writeTipEeprom(int i) {
int c=i+16;
    eepromWrite(typT[i], c);
}
/*******************************************************************************/

/*******************************************************************************/
void writeAlarmOffEeprom(int i, int a){ /* i - это номер sensor для которго сохраняем настройки в EEPROM */
         /* c - это адрес в EEPROM куда схраняем */
/* |Sensor | Alarm | Hister |
 * |       | hb lb | hb  lb |
 * |   0   | 0  1  | 2   3  |
 * |   1   | 4  5  | 6   7  |
 * |   2   | 8  9  | 10  11 |
 * |   3   | 12 13 | 14  15 | */
int c, tSet, sSet;
    if (a==0) {
  tSet=alAlarm[i];
  if (i==0) c=i;
  else if (i==1) c=4;
  else if (i==2) c=8;
  else if (i==3) c=12;
  readSetupEeprom(0, i);
  sSet=alAlarm[i];
    } else if (a==1) {
  tSet=histeresis[i];
  if (i==0) c=2;
  else if (i==1) c=6;
  else if (i==2) c=10;
  else if (i==3) c=14;
  readSetupEeprom(0, i);
  sSet=histeresis[i];
    }

    if (tSet != sSet) {
     /* побитный сдвиг вправо на 8 и побитное 'И' с 0x00FF, готовим 2-байт число для записи в 1-байтовый EEPROM */
  int lb=tSet&0x00ff, hb=(tSet>>8)&0x00ff;
      eepromWrite(hb, c);; c++; /* Старший байт */
      eepromWrite(lb, c);; c++; /* Младший байт */
    }
}
/*******************************************************************************/

/*******************************************************************************/
void printForLcd(int c, int i) {
    int klvst=9;
    if (posic==4) klvst+=4;
    lcd.setCursor(0, i);
    char tln[klvst];
    int st=181+13*i;
    for (byte ii=0; ii<klvst; ii++) {
  tln[ii]=EEPROM.read(st);
  lcd.print(tln[ii]);  /* отображение названия датчика */
  st++;
    }

    if (posic==4) lcd.print(" ");
    if (posic==0) lcd.setCursor(10, i);
    else if (posic==4) lcd.setCursor(14, i);
    if (c==1) {
  if (celsius>=0) lcd.print(" ");
  lcd.print(celsius, 1); /* отображение температуры */
  lcd.print("C");
  if ((celsius<10) && (posic!=4)) lcd.print(" ");
    } else lcd.print("Er-CRC");
}
/*******************************************************************************/

/*******************************************************************************/
void readSensor(byte *a) { /* Считывание температуры */
 byte  data[2];
    ds.reset();
    ds.select(a);
    ds.write(0xBE); // Read Scratchpad
    data[0] = ds.read();
    data[1] = ds.read();
    tmpCelsiusDS18x20 = (data[1] << 8) | data[0];
    celsius = tmpCelsiusDS18x20 / 16.0; // Переводим в температуру
}
/*******************************************************************************/

/*******************************************************************************/
void pinOutOff(int a) { // выключает выходы
  for (byte i = 0; i < 4; i++) {
    if ((typT[i] == 1) || (typT[i] == 2) || (typT[i] == 3) || (typT[i] == 4)) {
  if (a == 0) {   // отключение gpio выходов аварийное и при входе в меню с кнопок
      if (inversByt[i] == invers_LOW) digitalWrite(outGpio[i], HIGH);
      else if (inversByt[i] == invers_HIGH) digitalWrite(outGpio[i], LOW);
  } else if (a == 1) {  // инициализация gpio выходов (начальная и после установок) ТОЛЬКО НУЖНЫЕ ДЛЯ ТЕРМОСТАТА
      if (inversByt[i] == invers_LOW) pinMode(outGpio[i], HIGH);    // перед инициализацией выходов, что бы не сработал 
      else if (inversByt[i] == invers_HIGH) pinMode(outGpio[i], LOW); // перед инициализацией выходов, что бы не сработал 
      pinMode(outGpio[i], OUTPUT);        // делаем gpio выходами
  }
    } else if (typT[i] == 0) pinMode(outGpio[i], INPUT);
  }
}
/*******************************************************************************/

/*******************************************************************************/
/* TERMOSTAT */
void thermoStat(int i) {
   readSetupEeprom(0, i);
   byte bh, ch, dr=digitalRead(outGpio[i]);
   int tmp=alAlarm[i], tmp1 = (tmpCelsiusDS18x20 / 16.0) *10;

   if (typT[i] == termoStat) {
       /* **************** termostat ********************** */
  if (inversByt[i] == invers_HIGH) { bh = HIGH; ch = LOW; }
  else if (inversByt[i] == invers_LOW) { bh = LOW; ch = HIGH;}
       /* ******** kondicioner - pompa_Kotel ************** */
   } else if ((typT[i] == kondicioner) || (typT[i] == pompa_Kotel)) {
      if ((tmp1 <= 50) && (typT[i] == pompa_Kotel)) {
       /* менее +5,0 градусов "pompa_Kotel" из kondicioner переходим в термостат. */
  if (inversByt[i] == invers_HIGH) { bh = HIGH; ch = LOW; }
  else if (inversByt[i] == invers_LOW) { bh = LOW; ch = HIGH;}
      }
      else {  /* ********** kondicioner ************** */
  if (inversByt[i] == invers_LOW) { bh = HIGH; ch = LOW; }
  else if (inversByt[i] == invers_HIGH) { bh = LOW; ch = HIGH;}
      }
       /* ************************************************* */
   }

   if (dr == bh) {
  if (tmp1 < tmp) digitalWrite(outGpio[i], bh);
  else digitalWrite(outGpio[i], ch);
   } else {
  if (tmp1 <= (tmp-=histeresis[i])) digitalWrite(outGpio[i], bh);
  else digitalWrite(outGpio[i], ch);
   }
}
/*******************************************************************************/

/*******************************************************************************/
/* даем команду пропуска ROM, для ОДНОВРЕМЕННОГО старта температурного преобразования ВСЕХ ДАТЧИКОВ */
/* УВЕЛИЧИВАЕТСЯ БЫСТРОДЕЙСТВИЕ ПРОГРАММЫ ПРИ ИСПОЛЬЗОВАНИИ БОЛЬШЕ ОДНОГО ДАТЧИКА. ВСЕ 4-СЕНСОРА С  */
/* РАЗРЕШЕНИЕМ В 12-БИТ ОПРАШИВАЮТСЯ ГДЕ ТО ЗА 1,2-1,5 СЕКУНДЫ ВМЕСТО 3,5-4 СЕКУНДЫ */
void temperatureConv(){
    if (millis() - LastUpdateTime > UPDATE_TIME){ // обращаемся к датчикам раз в 1000 мс
  LastUpdateTime = millis();
  ds.reset();
  ds.write(0xCC); // даем команду пропуска ROM, для одновременно старта температурного преобразования
  ds.write(0x44); // старт температурного преобразования, нормальное питание
   }
}
/*******************************************************************************/

/*******************************************************************************/
/* чтение из памяти, ранее записанных установок температуры */
void readSetupEeprom(int a, int i){ 

/* |Sensor | Alarm | Hister |
 * |       | hb lb | hb  lb |
 * |   0   | 0  1  | 2   3  |
 * |   1   | 4  5  | 6   7  |
 * |   2   | 8  9  | 10  11 |
 * |   3   | 12 13 | 14  15 | */

  int tSet[1], c, l;
  if (i==0) { c=i, l=1; }
  else if (i==1) { c=4, l=1; }
  else if (i==2) { c=8, l=1; }
  else if (i==3) { c=12, l=1; }
  else if (i==4) { i=0; c=0, l=4; }
  for (int g=0; g<l; g++) {
  if (a==1) {
      if (g==0) Serial.print("T");
      else Serial.print("\tT");
      Serial.print(g);
      Serial.print(":");
  }
  tSet[1] = EEPROM.read(c);; c++;
  alAlarm[i] = tSet[1] << 8;
  tSet[1] = EEPROM.read(c);; c++;
  alAlarm[i]+=tSet[1];
  if (a==1) { Serial.print(alAlarm[i]); Serial.print(":"); }

  tSet[1] = EEPROM.read(c);; c++;
  histeresis[i] = tSet[1] << 8;
  tSet[1] = EEPROM.read(c);; c++;
  histeresis[i]+=tSet[1];
  if (a==1) { Serial.print(histeresis[i]); }
  }
  if ((a==1) && (l==4)) Serial.print("\t");

    if (a==1) {
  Serial.print("P:"); for (int i=16; i<20; i++) { Serial.print(EEPROM.read(i)); if (i<19) Serial.print(":"); else Serial.print("\t"); }
  Serial.print("Gpi:"); for (int i=20; i<24; i++) { Serial.print(EEPROM.read(i)); if (i<23) Serial.print(":"); else Serial.print("\t"); }
  Serial.print("Inv:"); for (int i=24; i<28; i++) { Serial.print(EEPROM.read(i)); if (i<27) Serial.print(":"); else Serial.print("\t"); }
//  Serial.print("kond:"); for (int i=28; i<32; i++) { Serial.print(EEPROM.read(i)); if (i<31) Serial.print(":"); else Serial.print("\t"); }
  Serial.print("Bau:"); convToSPEED(96); Serial.print("\t"); 
  Serial.print("LCD:"); for (int i=98; i<100; i++) { Serial.print(EEPROM.read(i)); if (i<99) Serial.print(":"); else Serial.print("\t"); }
  Serial.print("1Wr:"); Serial.print(EEPROM.read(511)); Serial.print("\t");
  Serial.print("i2c:"); Serial.print(EEPROM.read(510), HEX); Serial.print("\n");
    }

    if ((alAlarm[i] > 1000) || (alAlarm[i] < 0)) { alAlarm[i] = 0; writeAlarmOffEeprom(i, 0); }
    if ((histeresis[i] > (100)) || (histeresis[i] < 0)) { histeresis[i] = 1; writeAlarmOffEeprom(i, 1); }
}
/*******************************************************************************/

/*******************************************************************************/
/* чтение из памяти, ранее записанных туда ID-устройств */
void readMacEeprom(int a){ 
   int c = 100, b=0;
     for (int i = 0; i < 80; i++) {
  macAdrDs[i] = EEPROM.read(c);
  c++;
  if (a==1) {
    if ((i==0) || (i==8) || (i==16) || (i==24) || (i==32) || (i==40) ||
      (i==48) || (i==56) || (i==64) || (i==72)) { Serial.print(b++); Serial.print(":"); }
    serMacr(macAdrDs[i], a);
    if ((i==7) || (i==15) || (i==23) || (i==31) || (i==39) || (i==47) ||
          (i==55) || (i==63) || (i==71) || (i==79)) Serial.print("\t");
  }
     }
     if (a==1) Serial.print("\n"); /* END PRINT UART */
}
void serMacr(int c, int a){ 
     if (a==1) {
  if (c<15) Serial.print(0);
  Serial.print(c, HEX);
     }
     wdt_reset();
}
/*******************************************************************************/

/*******************************************************************************/
/* Сканирование шины 1W, обнаружение ID-устройств и их кличества */
void poiskRom(int c) {
  byte tmpMac[8]={};    // init+очищаем
  memset(tMac, 0, sizeof(tMac));  //очищаем
  int h=0;
  for (int i = 0; i < 10; i++) {
    if (!ds.search(tmpMac)) {
    if ((c == 0) || (c == 1)) kollSensor = i;
    ds.reset_search();
    return;
     }
     if (c == 1) {
  wdt_reset();
  int st=100;
  st=st+8*i;
  for (int ii = 0; ii < 8; ii++) { eepromWrite(tmpMac[ii], st); st++; }
   wdt_reset();
     } else if (c == 2) {
  for (int ii = 0; ii < 8; ii++) { tMac[h] = tmpMac[ii]; h++; }
  wdt_reset();
     }
  }
}
/*******************************************************************************/

/*******************************************************************************/
void test_crc_ds(int c) {
  bitClear(bit_ok_crc_ds, c);
  int a=0, h=0;
  macRead(c); /* macDs[i] */
  poiskRom(2);
  for(int i = 0; i < 8; i++) {
    if (tMac[h] == macDs[i]) a++;
    else { i=0xFFFF; a=0;
      if (h<8) h=7; else if (h<16) h=15; else if (h<24) h=23; else if (h<32) h=31;
        else if (h<40) h=39; else if (h<48) h=47; else if (h<56) h=55;
          else if (h<64) h=63; else if (h<72) h=71; else i=8;
    } h++;
  }
  if (a == 8) bitSet(bit_ok_crc_ds, c);
  wdt_reset();
}
/*******************************************************************************/

/*******************************************************************************/
void lcdInfoTx(int i) {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (i==0) lcd.print("SEARCH SENSOR");
  if (i==1) lcd.print("ERROR no Sensor");
  else if (i==2) {
    lcd.print("FOUND: ");
    lcd.print(kollSensor);
    lcd.print("-pieces");
  }
  else if (i==3) {
    lcd.print("SEARCH MAC OK");
      delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("SAVED MAC EEPROM");
  }
  else if (i==4) lcd.print("Reset to defauls OK");
  else if (i==5) lcd.print("/*   REBOOT   */");
  else if (i==6) lcd.print("    MENU SETUP");
  delay(1000);
  wdt_reset();
}
/*********************************************************************************************/
void convToSPEED(int c) {
    unsigned int tSet = EEPROM.read(c);; c++;
    unsigned long uSet = (tSet << 8)&0xffffff00;
  tSet = EEPROM.read(c);
  uSet+=tSet;
  Serial.print(uSet*400);
}
/*********************************************************************************************/

/*********************************************************************************************/
void seriallUsbReadd() {
  whil=0;
  if (Serial.available()>0) {
//  if ((Serial.available()>6) && (Serial.available()<65)) {

  int inKluc = Serial.read();

     if (inKluc == 'r') { // проверяем первый символ, если это 'r', то продолжаем принимать, если нет, то выходим из цикла чтения
  wdt_reset();
        
  if((Serial.read() == '+') && (Serial.read() == '=') && (Serial.read() == 's') && (Serial.read() == 'e') && (Serial.read() == 't')) { // проверяем 'r+=set'

    switch (Serial.read()) {
      case 'T': /* C ARDUINO ОТПРАВКА ПОКАЗАНИИ ТЕМПЕРАТУРЫ */
          /* echo r+=setT/ > /dev/ttyUSB0 */
        proga(1);
      break;

      case 'S': /* C ARDUINO ОТПРАВКА УСТАНОВОК allarm-hister, GPIO, typ, Invers-GPIO, "SPEED", "LCD", "1wr" */
          /* echo r+=setS/ > /dev/ttyUSB0 */
        readSetupEeprom(1, 4);
      break;

      case 'M': /* C ARDUINO ОТПРАВКА MAK-HEX */
          /* echo r+=setM/ > /dev/ttyUSB0 */
        readMacEeprom(1);
      break;

      case 'N': /* C ARDUINO ОТПРАВКА НАЗВАНИЯ ДАТЧИКOB 12-символов (лат) */
          /* echo r+=setN/ > /dev/ttyUSB0 */
        for (int i=0; i<10; i++) {
          int strn=181+13*i; Serial.print(i); Serial.print(":");
             for (int ii=0; ii<13; ii++) {
            char ly = EEPROM.read(strn);
            Serial.print(ly);
            if (ii==12) Serial.print("\t");
            strn++;
             }
        }
        Serial.print("\n");
      break;

      case 'R': /* C ARDUINO ОТПРАВКА "OK" */
          /* echo r+=setR/ > /dev/ttyUSB0 */
        Serial.print("OK\n");
      break;

      case 'D': /* C ARDUINO ОТПРАВКА НАЗВАНИЯ ПРИБОРА 
        (кирилица 27-символa a латынь 54-символa) EEPROM 312-366 */
          /* echo r+=setD > /dev/ttyUSB0 */
        wdt_reset();
        Serial.print("Nick:");
        for (int i=312; i<367; i++) {
          char ly = EEPROM.read(i);
          Serial.print(ly);
        }
          Serial.print("\n");
      break;

    } /* END switch */

  }  /* END проверки 'r+=set' */

     } /* END Serial.read()=='r' */
/*
echo r+=setT > /dev/ttyUSB0  # C ARDUINO ОТПРАВКА ПОКАЗАНИИ ТЕМПЕРАТУРЫ #
sleep 1
cat /tmp/arduino
echo r+=setD > /dev/ttyUSB0   # C ARDUINO ОТПРАВКА НАЗВАНИЯ ПРИБОРА (кирилица 27-символa a латынь 54-символa) EEPROM 312-366
sleep 1
cat /tmp/arduino
echo r+=setS > /dev/ttyUSB0  # C ARDUINO ОТПРАВКА УСТАНОВОК allarm-hister #
sleep 1
cat /tmp/arduino
echo r+=setN > /dev/ttyUSB0  # C ARDUINO ОТПРАВКА НАЗВАНИЕ ДАТЧИКА 12-символов (лат) #
sleep 1
cat /tmp/arduino
echo r+=setM > /dev/ttyUSB0  # C ARDUINO ОТПРАВКА MAK-HEX #
sleep 1
cat /tmp/arduino
echo r+=setT > /dev/ttyUSB0  # C ARDUINO ОТПРАВКА ПОКАЗАНИИ ТЕМПЕРАТУРЫ #
sleep 1
cat /tmp/arduino
 */


     else if ((inKluc == 'a') || (inKluc == 'b')
  || (inKluc == 'c') || (inKluc == 'd') || (inKluc == 'f')) { /* проверяем первый символ */ 

  wdt_reset();
  if((Serial.read() == '+') && (Serial.read() == '=')) { // проверяем '+='
      int pol = Serial.parseInt(); //read int or parseFloat for ..float...

    int tSet, st;
    byte macAdr[8]={};

    if ((inKluc=='a') && (pol == 0)) {
    /* echo a+=0 115200 16 04 11 27/ > /dev/ttyUSB0 */
      /* "speed" В ARDUINO ДЛЯ EEPROM */
        Serial.read();
      whil++;
      unsigned long speed;
        speed = Serial.parseInt();
        speedSave(speed);

      /* "LCD" В ARDUINO ДЛЯ EPROM */
      st=98;
      for (int i=0; i<2; i++) {
          macAdr[i] = Serial.parseInt();
          eepromWrite(macAdr[i], st);
          st++;
      }

      /* "w1_gpio" В ARDUINO ДЛЯ EEPROM */
      st=511;
      macAdr[0] = Serial.parseInt();
      eepromWrite(macAdr[0], st);

      /* i2c_adres В ARDUINO ДЛЯ EPROM */
      int hb;
      Serial.read();
      for (int ii=0; ii<2; ii++) {
        int bn=Serial.read();
        if ((bn>=48) && (bn<58)) tSet=bn-48;
        else if ((bn>=65) && (bn<71)) tSet=bn-55;
        else if ((bn>=97) && (bn<103)) tSet=bn-87;
        else tSet=0;
        if (ii==0) hb=(tSet<<4)&0x00f0;
      }
      tSet+=hb;
      eepromWrite(tSet, 510);

      asm volatile ("jmp 0x0000"); /* REBOOT */
    }

    else if (inKluc=='b') {
      whil++;
      int hb, lb;
    /* echo b+=0 7 KotelIn 28A9324B03000043 1 5 1 512 16/ > /dev/ttyUSB0 */ /* ШАБЛОН MAC1 */
    /* b+= ключ, 0-порядковый МАК№, 8-колличество символов в имени "Kotel-in", 28A9324B03000043 - MAK */
    /* echo b+=1 8 KotelOut 2810424B030000BF 1 6 0 556 16/ > /dev/ttyUSB0 */ /* ШАБЛОН MAC2 */
      wdt_reset();
      /* "name" ДЛЯ EEPROM */
      st=181;
      st=st+13*pol;
      hb = Serial.parseInt();
      Serial.read();
      for (int i=0; i < 13; i++) {
         if (i<hb) tSet=Serial.read();
         else tSet=' ';
         eepromWrite(tSet, st);
         st++;
      }
      /* "MAK" ДЛЯ EEPROM */
      wdt_reset();
      st=100;
      st=st+8*pol;
      Serial.read();
      for (int i=0; i < 8; i++) {
         for (int ii=0; ii<2; ii++) {
        int bn=Serial.read();
        if ((bn>=48) && (bn<58)) tSet=bn-48;
        else if ((bn>=65) && (bn<71)) tSet=bn-55;
        else if ((bn>=97) && (bn<103)) tSet=bn-87;
        else tSet=0;
        if (ii==0) hb=(tSet<<4)&0x00f0;
         }
         tSet+=hb;
         eepromWrite(tSet, st);
         st++;
      }
      wdt_reset();
      if (pol<4) { /* 4 - ограничиваем колличество для LCD. */
          st=16+pol; /* "typT" В ARDUINO ДЛЯ EPROM */
          macAdr[0] = Serial.parseInt();
          if (pol<4) eepromWrite(macAdr[0], st);

          st=20+pol; /* "nGp1_out" В ARDUINO ДЛЯ EPROM */
          macAdr[0] = Serial.parseInt();
          if (pol<4) eepromWrite(macAdr[0], st);

          st=24+pol; /* "invers1" В ARDUINO ДЛЯ EPROM */
          macAdr[0] = Serial.parseInt();
          if (pol<4) eepromWrite(macAdr[0], st);

          /* "alAlarm И histeresis" ДЛЯ EEPROM */
//          b+=9 12 Kotel****Out 2810424B030000BF 1 6 0 556 16/
          st=0;
          st=st+4*pol;
          for (int i=0; i<2; i++) {
          tSet = Serial.parseInt();
        if (i == 0) {
          if (tSet > 999) tSet=999;
          else if (tSet < 1) tSet=1;
        } else if (i == 1) {
          if (tSet > 99) tSet=99;
          else if (tSet < 1) tSet=1;
        }
        lb=tSet&0x00ff;
        hb=(tSet>>8)&0x00ff;
        eepromWrite(hb, st);; st++;
        eepromWrite(lb, st);; st++;
          }
      }

    }
    else if ((inKluc=='d') && (pol==0)) {
      /* echo d+=0 33 Termostat-serial-arduino-pro-mini > /dev/ttyUSB0 */
      /* echo d+=0 47 Кирилица-в-имени-arduino-pro-mini > /dev/ttyUSB0 */
    /* НАЗВАНИE ПРИБОРА  (кирилица 27-символa a латынь 54-символa) В ARDUINO ДЛЯ EEPROM 312-366 */
      /* "name" ДЛЯ EEPROM */
      wdt_reset();
      int lb, hb = Serial.parseInt(); // количество символов в НАЗВАНИИ ПРИБОРА
      hb+=312;
      Serial.read(); // пробел
      for (int i=312; i < 367; i++) {
         if (i<hb) lb=Serial.read();
         else lb=' ';
         eepromWrite(lb, i);
      }
    }
  }
     }
     else if (inKluc == 'e') {
      /* e+=end */
  if ((Serial.read() == '+') && (Serial.read() == '=') && (Serial.read() == 'e')
          && (Serial.read() == 'n') && (Serial.read() == 'd')) { // проверяем '+=end'
    if (whil != 0) delay(50); asm volatile ("jmp 0x0000"); /* REBOOT */
  }
     }
   }      /* END Serial.available() */
   wdt_reset();
   delay(50);
   if (whil>40) whil=1;
}
/*********************************************************************************************/

/*********************************************************************************************/
void speedSave(unsigned long speed) {
    speed/=400; /* ПРИНЯТОЕ ЧИСЛО "speed" ДЕЛИМ НА 400, ДЛЯ УПРОЩЕНИЯ ЗАПИСИ В 1-БАЙТОВЫЙ EEPROM */
    int st=96;
    /* сдвиг вправо на 8, побитное 'И' с 0x00FF, готовим 2-байт число для записи в 1-байт EEPROM */
    int lb=speed&0x00ff, hb=(speed>>8)&0x00ff;
    eepromWrite(hb, st);; st++;
    eepromWrite(lb, st);
}
/*********************************************************************************************/

/*********************************************************************************************/
void eepromWrite(int c, int st) {
 if (c != EEPROM.read(st)) {
    EEPROM.write(st, c);
 }
}
/*********************************************************************************************/
void reset_defaults(unsigned long speed) {
  // Allarm
    int tSet, hb, lb, st=0;
    for (int i=0; i<8; i++) {
  if ((i==0) || (i==2) || (i==4) || (i==6)) tSet=305;
  if ((i==1) || (i==3) || (i==5) || (i==7)) tSet=5;
  lb=tSet&0x00ff; hb=(tSet>>8)&0x00ff;
  eepromWrite(hb, st);; st++;
  eepromWrite(lb, st);; st++;
    }
  //"typT"
    st=16;
    for (int i=0; i<4; i++) {
  tSet=0;
  eepromWrite(tSet, st);
  st++;
    }
    //gpio
    eepromWrite(outPin1, st);
    st++;
    eepromWrite(outPin2, st);
    st++;
    eepromWrite(outPin3, st);
    st++;
    eepromWrite(outPin4, st);
    st++;
    //"invers"
    st=24;
    for (int i=0; i<4; i++) {
  tSet=1;
  eepromWrite(tSet, st);
  st++;
    }
    //lcd
    st=98;
    eepromWrite(lcd16xx, st);
    st++;
    eepromWrite(lcdxxx2, st);
    /* НАЗВАНИE ПРИБОРА  (кирилица 27-символa a латынь 55-символa) В ARDUINO ДЛЯ EEPROM 312-367 */
    st=312;
    tSet='N';
    eepromWrite(tSet, st);
    st++;
    tSet='e';
    eepromWrite(tSet, st);
    st++;
    tSet='w';
    eepromWrite(tSet, st);
    st++;
    tSet='-';
    eepromWrite(tSet, st);
    st++;
    tSet='N';
    eepromWrite(tSet, st);
    st++;
    tSet='a';
    eepromWrite(tSet, st);
    st++;
    tSet='m';
    eepromWrite(tSet, st);
    st++;
    tSet='e';
    eepromWrite(tSet, st);
    st++;
    tSet='?';
    eepromWrite(tSet, st);
    tSet=' ';
    for (st+=1; st < 367; st++) eepromWrite(tSet, st);
    // i2c_adres
    st=510;
    eepromWrite(i2c_adres, st);
    st++;
    // W1 - gpio pin
    eepromWrite(W1Pin, st);
    st++;
    // метка начальной установки.
    tSet=0;
    eepromWrite(tSet, st);
 //   speedSave(defaultSpeed);
    speedSave(speed);
  lcd.backlight(); // Backlight
  lcdInfoTx(4);       /* PRINT LCD "Reset to defauls OK" */
    while ((digitalRead (buttonPLUS) == LOW) || (digitalRead (buttonMinus) == LOW)) {
  lcd.noBacklight(); // nobacklight Reset to defaults:
  delay(500);
  lcd.backlight(); // Backlight
  delay(500);
    }
  lcdInfoTx(5);     /* PRINT LCD "REBOOT" */
    delay(1500);
    delay(50); asm volatile ("jmp 0x0000"); /* REBOOT */
}



