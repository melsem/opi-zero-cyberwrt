/*
 *  Custom GPIO-based Rotary Encoder driver (Dynamic Controller Detection v2.2)
 */

#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0)
#error "Error: This universal driver requires Linux kernel 5.4.0 or newer!"
#endif

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio/consumer.h>
#include <linux/gpio/machine.h>
#include <linux/gpio/driver.h>
#include <linux/gpio.h>

#include "gpiolib.h"

#define DRV_NAME "rotary-gpio-custom"
#define BUS_COUNT 4

struct rotary_gpio_custom_device {
	unsigned int id;
	unsigned int gpio_a;
	unsigned int gpio_b;
	unsigned int steps;
	unsigned int axis;
	bool present;
};

static struct rotary_gpio_custom_device devices[BUS_COUNT];
static struct platform_device *pdevs[BUS_COUNT];
static struct gpiod_lookup_table *lookup_tables[BUS_COUNT];

static unsigned int bus0[5];
static unsigned int bus1[5];
static unsigned int bus2[5];
static unsigned int bus3[5];

static int bus0_npar, bus1_npar, bus2_npar, bus3_npar;

module_param_array(bus0, uint, &bus0_npar, 0444);
MODULE_PARM_DESC(bus0, "bus0 parameters: id,gpio_a,gpio_b,steps,axis");

module_param_array(bus1, uint, &bus1_npar, 0444);
MODULE_PARM_DESC(bus1, "bus1 parameters: id,gpio_a,gpio_b,steps,axis");

module_param_array(bus2, uint, &bus2_npar, 0444);
MODULE_PARM_DESC(bus2, "bus2 parameters: id,gpio_a,gpio_b,steps,axis");

module_param_array(bus3, uint, &bus3_npar, 0444);
MODULE_PARM_DESC(bus3, "bus3 parameters: id,gpio_a,gpio_b,steps,axis");

static void __init rotary_gpio_custom_parse_device(int index, unsigned int *bus_params, int npar)
{
	if (npar < 3)
		return;

	devices[index].id = bus_params[0];
	devices[index].gpio_a = bus_params[1];
	devices[index].gpio_b = bus_params[2];
	devices[index].steps = (npar > 3) ? bus_params[3] : 24; /* Дефолт: 24 шага на оборот */
	devices[index].axis = (npar > 4) ? bus_params[4] : 0;  /* Дефолт: ось 0 (REL_X или REL_WHEEL) */
	devices[index].present = true;
}

static int __init rotary_gpio_custom_init(void)
{
	int i;
	int err = 0;

	pr_info(DRV_NAME ": Custom GPIO-based Rotary Encoder driver loading\n");

	rotary_gpio_custom_parse_device(0, bus0, bus0_npar);
	rotary_gpio_custom_parse_device(1, bus1, bus1_npar);
	rotary_gpio_custom_parse_device(2, bus2, bus2_npar);
	rotary_gpio_custom_parse_device(3, bus3, bus3_npar);

	for (i = 0; i < BUS_COUNT; i++) {
		struct platform_device *pdev;
		struct gpiod_lookup_table *gbl;
		struct gpio_desc *desc;
		const char *chip_label;
		char *dev_name;

		if (!devices[i].present)
			continue;

		if (!gpio_is_valid(devices[i].gpio_a) || !gpio_is_valid(devices[i].gpio_b)) {
			pr_err(DRV_NAME ": invalid GPIO pins for encoder %d\n", i);
			continue;
		}

		/* Твоя магия из релиза 2.2: вытаскиваем метку чипа по пину фазы А */
		desc = gpio_to_desc(devices[i].gpio_a);
		if (!desc) {
			pr_err(DRV_NAME ": cannot find kernel descriptor for GPIO %d\n", devices[i].gpio_a);
			continue;
		}

		chip_label = desc->gdev->chip->label;
		pr_info(DRV_NAME ": Detected GPIO controller '%s' for encoder %d\n", chip_label, i);

		dev_name = kasprintf(GFP_KERNEL, "rotary-encoder.%d", devices[i].id);
		if (!dev_name) {
			err = -ENOMEM;
			goto err_unregister;
		}

		/* Выделяем память под таблицу: 2 пина (Фаза А, Фаза B) + 1 терминатор */
		gbl = kzalloc(struct_size(gbl, table, 3), GFP_KERNEL);
		if (!gbl) {
			kfree(dev_name);
			err = -ENOMEM;
			goto err_unregister;
		}

		gbl->dev_id = dev_name;
		
		/* Фаза А (gpios-свойство ядра "gpios" с индексом 0) */
		gbl->table[0].key = chip_label;
		gbl->table[0].chip_hwnum = devices[i].gpio_a;
		gbl->table[0].con_id = "gpios";
		gbl->table[0].idx = 0;
		gbl->table[0].flags = GPIO_ACTIVE_HIGH;

		/* Фаза B (gpios-свойство ядра "gpios" с индексом 1) */
		gbl->table[1].key = chip_label;
		gbl->table[1].chip_hwnum = devices[i].gpio_b;
		gbl->table[1].con_id = "gpios";
		gbl->table[1].idx = 1;
		gbl->table[1].flags = GPIO_ACTIVE_HIGH;

		gpiod_add_lookup_table(gbl);
		lookup_tables[i] = gbl;

		/* Выделяем системное устройство под встроенный драйвер ядра "rotary-encoder" */
		pdev = platform_device_alloc("rotary-encoder", devices[i].id);
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
		pr_info(DRV_NAME ": registered encoder%d (id=%d, a=%d, b=%d)\n",
			i, devices[i].id, devices[i].gpio_a, devices[i].gpio_b);
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

static void __exit rotary_gpio_custom_exit(void)
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

module_init(rotary_gpio_custom_init);
module_exit(rotary_gpio_custom_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("melsem");
MODULE_DESCRIPTION("Universal Custom GPIO-based Rotary Encoder driver");
MODULE_VERSION("2.2");

