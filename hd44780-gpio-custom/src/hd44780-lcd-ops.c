#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/gpio.h>

#include "hd44780-lcd-ops.h"

static void hd44780_strobe(int i)
{
	gpiod_set_value(gpio_to_desc(buses[i].pins[HD_E]), 1);
	udelay(2);
	gpiod_set_value(gpio_to_desc(buses[i].pins[HD_E]), 0);
	udelay(40);
}

/* Налаштування ліній даних на вихід або вхід */
static void hd44780_data_dir(int i, int out)
{
	if (out) {
		gpiod_direction_output(gpio_to_desc(buses[i].pins[HD_DB4]), 0);
		gpiod_direction_output(gpio_to_desc(buses[i].pins[HD_DB5]), 0);
		gpiod_direction_output(gpio_to_desc(buses[i].pins[HD_DB6]), 0);
		gpiod_direction_output(gpio_to_desc(buses[i].pins[HD_DB7]), 0);
	} else {
		gpiod_direction_input(gpio_to_desc(buses[i].pins[HD_DB4]));
		gpiod_direction_input(gpio_to_desc(buses[i].pins[HD_DB5]));
		gpiod_direction_input(gpio_to_desc(buses[i].pins[HD_DB6]));
		gpiod_direction_input(gpio_to_desc(buses[i].pins[HD_DB7]));
	}
}

static void hd44780_write4(int i, unsigned int val)
{
	hd44780_data_dir(i, 1);
	gpiod_set_value(gpio_to_desc(buses[i].pins[HD_DB4]), (val >> 0) & 1);
	gpiod_set_value(gpio_to_desc(buses[i].pins[HD_DB5]), (val >> 1) & 1);
	gpiod_set_value(gpio_to_desc(buses[i].pins[HD_DB6]), (val >> 2) & 1);
	gpiod_set_value(gpio_to_desc(buses[i].pins[HD_DB7]), (val >> 3) & 1);
	hd44780_strobe(i);
}

/* Читання 4 біт з дисплея */
static unsigned char hd44780_read4(int i)
{
	unsigned char val = 0;
	hd44780_data_dir(i, 0);
	
	gpiod_set_value(gpio_to_desc(buses[i].pins[HD_E]), 1);
	udelay(2);
	val |= gpiod_get_value(gpio_to_desc(buses[i].pins[HD_DB4])) << 0;
	val |= gpiod_get_value(gpio_to_desc(buses[i].pins[HD_DB5])) << 1;
	val |= gpiod_get_value(gpio_to_desc(buses[i].pins[HD_DB6])) << 2;
	val |= gpiod_get_value(gpio_to_desc(buses[i].pins[HD_DB7])) << 3;
	gpiod_set_value(gpio_to_desc(buses[i].pins[HD_E]), 0);
	udelay(40);
	
	return val;
}

static void hd44780_write_byte(int i, unsigned int val, int mode)
{
	gpiod_set_value(gpio_to_desc(buses[i].pins[HD_RS]), mode);
	gpiod_set_value(gpio_to_desc(buses[i].pins[HD_RW]), 0);
	hd44780_write4(i, val >> 4);
	hd44780_write4(i, val & 0x0F);
}

/* Читання повного байту з дисплея (наприклад з CGRAM чи DDRAM) */
static unsigned char hd44780_read_byte(int i, int mode)
{
	unsigned char val = 0;
	gpiod_set_value(gpio_to_desc(buses[i].pins[HD_RS]), mode);
	gpiod_set_value(gpio_to_desc(buses[i].pins[HD_RW]), 1); /* Режим читання */
	
	val |= (hd44780_read4(i) << 4);
	val |= (hd44780_read4(i) & 0x0F);
	
	return val;
}

static void hd44780_lcd_hw_init(int i)
{
	msleep(50);
	gpiod_set_value(gpio_to_desc(buses[i].pins[HD_RS]), 0);
	hd44780_write4(i, 0x03); msleep(5);
	hd44780_write4(i, 0x03); udelay(150);
	hd44780_write4(i, 0x03);
	hd44780_write4(i, 0x02);
	hd44780_write_byte(i, 0x28, 0);
	hd44780_write_byte(i, 0x0C, 0);
	hd44780_write_byte(i, 0x06, 0);
	hd44780_write_byte(i, 0x01, 0);
	msleep(2);
}

/* Функція керування через IOCTL */
static long hd44780_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct miscdevice *mdev = file->private_data;
	struct hd44780_custom_bus *bus = container_of(mdev, struct hd44780_custom_bus, misc_dev);
	int i = bus - buses;
	unsigned char val;
	unsigned char custom_char[8];
	int k;

	switch (cmd) {
	case HD_IOC_WRITE_CMD:
		if (get_user(val, (unsigned char __user *)arg)) return -EFAULT;
		hd44780_write_byte(i, val, 0);
		if (val == 0x01 || val == 0x02) msleep(2);
		break;

	case HD_IOC_WRITE_DATA:
		if (get_user(val, (unsigned char __user *)arg)) return -EFAULT;
		hd44780_write_byte(i, val, 1);
		break;

	case HD_IOC_READ_DATA:
		val = hd44780_read_byte(i, 1);
		if (put_user(val, (unsigned char __user *)arg)) return -EFAULT;
		break;

	case HD_IOC_SET_CGRAM:
		/* arg передає номер символу (0-7) та масив з 8 байт бітмапи */
		if (copy_from_user(custom_char, (unsigned char __user *)arg, 8)) return -EFAULT;
		/* Очікується, що перед цим користувач викликав команду встановлення адреси CGRAM */
		for (k = 0; k < 8; k++) {
			hd44780_write_byte(i, custom_char[k], 1);
		}
		break;

	default:
		return -EINVAL;
	}
	return 0;
}

static ssize_t hd44780_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	struct miscdevice *mdev = file->private_data;
	struct hd44780_custom_bus *bus = container_of(mdev, struct hd44780_custom_bus, misc_dev);
	int i = bus - buses;
	size_t idx;
	char c;

	for (idx = 0; idx < count; idx++) {
		if (copy_from_user(&c, buf + idx, 1)) return -EFAULT;
		if (c == '\n') hd44780_write_byte(i, 0xC0, 0);
		else if (c == '\r') hd44780_write_byte(i, 0x02, 0);
		else hd44780_write_byte(i, c, 1);
	}
	return count;
}

static const struct file_operations hd44780_fops = {
	.owner          = THIS_MODULE,
	.write          = hd44780_write,
	.unlocked_ioctl = hd44780_ioctl, /* ДОДАНО: підключення ioctl обробника */
};

int hd44780_bus_init(int index)
{
	int err;
	buses[index].dev_node_name = kasprintf(GFP_KERNEL, "hd44780.%d", buses[index].id);
	if (!buses[index].dev_node_name) return -ENOMEM;

	buses[index].misc_dev.minor = MISC_DYNAMIC_MINOR;
	buses[index].misc_dev.name  = buses[index].dev_node_name;
	buses[index].misc_dev.fops  = &hd44780_fops;

	err = misc_register(&buses[index].misc_dev);
	if (err) {
		kfree(buses[index].dev_node_name);
		buses[index].dev_node_name = NULL;
		return err;
	}

	hd44780_lcd_hw_init(index);
	return 0;
}

void hd44780_bus_cleanup(int index)
{
	if (buses[index].dev_node_name) {
		hd44780_write_byte(index, 0x01, 0);
		msleep(2);
		misc_deregister(&buses[index].misc_dev);
		kfree(buses[index].dev_node_name);
		buses[index].dev_node_name = NULL;
	}
}

