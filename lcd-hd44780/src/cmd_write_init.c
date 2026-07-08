#include <linux/module.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include "hd44780.h"

// Реалізація функції запису з підтримкою CGRAM
ssize_t hd44780_write(struct file *file, const char *buf, size_t count, loff_t *ppos) 
{
    char *kernel_buf;
    int i;

    if (count == 0)
        return 0;

    kernel_buf = kmalloc(count, GFP_KERNEL);
    if (!kernel_buf)
        return -ENOMEM;

    if (copy_from_user(kernel_buf, buf, count)) {
        kfree(kernel_buf);
        return -EFAULT;
    }

    if ((kernel_buf[0] & 0xff) == CGRAM_CHAR)
    {
        if (count >= 10) 
        {
            unsigned char char_num = kernel_buf[1] & 0x07; 
            unsigned char cgram_address = 0x40 + (char_num * 8); 
            
            WriteCommand(cgram_address); 
            udelay(50);

            for (i = 0; i < 8; i++) 
            {
                WriteData(kernel_buf[2 + i]);
            }
        }
        WriteCommand(0x80); 
        udelay(50);
    }
    else if ((kernel_buf[0] & 0xff) == COMMAND_CHAR) 
    { 
        for (i = 1; i < count; i++)
        {
            msleep(20); 
            WriteCommand(kernel_buf[i]);
        }
    }
    else
    { 
        for (i = 0; i < count; i++)
        {
            if (kernel_buf[i] == '\n') 
                WriteCommand(0xC0); 
            else
                WriteData(kernel_buf[i]);
        }
    }

    kfree(kernel_buf);
    return count;
}

// Реалізація функції ініціалізації
int __init hd44780_init(void)
{
    int i;
    int ret;

    pins[HD_RS].pin  = rs_pin;
    pins[HD_RW].pin  = rw_pin;
    pins[HD_E].pin   = e_pin;
    pins[HD_DB4].pin = db4_pin;
    pins[HD_DB5].pin = db5_pin;
    pins[HD_DB6].pin = db6_pin;
    pins[HD_DB7].pin = db7_pin;

    for (i = 0; i < PINS_NEEDED; i++)
    {
        ret = gpio_request(pins[i].pin, pins[i].name);
        if (ret < 0) {
            printk(KERN_ERR "hd44780: Failed to request GPIO %d\n", pins[i].pin);
            FreePins();
            return ret;
        }
        pins[i].requested = 1;

        ret = gpio_direction_output(pins[i].pin, 0);
        if (ret < 0) {
            printk(KERN_ERR "hd44780: Failed to set direction for GPIO %d\n", pins[i].pin);
            FreePins();
            return ret;
        }
    }

    msleep(50); 
    
    WriteCommand(0x33); 
    msleep(5); 
    WriteCommand(0x32); 
    msleep(5); 
    
    WriteCommand(0x28); 
    udelay(50);
    WriteCommand(0x0C); 
    udelay(50);
    WriteCommand(0x01); 
    msleep(2); 
    WriteCommand(0x06); 
    udelay(50);

    if (misc_register(&hd44780_device)) {
        printk(KERN_WARNING "hd44780: Couldn't register LCD device\n");
        FreePins();
        return -EBUSY;
    }

    printk(KERN_INFO "hd44780 LCD driver loaded with pins: RS=%d, E=%d\n", rs_pin, e_pin);
    return 0;
}

