/*
   OpenWRT LCD-hd44780 4-bit driver using GPIO pins.

   Copyright (C) brentthomson@gmail.com, 2014

   Adapted from code by bifferos:
       http://svn.code.sf.net/p/linux-adm5120/code/OpenWrt/hd44780/
       Copyright (C) bifferos@yahoo.co.uk, 2008
*/


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION (3, 18, 0)
#include <asm/uaccess.h>
#else
#include <linux/uaccess.h>
#endif
#include <asm/gpio.h>


/*
* You'll need to change these values to suit your board's available GPIOs
* These are convenient (adjacent) pins on the GS-Oolite v1.0
* Pin ID  OrangePi-zero  GPIO number
*/
// #define HD_RW	6 // not used if LCD is write-only (typical)
#define HD_RS	1
#define HD_E	0
#define HD_DB4	3
#define HD_DB5	15
#define HD_DB6	16
#define HD_DB7	14

#define COMMAND_CHAR   0xff // ASCII 255 = nbsp

// Structures to hold the pins we've successfully allocated.
typedef struct PinSet {
	int pin;      // Linux GPIO pin number
	char* name;   // Name of the pin, supplied to gpio_request()
	int result;   // set to zero on successfully obtaining pin
} tPinSet;


static tPinSet pins[] = {
//	{HD_RW,  "HD44780_RW",  -1},
	{HD_RS,  "HD44780_RS",  -1},
	{HD_E,   "HD44780_E",   -1},
	{HD_DB4, "HD44780_DB4", -1},
	{HD_DB5, "HD44780_DB5", -1},
	{HD_DB6, "HD44780_DB6", -1},
	{HD_DB7, "HD44780_DB7", -1},
};

#define PINS_NEEDED (sizeof(pins)/sizeof(tPinSet))


// Macros for setting control lines
#define HD_RS_LOW  gpio_set_value(HD_RS, 0);
#define HD_RS_HIGH gpio_set_value(HD_RS, 1);
#define HD_E_LOW   gpio_set_value(HD_E, 0);
#define HD_E_HIGH  gpio_set_value(HD_E, 1);


MODULE_AUTHOR("brnt");
MODULE_LICENSE("GPL");


// ioctl - I/O control
/*
static int hd44780_ioctl(struct inode *inode, struct file *file, 
		unsigned int cmd, unsigned long arg) 
{
	int data;

	// Read the request data
	if (copy_from_user(&data, (int *)arg, sizeof(data)))
	{
		printk(KERN_INFO "copy_from_user error on hd44780 ioctl.\n");
		return -EFAULT;
	}

	switch (cmd)
	{

		default:		
			printk(KERN_INFO "Invalid ioctl on hd44780\n");
			return -EINVAL;
	}

	// return the result
	if (copy_to_user((int *)arg, &data, 4))
	{
		printk(KERN_INFO "copy_to_user error on hd44780 ioctl\n");
		return -EFAULT;
	}

	return 0;
}
*/
// Write a nibble to the display
static void WriteNibble(unsigned int val)
{
	HD_E_LOW;
	gpio_set_value(HD_DB4, (val&0x1));
	gpio_set_value(HD_DB5, (val&0x2)>>1);
	gpio_set_value(HD_DB6, (val&0x4)>>2);
	gpio_set_value(HD_DB7, (val&0x8)>>3);
	udelay(1);   // data setup time
	HD_E_HIGH;
	udelay(1);
	HD_E_LOW;
	udelay(1);    // data hold time
}

// Write char data to display
static void WriteData(char c)
{
	udelay(1);
	HD_RS_HIGH;
	udelay(1);
	WriteNibble((c>>4)&0xf);
	WriteNibble(c&0xf);
	udelay(50);
}

// Send command code to the display
static void WriteCommand(char c)
{
	udelay(1);
	HD_RS_LOW;
	udelay(1);
	WriteNibble((c>>4)&0xf);
	WriteNibble(c&0xf);
	udelay(50);
}

// Called when writing to the device file.
static ssize_t hd44780_write(struct file *file, const char *buf, size_t count, loff_t *ppos) 
{
	int err;
	char c;
	const char* ptr = buf;
	int i;

	// grab first char and check for command code
	err = copy_from_user(&c,ptr,1);
	if (err != 0)
		return -EFAULT;

	if ((c&0xff) == COMMAND_CHAR) // somehow, masking the char fixes a multiplying byte problem
	{ // if the first char is COMMAND_CHAR, we enter command mode for the rest of the string
		ptr++;
		for (i=1;i<count;i++)
		{
			err = copy_from_user(&c,ptr++,1);
			if (err != 0)
				return -EFAULT;

			mdelay(20);
			WriteCommand(c);
		}
	}
	else
	{ // otherwise, we spit data out to the screen
		for (i=0;i<count;i++)
		{
			err = copy_from_user(&c,ptr++,1);
			if (err != 0)
				return -EFAULT;

			if (c == '\n') // convert newline char to next line command
				WriteCommand(0xC0);
			else
				WriteData(c);
		}
	}
	return count;
}

static struct file_operations hd44780_fops = {
	.owner = THIS_MODULE,
//	.ioctl = hd44780_ioctl,		//  ioctl not supported for now.
	.write = hd44780_write,
};

static struct miscdevice hd44780_device = {
	MISC_DYNAMIC_MINOR,
	"hd44780",
	&hd44780_fops,
};

// Return any acquired pins.
static void FreePins(void)
{
	int i;
	for (i=0;i<PINS_NEEDED;i++)
	{
		if (pins[i].result == 0)
		{
			gpio_free(pins[i].pin);
			pins[i].result = -1;    // defensive programming - avoid multiple free.
		}
	}
}

static int __init hd44780_init(void)
{
	int i;
	int got_pins = 1;

	// Register misc device
	if (misc_register(&hd44780_device)) {
		printk(KERN_WARNING "Couldn't register LCD device\n");
		return -EBUSY;
	}

	// Request pins
	for (i=0;i<PINS_NEEDED;i++)
	{
		pins[i].result = gpio_request(pins[i].pin,pins[i].name);
		if (pins[i].result != 0)
			got_pins = 0;
	}

	// On any failures, return any pins we managed to get and quit.
	if (!got_pins)
	{
		FreePins();
		return -EBUSY;
	}

	// Set port direction.  We assume we can do this if we got the pins.
	// Set initial values to low (0v).
	for (i=0;i<PINS_NEEDED;i++)
	{
		gpio_direction_output(pins[i].pin,0);
	}

	// Power on and setup the display
	WriteCommand(0x33);
	udelay(50);
	WriteCommand(0x32);
	udelay(50);
	WriteCommand(0x28);
	udelay(50);
	WriteCommand(0x0c);
	udelay(50);
	WriteCommand(0x01);
	udelay(50);
	WriteCommand(0x06);
	udelay(50);

	printk(KERN_INFO "hd44780 LCD driver (v1.1) loaded.\n");
	return 0;
}

static void __exit hd44780_exit(void)
{
	FreePins();
	misc_deregister(&hd44780_device);
	printk(KERN_INFO "hd44780 LCD driver (v1.1) unloaded.\n");
}

module_init(hd44780_init);
module_exit(hd44780_exit);


