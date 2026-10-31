#ifndef _HD44780_LCD_OPS_H
#define _HD44780_LCD_OPS_H

#include <linux/miscdevice.h>
#include <linux/ioctl.h>

#define HD_RS   0
#define HD_RW   1
#define HD_E    2
#define HD_DB4  3
#define HD_DB5  4
#define HD_DB6  5
#define HD_DB7  6

#define BUS_COUNT 4

/* Магічний номер для IOCTL */
#define HD_IOC_MAGIC 'k'

/* Команди IOCTL для повного керування */
#define HD_IOC_WRITE_CMD   _IOW(HD_IOC_MAGIC, 1, unsigned char)
#define HD_IOC_WRITE_DATA  _IOW(HD_IOC_MAGIC, 2, unsigned char)
#define HD_IOC_SET_CGRAM   _IOW(HD_IOC_MAGIC, 3, unsigned char[8])
#define HD_IOC_READ_DATA   _IOR(HD_IOC_MAGIC, 4, unsigned char)

struct hd44780_custom_bus {
	unsigned int id;
	unsigned int pins[7];
	bool present;
	struct miscdevice misc_dev;
	char *dev_node_name;
};

extern struct hd44780_custom_bus buses[BUS_COUNT];

int hd44780_bus_init(int index);
void hd44780_bus_cleanup(int index);

#endif /* _HD44780_LCD_OPS_H */
