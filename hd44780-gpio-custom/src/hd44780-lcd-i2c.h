#ifndef _HD44780_LCD_I2C_H
#define _HD44780_LCD_I2C_H

#include <linux/types.h>

/**
 * hd44780_i2c_bus_init - Ініціалізація дисплея в режимі I2C Expander
 * @index:    Індекс у внутрішньому масиві buses[BUS_COUNT] (0..3)
 * @dev_num:  Номер пристрою, за яким створиться файл у /dev/hd44780.X
 * @address:  Шістнадцяткова адреса PCF8574 (наприклад, 0x27 або 0x3F)
 * @sda:      Номер GPIO піна для лінії SDA (використовується для "virtual")
 * @scl:      Номер GPIO піна для лінії SCL (використовується для "virtual")
 * @bus_type: Тип шини з конфігу UCI ("builtin" або "virtual")
 */
int hd44780_i2c_bus_init(int index, int dev_num, unsigned short address, int sda, int scl, const char *bus_type);

/**
 * hd44780_i2c_bus_cleanup - Чисте вивантаження I2C дисплея та видалення пристроїв
 * @index: Індекс у внутрішньому масиві buses[BUS_COUNT] (0..3)
 */
void hd44780_i2c_bus_cleanup(int index);

#endif /* _HD44780_LCD_I2C_H */
