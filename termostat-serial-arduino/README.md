 Help по использованию скетча *Терморегулятор* (Termometr_Lcd_autoScan_4Sensor.ino)
 
 http://cyber-place.ru/showpost.php?p=39406&postcount=1

Терморегулятор на Arduino работает самостоятельно.
Роутер не обязателен, можно одноразово и с пк настройки прописать.

 Вход в меню-настроек: одновременно нажать-отпустить две кнопки *buttonMenu* и *buttonPLUS*
В меню-настроек на самой arduino можно изменять:
тип - термометр, термо-стат, кондиционер, водяной насос отопительного котла и их температуру отключения-включения и гистерезис на 4 датчиках и колличество датчиков.

  Термостат, кондиционер и насос отопительного котла - это терморегуляторы отличающиеся, между собой, по методу включения-отключения gpio-выходов.
  
 1. Залить Скетч в arduino. Подключить кнопки, i2C-LCD и ds18b20 + подтяжку от 3,3кОм до 4,7кОм
 *  buttonMenu = 2, //кнопка пин 2
 *  buttonPLUS = 3, //кнопка пин 3
 *  buttonMinus = 4; //кнопка пин 4

 2. СЕРИЙНЫЕ НОМЕРА ДАТЧИКОВ ПРОГРАМА ПРОСКАНИРУЕТ И СОХРАНИТ САМА, ТОЛЬКО НАДО:
 * НАЖАТЬ И УДЕРЖИВАТЬ ОДНОВРЕМЕННО ДВЕ КНОПКИ "buttonMenu" И "buttonPLUS",
 * и нажать и потом отпустить кнопку reset на плате, ПОДОЖДАТЬ 5-6 СЕК, 
 * ОТПУСТИТЬ КНОПКИ "buttonMenu" И "buttonPLUS"
 * или 
 * НАЖАТЬ И УДЕРЖИВАТЬ ОДНОВРЕМЕННО ДВЕ КНОПКИ "buttonMenu" И "buttonPLUS",
 * ПОДАТЬ ПИТАНИЕ, ПОДОЖДАТЬ 5-6 СЕК И ОТПУСТИТЬ КНОПКИ "buttonMenu" И "buttonPLUS"
 
 На LCD отобразит температуру. Прибор уже готов сам работать.
 
  После первой прошивки arduino, программа ЕДИНО-РАЗОВО сама зделает RESET_to_defaults.
  Последующие прошивки сброса делать не будут!!!
  
             RESET_to_defaults

 *   // W1 gpio- вход-выход
 * #define W1Pin   11
 *   // gpio- выходы
 * #define outPin1   5
 * #define outPin2   6
 * #define outPin3   7
 * #define outPin4   8
 *   // cкорость порта
 * #define defaultSpeed  115200
 *   // LCD 16x2
 * #define i2c_adres 0x27
 * #define lcd16xx   16
 * #define lcdxxx2   2  // два датчика ds18b20

Если, что то, где то криво пошло не так. Есть RESET всех настроек.

 АВАРИЙНОЕ ВОССТАНОВЛЕНИЕ РАБОТОСПОСОБНОСТИ UART-232 И ВСЕХ НАСТРОЕК
 * НАЖАТЬ И УДЕРЖИВАТЬ КНОПКУ "buttonPLUS" ПОДАТЬ ПИТАНИЕ, ПОДОЖДАТЬ 5-6 СЕК И ОТПУСТИТЬ КНОПКУ "buttonPLUS" 
 * НАЖАТАЯ КНОПКА "buttonPLUS" Reset to defaults: ВСЕХ НАСТРОЕК И ЗАПИШЕТ speed=115200;
 
 * или 
 * ПРИ ВКЛЮЧЕННОМ ПИТАНИИ НАЖАТЬ И УДЕРЖИВАТЬ КНОПКУ "buttonPLUS" НАЖАТЬ И ОТПУСТИТЬ КНОПКУ "RESET" НА ПЛАТЕ,
 * ПОДОЖДАТЬ 5-6 СЕК И ОТПУСТИТЬ КНОПКУ "buttonPLUS" 
 
 * или
 * НАЖАТАЯ КНОПКА "buttonMinus" Reset to defaults: ВСЕХ НАСТРОЕК И ЗАПИШЕТ speed=57600;
 
 * или
 * НАЖАТЫЕ ВМЕСТЕ КНОПКИ "buttonPLUS" И "buttonMinus" Reset to defaults: ВСЕХ НАСТРОЕК И ЗАПИШЕТ speed=9600;


* -----------------------------------------------------------------------------------------
*	С вебморды еще можно изменить gpio-выходы и менять местами, LCD на 20-ти символьный, подписывать имена к датчикам,
*	местами менять Серийные номера, переключать активные уровни HIGH-LOW,
*	скрость порта, адрес i2c.
* -----------------------------------------------------------------------------------------

2. Поключить к роутеру.
* Потом установить Необходимые пакеты в роутер:
* kmod-usb-serial-ch341 или kmod-usb-serial-pl2303, или другой, в зависимости какой usb-uart переходник используется на arduino.
* Если подключаться к uart роутера то usb-serial не надобно устанавливать.

3. С архива установить:
opkg install port_listen
opkg install termostat-serial-arduino

4. Модуь пропишется в вебке Cyberwrt.
После установки при первом входе в вебку модуля перекинет в настройки, это надо для запуска демона port_listen, который постоянно слушает порт.
С не настроенным или не установленным ком-портом не запустится.

# И обязательная последовательность действий для упрощения первичной настройки синхронизации.

* Если в скетче ничего не изменялось то прописать только свой ком-порт
* и нажать сохранить и перезагрузить обязательно.
* Потом нажать в вебе "Термостат-arduino-serial", должна отобразится температура.
* И нажать "Настройка_arduino". 

* И все, Настройки arduino и модуля в роутере синхронизируются, и теперь можно настраивать под себя.

   Если что то настроить кнопками на самой arduino, в вебе зайдя в "Настройка_arduino" автоматом пропишется и сохранится.
   Что бы от этого не затиралась флэш, вписываются только изменения. Если изменений в arduino не было, то и писать ничего не будет.
