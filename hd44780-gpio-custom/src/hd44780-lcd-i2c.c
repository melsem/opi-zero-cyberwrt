#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/gpio/machine.h>
#include <linux/gpio/driver.h>
#include <linux/gpio.h>
#include <linux/string.h>

/* Шлях до приватних структур вашого ядра */
#include "../../drivers/gpio/gpiolib.h"
#include "hd44780-lcd-ops.h"

/* Секретний токен доступу до консольних команд ядра */
#define SECURE_TOKEN "780"

/* Статичні масиви для збереження об'єктів саме цієї I2C підсистеми */
static struct platform_device *i2c_pdevs[BUS_COUNT];
static struct gpiod_lookup_table *i2c_lookup_tables[BUS_COUNT];
static struct i2c_client *i2c_clients[BUS_COUNT];

/* 1. НАДІЙНІ ОПЕРАЦІЇ З ШИНОЮ ЧЕРЕЗ ОФІЦІЙНИЙ SMBUS API ЯДРА (Еталон pcf857x.c) */
static void pcf8574_write_byte(int i, unsigned char data)
{
	if (i2c_clients[i]) {
		/* Використовуємо захищений від збоїв атомарний SMBus запис байта */
		i2c_smbus_write_byte(i2c_clients[i], data);
	}
}

static void hd44780_i2c_write4(int i, unsigned char val, int mode)
{
	unsigned char high_nibble = (val << 4) & 0xF0;
	if (mode) high_nibble |= 0x01; /* RS = 1 (режим даних), RS = 0 (команда) */
	
	/* Утримуємо біт підсвітки (Bit 3) завжди увімкненим */
	pcf8574_write_byte(i, high_nibble | 0x08); 
	
	/* Строб сигналу Enable (Bit 2) */
	pcf8574_write_byte(i, high_nibble | 0x0C | 0x08); /* E = 1 */
	udelay(2);
	pcf8574_write_byte(i, (high_nibble & ~0x04) | 0x08); /* E = 0 */
	udelay(40);
}

static void hd44780_i2c_write_byte(int i, unsigned int val, int mode)
{
	hd44780_i2c_write4(i, val >> 4, mode);   /* Відправляємо старші 4 біти */
	hd44780_i2c_write4(i, val & 0x0F, mode); /* Відправляємо молодші 4 біти */
}

/* 2. АПАРАТНА ІНІЦІАЛІЗАЦІЯ ДИСПЛЕЯ HD44780 */
static void hd44780_i2c_lcd_hw_init(int i)
{
	msleep(50);
	hd44780_i2c_write4(i, 0x03, 0); msleep(5);
	hd44780_i2c_write4(i, 0x03, 0); udelay(150);
	hd44780_i2c_write4(i, 0x03, 0);
	hd44780_i2c_write4(i, 0x02, 0); /* Перемикання в 4-бітний режим */
	
	hd44780_i2c_write_byte(i, 0x28, 0); /* 2 рядки, матриця 5х8 */
	hd44780_i2c_write_byte(i, 0x0C, 0); /* Дисплей увімкнено, курсор вимкнено */
	hd44780_i2c_write_byte(i, 0x06, 0); /* Автоматичний зсув курсору праворуч */
	hd44780_i2c_write_byte(i, 0x01, 0); /* Очищення екрана */
	msleep(2);
}

/* 3. ОБРОБНИКИ СИСТЕМНИХ ВИКЛИКІВ ТА IOCTL КОМАНД ДЛЯ USER-SPACE */
static long hd44780_i2c_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
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
		hd44780_i2c_write_byte(i, val, 0);
		if (val == 0x01 || val == 0x02) msleep(2);
		break;

	case HD_IOC_WRITE_DATA:
		if (get_user(val, (unsigned char __user *)arg)) return -EFAULT;
		hd44780_i2c_write_byte(i, val, 1);
		break;

	case HD_IOC_READ_DATA:
		val = 0xFF; 
		if (i2c_clients[i]) {
			/* Використовуємо офіційне апаратне SMBus читання байта */
			int res = i2c_smbus_read_byte(i2c_clients[i]);
			if (res >= 0) {
				val = (unsigned char)res;
			}
		}
		if (put_user(val, (unsigned char __user *)arg)) return -EFAULT;
		break;

	case HD_IOC_SET_CGRAM:
		if (copy_from_user(custom_char, (unsigned char __user *)arg, 8)) return -EFAULT;
		for (k = 0; k < 8; k++) {
			hd44780_i2c_write_byte(i, custom_char[k], 1);
		}
		break;

	default:
		return -EINVAL;
	}
	return 0;
}

