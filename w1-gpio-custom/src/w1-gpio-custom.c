/*
 *  Custom GPIO-based W1 driver
 *
 *  Copyright (C) 2007 Gabor Juhos <juhosg at openwrt.org>
 *  Copyright (C) 2008 Bifferos <bifferos at yahoo.co.uk>
 *  Copyright (C) 2026 melsem
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 * ---------------------------------------------------------------------------
 *
 *  The behaviour of this driver can be altered by setting some parameters
 *  from the insmod command line.
 *
 *  The following parameters are adjustable:
 *
 *	bus0	These four arguments must be arrays of
 *	bus1	3 unsigned integers as follows:
 *	bus2
 *	bus3	<id>,<pin>,<od>
 *
 *  where:
 *
 *  <id>	ID to used as device_id for the corresponding bus (required)
 *  <sda>	GPIO pin ID of data pin (required)
 *  <od>	Pin is configured as open drain.
 *
 *  See include/w1-gpio.h for more information about the parameters.
 *
 *  If this driver is built into the kernel, you can use the following kernel
 *  command line parameters, with the same values as the corresponding module
 *  parameters listed above:
 *
 *	w1-gpio-custom.bus0
 *	w1-gpio-custom.bus1
 *	w1-gpio-custom.bus2
 *	w1-gpio-custom.bus3
 *
 * ---------------------------------------------------------------------------
 *
 *  Examples for insmod:
 *
 *  1. Single bus with ID 1 on GPIO pin 4 (not open drain):
 *     insmod w1-gpio-custom bus0=1,4,0
 *
 *  2. Multiple buses (ID 1 on GPIO 4, and ID 2 on GPIO 17 as open drain):
 *     insmod w1-gpio-custom bus0=1,4,0 bus1=2,17,1
 */

#include <linux/version.h>

/* Перевірка версії ядра (мінімум 5.4.0) */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0)
#error "Error: This universal driver requires Linux kernel 5.4.0 or newer due to modern gpiod API!"
#endif

#include <linux/kernel.h>
/* #include <linux/module.h> */
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio/consumer.h>
#include <linux/gpio/machine.h>
#include <linux/gpio/driver.h>
#include <linux/gpio.h>

/* Шлях до gpiolib.h може змінюватися залежно від архітектури OpenWrt, 
   підключаємо для доступу до desc->gdev->chip 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 137)
#include "../../drivers/gpio/gpiolib.h"
#else */
#include "gpiolib.h"
/*#endif
*/

#define DRV_NAME "w1-gpio-custom"
#define BUS_COUNT 4

struct w1_gpio_custom_bus {
	unsigned int id;
	unsigned int pin;
	unsigned int od;
	bool present;
};

static struct w1_gpio_custom_bus buses[BUS_COUNT];
static struct platform_device *pdevs[BUS_COUNT];
static struct gpiod_lookup_table *lookup_tables[BUS_COUNT];

static unsigned int bus0[3];
static unsigned int bus1[3];
static unsigned int bus2[3];
static unsigned int bus3[3];

static int bus0_npar, bus1_npar, bus2_npar, bus3_npar;

module_param_array(bus0, uint, &bus0_npar, 0444);
MODULE_PARM_DESC(bus0, "bus0 parameters: id,pin,od");

module_param_array(bus1, uint, &bus1_npar, 0444);
MODULE_PARM_DESC(bus1, "bus1 parameters: id,pin,od");

module_param_array(bus2, uint, &bus2_npar, 0444);
MODULE_PARM_DESC(bus2, "bus2 parameters: id,pin,od");

module_param_array(bus3, uint, &bus3_npar, 0444);
MODULE_PARM_DESC(bus3, "bus3 parameters: id,pin,od");

static void __init w1_gpio_custom_parse_bus(int index, unsigned int *bus_params, int npar)
{
	if (npar < 2)
		return;

	buses[index].id = bus_params[0];
	buses[index].pin = bus_params[1];
	buses[index].od = (npar > 2) ? bus_params[2] : 0;
	buses[index].present = true;
}

