#include <linux/version.h>

/* Перевірка версії ядра (мінімум 5.4.0) */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0)
#error "Error: Kernel 5.4.0 or newer required due to modern gpiod API!"
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

/* Шлях до gpiolib.h для доступу до desc->gdev->chip */
/* #include "gpiolib.h" */
#include "../../drivers/gpio/gpiolib.h"

/* ПІДКЛЮЧЕНО: логіку операцій та структуру з окремого .h файлу */
#include "hd44780-lcd-ops.h"

#define DRV_NAME "hd44780-gpio-custom"

/* Визначення глобального масиву шин (експортується через extern) */
struct hd44780_custom_bus buses[BUS_COUNT];

static struct platform_device *pdevs[BUS_COUNT];
static struct gpiod_lookup_table *lookup_tables[BUS_COUNT];

static unsigned int bus0[8];
static unsigned int bus1[8];
static unsigned int bus2[8];
static unsigned int bus3[8];

static int bus0_npar, bus1_npar, bus2_npar, bus3_npar;

module_param_array(bus0, uint, &bus0_npar, 0444);
MODULE_PARM_DESC(bus0, "bus0: id,rs,rw,e,db4,db5,db6,db7");

module_param_array(bus1, uint, &bus1_npar, 0444);
MODULE_PARM_DESC(bus1, "bus1: id,rs,rw,e,db4,db5,db6,db7");

module_param_array(bus2, uint, &bus2_npar, 0444);
MODULE_PARM_DESC(bus2, "bus2: id,rs,rw,e,db4,db5,db6,db7");

module_param_array(bus3, uint, &bus3_npar, 0444);
MODULE_PARM_DESC(bus3, "bus3: id,rs,rw,e,db4,db5,db6,db7");

static void __init hd44780_custom_parse_bus(int index, 
					    unsigned int *bus_params, 
					    int npar)
{
	if (npar < 8)
		return;

	buses[index].id = bus_params[0];
	buses[index].pins[HD_RS]  = bus_params[1];
	buses[index].pins[HD_RW]  = bus_params[2];
	buses[index].pins[HD_E]   = bus_params[3];
	buses[index].pins[HD_DB4] = bus_params[4];
	buses[index].pins[HD_DB5] = bus_params[5];
	buses[index].pins[HD_DB6] = bus_params[6];
	buses[index].pins[HD_DB7] = bus_params[7];
	buses[index].present = true;
}

