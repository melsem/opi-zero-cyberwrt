/*
 *  Custom GPIO-based SPI driver (Dynamic Controller Detection v2.2)
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

#define DRV_NAME "spi-gpio-custom"
#define BUS_COUNT 4

struct spi_gpio_custom_bus {
	unsigned int id;
	unsigned int sck;
	unsigned int mosi;
	unsigned int miso;
	unsigned int cs;
	bool present;
};

static struct spi_gpio_custom_bus buses[BUS_COUNT];
static struct platform_device *pdevs[BUS_COUNT];
static struct gpiod_lookup_table *lookup_tables[BUS_COUNT];

static unsigned int bus0[5];
static unsigned int bus1[5];
static unsigned int bus2[5];
static unsigned int bus3[5];

static int bus0_npar, bus1_npar, bus2_npar, bus3_npar;

module_param_array(bus0, uint, &bus0_npar, 0444);
MODULE_PARM_DESC(bus0, "bus0 parameters: id,sck,mosi,miso,cs");

module_param_array(bus1, uint, &bus1_npar, 0444);
MODULE_PARM_DESC(bus1, "bus1 parameters: id,sck,mosi,miso,cs");

module_param_array(bus2, uint, &bus2_npar, 0444);
MODULE_PARM_DESC(bus2, "bus2 parameters: id,sck,mosi,miso,cs");

module_param_array(bus3, uint, &bus3_npar, 0444);
MODULE_PARM_DESC(bus3, "bus3 parameters: id,sck,mosi,miso,cs");

static void __init spi_gpio_custom_parse_bus(int index, unsigned int *bus_params, int npar)
{
	if (npar < 5)
		return;

	buses[index].id = bus_params[0];
	buses[index].sck = bus_params[1];
	buses[index].mosi = bus_params[2];
	buses[index].miso = bus_params[3];
	buses[index].cs = bus_params[4];
	buses[index].present = true;
}

static int __init spi_gpio_custom_init(void)
{
	int i;
	int err = 0;

	pr_info(DRV_NAME ": Custom GPIO-based SPI driver loading\n");

	spi_gpio_custom_parse_bus(0, bus0, bus0_npar);
	spi_gpio_custom_parse_bus(1, bus1, bus1_npar);
	spi_gpio_custom_parse_bus(2, bus2, bus2_npar);
	spi_gpio_custom_parse_bus(3, bus3, bus3_npar);

	for (i = 0; i < BUS_COUNT; i++) {
		struct platform_device *pdev;
		struct gpiod_lookup_table *gbl;
		struct gpio_desc *desc;
		const char *chip_label;
		char *dev_name;

		if (!buses[i].present)
			continue;

		if (!gpio_is_valid(buses[i].sck) || !gpio_is_valid(buses[i].mosi) ||
		    !gpio_is_valid(buses[i].miso) || !gpio_is_valid(buses[i].cs)) {
			pr_err(DRV_NAME ": invalid GPIO pins for bus %d\n", i);
			continue;
		}

		/* Твоя магия: берем имя чипа по пину SCK */
		desc = gpio_to_desc(buses[i].sck);
		if (!desc) {
			pr_err(DRV_NAME ": cannot find kernel descriptor for SCK GPIO %d\n", buses[i].sck);
			continue;
		}

		chip_label = desc->gdev->chip->label;
		pr_info(DRV_NAME ": Detected GPIO controller '%s' for bus %d\n", chip_label, i);

		dev_name = kasprintf(GFP_KERNEL, "spi-gpio.%d", buses[i].id);
		if (!dev_name) {
			err = -ENOMEM;
			goto err_unregister;
		}

		/* Выделяем под 4 пина + 1 терминатор */
		gbl = kzalloc(struct_size(gbl, table, 5), GFP_KERNEL);
		if (!gbl) {
			kfree(dev_name);
			err = -ENOMEM;
			goto err_unregister;
		}

		gbl->dev_id = dev_name;
		
		/* SCK */
		gbl->table[0].key = chip_label;
		gbl->table[0].chip_hwnum = buses[i].sck;
		gbl->table[0].con_id = "sck";
		gbl->table[0].flags = GPIO_ACTIVE_HIGH;

		/* MOSI */
		gbl->table[1].key = chip_label;
		gbl->table[1].chip_hwnum = buses[i].mosi;
		gbl->table[1].con_id = "mosi";
		gbl->table[1].flags = GPIO_ACTIVE_HIGH;

		/* MISO */
		gbl->table[2].key = chip_label;
		gbl->table[2].chip_hwnum = buses[i].miso;
		gbl->table[2].con_id = "miso";
		gbl->table[2].flags = GPIO_ACTIVE_HIGH;

		/* CS */
		gbl->table[3].key = chip_label;
		gbl->table[3].chip_hwnum = buses[i].cs;
		gbl->table[3].con_id = "cs";
		gbl->table[3].flags = GPIO_ACTIVE_HIGH;

		gpiod_add_lookup_table(gbl);
		lookup_tables[i] = gbl;

		pdev = platform_device_alloc("spi-gpio", buses[i].id);
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
		pr_info(DRV_NAME ": registered bus%d (id=%d, sck=%d, mosi=%d, miso=%d, cs=%d)\n",
			i, buses[i].id, buses[i].sck, buses[i].mosi, buses[i].miso, buses[i].cs);
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

static void __exit spi_gpio_custom_exit(void)
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

module_init(spi_gpio_custom_init);
module_exit(spi_gpio_custom_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("melsem");
MODULE_DESCRIPTION("Universal Custom GPIO-based SPI driver");
MODULE_VERSION("2.2");