static int __init w1_gpio_custom_init(void)
{
	int i;
	int err = 0;

	pr_info(DRV_NAME ": Custom GPIO-based W1 driver loading\n");

	w1_gpio_custom_parse_bus(0, bus0, bus0_npar);
	w1_gpio_custom_parse_bus(1, bus1, bus1_npar);
	w1_gpio_custom_parse_bus(2, bus2, bus2_npar);
	w1_gpio_custom_parse_bus(3, bus3, bus3_npar);

	for (i = 0; i < BUS_COUNT; i++) {
		struct platform_device *pdev;
		struct gpiod_lookup_table *gbl;
		struct gpio_desc *desc;
		const char *chip_label;
		char *dev_name;

		if (!buses[i].present)
			continue;

		if (!gpio_is_valid(buses[i].pin)) {
			pr_err(DRV_NAME ": invalid GPIO pin %d for bus %d\n", buses[i].pin, i);
			continue;
		}

		/* Отримуємо внутрішній дескриптор ядра */
		desc = gpio_to_desc(buses[i].pin);
		if (!desc) {
			pr_err(DRV_NAME ": cannot find kernel descriptor for GPIO %d\n", buses[i].pin);
			continue;
		}

		/* Динамічно забираємо текстове ім'я поточного GPIO контролера */
		chip_label = desc->gdev->chip->label;
		pr_info(DRV_NAME ": Detected GPIO controller '%s' for pin %d\n", chip_label, buses[i].pin);

		dev_name = kasprintf(GFP_KERNEL, "w1-gpio.%d", buses[i].id);
		if (!dev_name) {
			err = -ENOMEM;
			goto err_unregister;
		}

		gbl = kzalloc(struct_size(gbl, table, 2), GFP_KERNEL);
		if (!gbl) {
			kfree(dev_name);
			err = -ENOMEM;
			goto err_unregister;
		}

		gbl->dev_id = dev_name;
		gbl->table[0].key = chip_label;
		gbl->table[0].chip_hwnum = buses[i].pin;
		gbl->table[0].con_id = NULL;
		gbl->table[0].idx = 0;
		gbl->table[0].flags = GPIO_ACTIVE_HIGH | (buses[i].od ? GPIO_OPEN_DRAIN : 0);

		gpiod_add_lookup_table(gbl);
		lookup_tables[i] = gbl;

		pdev = platform_device_alloc("w1-gpio", buses[i].id);
		if (!pdev) {
			err = -ENOMEM;
			goto err_unregister;
		}

		err = platform_device_add(pdev);
		if (err) {
			platform_device_put(pdev);
			goto err_unregister;
		}

		pdevs[i] = pdev;
		pr_info(DRV_NAME ": registered bus%d (id=%d, gpio=%d, od=%d)\n",
			i, buses[i].id, buses[i].pin, buses[i].od);
	}

	return 0;

err_unregister:
	for (i = 0; i < BUS_COUNT; i++) {
		if (pdevs[i]) {
			platform_device_unregister(pdevs[i]);
			pdevs[i] = NULL;
		}
		if (lookup_tables[i]) {
			gpiod_remove_lookup_table(lookup_tables[i]);
			kfree(lookup_tables[i]->dev_id);
			kfree(lookup_tables[i]);
			lookup_tables[i] = NULL;
		}
	}
	return err;
}

static void __exit w1_gpio_custom_exit(void)
{
	int i;

	pr_info(DRV_NAME ": Unloading driver\n");

	for (i = 0; i < BUS_COUNT; i++) {
		if (pdevs[i]) {
			platform_device_unregister(pdevs[i]);
			pdevs[i] = NULL;
		}
		if (lookup_tables[i]) {
			gpiod_remove_lookup_table(lookup_tables[i]);
			kfree(lookup_tables[i]->dev_id);
			kfree(lookup_tables[i]);
			lookup_tables[i] = NULL;
		}
	}
}

module_init(w1_gpio_custom_init);
module_exit(w1_gpio_custom_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Gabor Juhos <juhosg at openwrt.org>");
MODULE_AUTHOR("melsem");
MODULE_DESCRIPTION("Universal Custom GPIO-based W1 driver");
MODULE_VERSION("2.2");