/* ЗАХИЩЕНИЙ ТЕКСТОВИЙ ОБРОБНИК ЗАПИСУ З КОНСОЛІ (SHELL) */
static ssize_t hd44780_i2c_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	struct miscdevice *mdev = file->private_data;
	struct hd44780_custom_bus *bus = container_of(mdev, struct hd44780_custom_bus, misc_dev);
	int i = bus - buses;
	char *kbuf;
	size_t idx;

	if (count == 0) return 0;

	/* Виділяємо безпечний буфер у пам'яті ядра */
	kbuf = kmalloc(count + 1, GFP_KERNEL);
	if (!kbuf) return -ENOMEM;

	if (copy_from_user(kbuf, buf, count)) {
		kfree(kbuf);
		return -EFAULT;
	}
	kbuf[count] = '\0';

	/* Перевіряємо простий токен-запит: "CMD:780:[Hex_Cmd]" (мінімум 10 символів) */
	if (count >= 10 && strncmp(kbuf, "CMD:", 4) == 0) {
		char token[8] = {0};
		char cmd_str[4] = {0};
		unsigned int parsed_cmd = 0;
		int parsed;

		/* Розбираємо рядок за допомогою sscanf */
		parsed = sscanf(kbuf, "CMD:%7[^:]:%3s", token, cmd_str);
		if (parsed == 2 && strcmp(token, SECURE_TOKEN) == 0) {
			
			/* Конвертуємо hex-рядок команди в число */
			sscanf(cmd_str, "%x", &parsed_cmd);

			/* Виконуємо як апаратну команду (mode=0) */
			hd44780_i2c_write_byte(i, (unsigned char)parsed_cmd, 0);
			
			/* Якщо це команда очищення (0x01) або Home (0x02) — робимо паузу */
			if (parsed_cmd == 0x01 || parsed_cmd == 0x02) msleep(2);
			
			kfree(kbuf);
			return count;
		}
	}

	/* ЯКЩО ЦЕ ЗВИЧАЙНИЙ ТЕКСТ (ДРУКУЄМО НА ЕКРАН) */
	for (idx = 0; idx < count; idx++) {
		char c = kbuf[idx];
		if (c == '\n') hd44780_i2c_write_byte(i, 0xC0, 0);
		else if (c == '\r') hd44780_i2c_write_byte(i, 0x02, 0);
		else if (c >= 32)   hd44780_i2c_write_byte(i, c, 1);
	}

	kfree(kbuf);
	return count;
}

static const struct file_operations hd44780_i2c_fops = {
	.owner          = THIS_MODULE,
	.write          = hd44780_i2c_write,
	.unlocked_ioctl = hd44780_i2c_ioctl,
};

