#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/moduleparam.h>
#include "hd44780.h"

// Оголошуємо прототипи функцій, які знаходяться в іншому файлі
int __init hd44780_init(void);
ssize_t hd44780_write(struct file *file, const char *buf, size_t count, loff_t *ppos);

int rs_pin = 0;
int rw_pin = 1;
int e_pin  = 2;
int db4_pin = 3;
int db5_pin = 15;
int db6_pin = 16;
int db7_pin = 14;

module_param_named(hd_rs, rs_pin, int, 0444);
module_param_named(hd_rw, rw_pin, int, 0444);
module_param_named(hd_e, e_pin, int, 0444);
module_param_named(hd_db4, db4_pin, int, 0444);
module_param_named(hd_db5, db5_pin, int, 0444);
module_param_named(hd_db6, db6_pin, int, 0444);
module_param_named(hd_db7, db7_pin, int, 0444);

tPinSet pins[PINS_NEEDED] = {
    {0, "hd44780_rs",  0},
    {0, "hd44780_rw",  0},
    {0, "hd44780_e",   0},
    {0, "hd44780_db4", 0},
    {0, "hd44780_db5", 0},
    {0, "hd44780_db6", 0},
    {0, "hd44780_db7", 0},
};

#define HD_RS_LOW  gpio_set_value(pins[HD_RS].pin, 0)
#define HD_RS_HIGH gpio_set_value(pins[HD_RS].pin, 1)
#define HD_RW_LOW  gpio_set_value(pins[HD_RW].pin, 0)
#define HD_RW_HIGH gpio_set_value(pins[HD_RW].pin, 1)
#define HD_E_LOW   gpio_set_value(pins[HD_E].pin, 0)
#define HD_E_HIGH  gpio_set_value(pins[HD_E].pin, 1)

MODULE_AUTHOR("brnt");
MODULE_LICENSE("GPL");

void WriteNibble(unsigned int val)
{
    HD_E_LOW;
    gpio_set_value(pins[HD_DB4].pin, (val & 0x1));
    gpio_set_value(pins[HD_DB5].pin, (val & 0x2) >> 1);
    gpio_set_value(pins[HD_DB6].pin, (val & 0x4) >> 2);
    gpio_set_value(pins[HD_DB7].pin, (val & 0x8) >> 3);
    
    udelay(1);   
    HD_E_HIGH;
    udelay(2); 
    HD_E_LOW;
    udelay(1);    
}

void WriteData(char c)
{
    udelay(1);
    HD_RS_HIGH;
    HD_RW_LOW;
    udelay(1);
    WriteNibble((c >> 4) & 0xf);
    WriteNibble(c & 0xf);
    udelay(50);
}

void WriteCommand(char c)
{
    udelay(1);
    HD_RS_LOW;
    HD_RW_LOW;
    udelay(1);
    WriteNibble((c >> 4) & 0xf);
    WriteNibble(c & 0xf);
    udelay(50);
}

struct file_operations hd44780_fops = {
    .owner = THIS_MODULE,
    .write = hd44780_write, // Береться з cmd_write_init.c
};

struct miscdevice hd44780_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "hd44780",
    .fops = &hd44780_fops,
};

void FreePins(void)
{
    int i;
    for (i = 0; i < PINS_NEEDED; i++)
    {
        if (pins[i].requested)
        {
            gpio_free(pins[i].pin);
            pins[i].requested = 0;    
        }
    }
}

static void __exit hd44780_exit(void)
{
    misc_deregister(&hd44780_device);
    FreePins();
    printk(KERN_INFO "hd44780 LCD driver unloaded.\n");
}

module_init(hd44780_init); // Точка входу тепер у cmd_write_init.c
module_exit(hd44780_exit);

