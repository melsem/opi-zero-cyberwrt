#ifndef _HD44780_H_
#define _HD44780_H_

#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#define HD_RS   0
#define HD_RW   1
#define HD_E    2
#define HD_DB4  3
#define HD_DB5  4
#define HD_DB6  5
#define HD_DB7  6

#define COMMAND_CHAR   0xff 
#define CGRAM_CHAR     0xfe

typedef struct PinSet {
    int pin;               
    char* name;            
    int requested;
} tPinSet;

#define PINS_NEEDED 7

// Оголошуємо змінні та функції, які будуть спільними для обох файлів
extern tPinSet pins[PINS_NEEDED];
extern int rs_pin, rw_pin, e_pin, db4_pin, db5_pin, db6_pin, db7_pin;
extern struct miscdevice hd44780_device;

void WriteCommand(char c);
void WriteData(char c);
void FreePins(void);

#endif