/* 4. ГОЛОВНА ФУНКЦІЯ ІНІЦІАЛІЗАЦІЇ ШИНИ (ВИКЛИКАЄТЬСЯ З ГОЛОВНОГО ФАЙЛУ) */
int hd44780_i2c_bus_init(int index, int dev_num, unsigned short address, int sda, int scl, const char *bus_type)
{
	int err = 0;
	struct i2c_adapter *adap = NULL;
	struct i2c_board_info info;

	buses[index].id = dev_num;
	buses[index].dev_node_name = kasprintf(GFP_KERNEL, "hd44780.%d", dev_num);
	if (!buses[index].dev_node_name) return -ENOMEM;

	/* === РЕЖИМ VIRTUAL (Кроссплатформний бітбенг на базі вашого еталону) === */
	if (strcmp(bus_type, "virtual") == 0) {
		struct platform_device *pdev;
		struct gpiod_lookup_table *gbl;
		struct gpio_desc *desc;
		const char *chip_label;
		char *dev_name;

		if (!gpio_is_valid(sda) || !gpio_is_valid(scl)) {
			pr_err("hd44780: invalid GPIO pins for virtual bus %d (SDA:%d, SCL:%d)\n", index, sda, scl);
			err = -EINVAL;
			goto err_free_name;
		}

		/* Зчитуємо мітку вашого GPIO контролера по нішці SDA */
		desc = gpio_to_desc(sda);
		if (!desc) {
			pr_err("hd44780: cannot find kernel descriptor for SDA GPIO %d\n", sda);
			err = -ENODEV;
			goto err_free_name;
		}

		chip_label = desc->gdev->chip->label;
		dev_name = kasprintf(GFP_KERNEL, "i2c-gpio.%d", dev_num);
		if (!dev_name) { err = -ENOMEM; goto err_free_name; }

		/* Виділяємо пам'ять під 3 елементи (SDA, SCL + NULL термінатор) */
		gbl = kzalloc(struct_size(gbl, table, 3), GFP_KERNEL);
		if (!gbl) { kfree(dev_name); err = -ENOMEM; goto err_free_name; }

		gbl->dev_id = dev_name;
		
		/* Заповнюємо 0 елемент масиву таблиці — ЛІНІЯ SDA */
		gbl->table[0].key = chip_label;
		gbl->table[0].chip_hwnum = sda;
		gbl->table[0].con_id = "sda";
		gbl->table[0].idx = 0;
		gbl->table[0].flags = GPIO_ACTIVE_HIGH | GPIO_OPEN_DRAIN;

		/* Заповнюємо 1 елемент масиву таблиці — ЛІНІЯ SCL */
		gbl->table[1].key = chip_label;
		gbl->table[1].chip_hwnum = scl;
		gbl->table[1].con_id = "scl";
		gbl->table[1].idx = 0;
		gbl->table[1].flags = GPIO_ACTIVE_HIGH | GPIO_OPEN_DRAIN;

		gpiod_add_lookup_table(gbl);
		i2c_lookup_tables[index] = gbl;

		pdev = platform_device_alloc("i2c-gpio", dev_num);
		if (!pdev) { err = -ENOMEM; goto err_cleanup_lookup; }

		err = platform_device_add(pdev);
		if (err) { platform_device_put(pdev); goto err_cleanup_lookup; }
		i2c_pdevs[index] = pdev;

		pr_info("hd44780: registered virtual i2c bus (id=%d, sda=%d, scl=%d)\n", dev_num, sda, scl);

		adap = i2c_get_adapter(dev_num);
	} else {
		/* === РЕЖИМ BUILTIN (Підключення до рідної апаратної шини процесора) === */
		pr_info("hd44780: Hooking onto built-in hardware bus i2c-%d\n", dev_num);
		adap = i2c_get_adapter(dev_num);
	}

	/* === ЗВ'ЯЗУВАННЯ PCF8574 КЛІЄНТА ТА СТВОРЕННЯ /dev/hd44780.X === */
	if (!adap) {
		pr_err("hd44780: Required I2C adapter i2c-%d not found!\n", dev_num);
		err = -ENODEV;
		goto err_cleanup_pdev;
	}

	memset(&info, 0, sizeof(struct i2c_board_info));
	strscpy(info.type, "pcf8574", I2C_NAME_SIZE);
	info.addr = address;

	i2c_clients[index] = i2c_new_client_device(adap, &info);
	i2c_put_adapter(adap);

	if (!i2c_clients[index]) { err = -ENODEV; goto err_cleanup_pdev; }

	buses[index].misc_dev.minor = MISC_DYNAMIC_MINOR;
	buses[index].misc_dev.name  = buses[index].dev_node_name;
	buses[index].misc_dev.fops  = &hd44780_i2c_fops;

	err = misc_register(&buses[index].misc_dev);
	if (err) goto err_unreg_client;

	hd44780_i2c_lcd_hw_init(index);
	return 0;

err_unreg_client:
	i2c_unregister_device(i2c_clients[index]);
	i2c_clients[index] = NULL;
err_cleanup_pdev:
	if (i2c_pdevs[index]) { platform_device_unregister(i2c_pdevs[index]); i2c_pdevs[index] = NULL; }
err_cleanup_lookup:
	if (i2c_lookup_tables[index]) {
		gpiod_remove_lookup_table(i2c_lookup_tables[index]);
		kfree(i2c_lookup_tables[index]->dev_id);
		kfree(i2c_lookup_tables[index]);
		i2c_lookup_tables[index] = NULL;
	}
err_free_name:
	kfree(buses[index].dev_node_name);
	buses[index].dev_node_name = NULL;
	return err;
}

void hd44780_i2c_bus_cleanup(int index)
{
	if (buses[index].dev_node_name) {
		misc_deregister(&buses[index].misc_dev);
		if (i2c_clients[index]) { i2c_unregister_device(i2c_clients[index]); i2c_clients[index] = NULL; }
		if (i2c_pdevs[index]) { platform_device_unregister(i2c_pdevs[index]); i2c_pdevs[index] = NULL; }
		if (i2c_lookup_tables[index]) {
			gpiod_remove_lookup_table(i2c_lookup_tables[index]);
			kfree(i2c_lookup_tables[index]->dev_id);
			kfree(i2c_lookup_tables[index]);
			i2c_lookup_tables[index] = NULL;
		}
		kfree(buses[index].dev_node_name);
		buses[index].dev_node_name = NULL;
	}
}