static int __init hd44780_custom_init(void)
{
	int i, p;
	int err = 0;

	pr_info(DRV_NAME ": Custom GPIO-based HD44780 driver loading\n");

	hd44780_custom_parse_bus(0, bus0, bus0_npar);
	hd44780_custom_parse_bus(1, bus1, bus1_npar);
	hd44780_custom_parse_bus(2, bus2, bus2_npar);
	hd44780_custom_parse_bus(3, bus3, bus3_npar);

	for (i = 0; i < BUS_COUNT; i++) {
		struct platform_device *pdev;
		struct gpiod_lookup_table *gbl;
		struct gpio_desc *desc;
		const char *chip_label = NULL;
		char *dev_name;
		bool pins_valid = true;

		if (!buses[i].present)
			continue;

		for (p = 0; p < 7; p++) {
			if (!gpio_is_valid(buses[i].pins[p])) {
				pr_err(DRV_NAME ": invalid GPIO %d on bus %d\n",
				       buses[i].pins[p], i);
				pins_valid = false;
				break;
			}
			
			desc = gpio_to_desc(buses[i].pins[p]);
			if (!desc) {
				pr_err(DRV_NAME ": no descriptor for GPIO %d\n",
				       buses[i].pins[p]);
				pins_valid = false;
				break;
			}
			
			if (!chip_label) {
				chip_label = desc->gdev->chip->label;
			}
		}

		if (!pins_valid || !chip_label)
			continue;

		pr_info(DRV_NAME ": Controller '%s' for bus %d\n", chip_label, i);

		dev_name = kasprintf(GFP_KERNEL, "hd44780.%d", buses[i].id);
		if (!dev_name) {
			err = -ENOMEM;
			goto err_unregister;
		}

		gbl = kzalloc(struct_size(gbl, table, 8), GFP_KERNEL);
		if (!gbl) {
			kfree(dev_name);
			err = -ENOMEM;
			goto err_unregister;
		}

		gbl->dev_id = dev_name;

		gbl->table[HD_RS].key = chip_label;
		gbl->table[HD_RS].chip_hwnum = buses[i].pins[HD_RS];
		gbl->table[HD_RS].con_id = "rs";
		gbl->table[HD_RS].flags = GPIO_ACTIVE_HIGH;

		gbl->table[HD_RW].key = chip_label;
		gbl->table[HD_RW].chip_hwnum = buses[i].pins[HD_RW];
		gbl->table[HD_RW].con_id = "rw";
		gbl->table[HD_RW].flags = GPIO_ACTIVE_HIGH;

		gbl->table[HD_E].key = chip_label;
		gbl->table[HD_E].chip_hwnum = buses[i].pins[HD_E];
		gbl->table[HD_E].con_id = "enable";
		gbl->table[HD_E].flags = GPIO_ACTIVE_HIGH;

		gbl->table[HD_DB4].key = chip_label;
		gbl->table[HD_DB4].chip_hwnum = buses[i].pins[HD_DB4];
		gbl->table[HD_DB4].con_id = "datacon";
		gbl->table[HD_DB4].idx = 4;
		gbl->table[HD_DB4].flags = GPIO_ACTIVE_HIGH;

		gbl->table[HD_DB5].key = chip_label;
		gbl->table[HD_DB5].chip_hwnum = buses[i].pins[HD_DB5];
		gbl->table[HD_DB5].con_id = "datacon";
		gbl->table[HD_DB5].idx = 5;
		gbl->table[HD_DB5].flags = GPIO_ACTIVE_HIGH;

		gbl->table[HD_DB6].key = chip_label;
		gbl->table[HD_DB6].chip_hwnum = buses[i].pins[HD_DB6];
		gbl->table[HD_DB6].con_id = "datacon";
		gbl->table[HD_DB6].idx = 6;
		gbl->table[HD_DB6].flags = GPIO_ACTIVE_HIGH;

		gbl->table[HD_DB7].key = chip_label;
		gbl->table[HD_DB7].chip_hwnum = buses[i].pins[HD_DB7];
		gbl->table[HD_DB7].con_id = "datacon";
		gbl->table[HD_DB7].idx = 7;
		gbl->table[HD_DB7].flags = GPIO_ACTIVE_HIGH;

		gpiod_add_lookup_table(gbl);
		lookup_tables[i] = gbl;

		pdev = platform_device_alloc("hd44780", buses[i].id);
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

		/* ДОДАНО: Ініціалізація символьного файлу пристрою з окремого коду */
		err = hd44780_bus_init(i);
		if (err) {
			goto err_unregister;
		}

		pr_info(DRV_NAME ": registered bus%d (hd44780.%d, hd_rs=%d, hd_rw=%d, hd_e=%d, hd_db4=%d, hd_db5=%d, hd_db6=%d, hd_db7=%d)\n",
			i, buses[i].id, 
			buses[i].pins[HD_RS],  buses[i].pins[HD_RW],  buses[i].pins[HD_E],
			buses[i].pins[HD_DB4], buses[i].pins[HD_DB5], buses[i].pins[HD_DB6], buses[i].pins[HD_DB7]);

	}

	return 0;

err_unregister:
	for (i = 0; i < BUS_COUNT; i++) {
		/* ДОДАНО: Видалення пристрою /dev/hd44780.X при помилці */
		hd44780_bus_cleanup(i);

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

static void __exit hd44780_custom_exit(void)
{
	int i;

	pr_info(DRV_NAME ": Unloading driver\n");

	for (i = 0; i < BUS_COUNT; i++) {
		/* ДОДАНО: Чисте видалення пристрою /dev/hd44780.X при виході */
		hd44780_bus_cleanup(i);

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

module_init(hd44780_custom_init);
module_exit(hd44780_custom_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Gabor Juhos <juhosg at openwrt.org>");
MODULE_AUTHOR("melsem");
MODULE_DESCRIPTION("Universal Custom GPIO-based HD44780 driver");
MODULE_VERSION("2.2");

