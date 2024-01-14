
/*
* 
*/

#define TEST_DTS_MMC	0
#define DTB_TO_DTS_MMC	1
#define DTS_TO_DTB_MMC	2

#define TEST_DTS_SPI	3
#define DTB_TO_DTS_SPI	4
#define DTS_TO_DTB_SPI	5

#define ERROR_ON_OFF	6
#define HELP		7
#define	MODULE		8
#define	PIN_NAME	9
#define NUM_GPIO	10
#define OVERLAYS	11

#define	SET_GPIO	12
#define DTB_TO_DTS	13

#define CONV		14

#define err_help	mini_help

#define GPIO_20_HEADER_26	20
#define GPIO_HEADER	26 /* ORANGEPI_ZERO && ORANGEPI_ZERO_LTS && ORANGEPI_ZERO_H3 && ORANGEPI_ZERO_LTS
			   && ORANGEPI_R1 && ORANGEPI_LITE2 && ORANGEPI_ZEROPLUS2_H3 && ORANGEPI_3
			   && ORANGEPI_ZEROPLUS2_H5 && ORANGEPI_ZEROPLUS
			   */
#define ON_STATUS	disabled/okay
#define OFF_STATUS	okay/disabled

void test_dts ();
void okay_disabled ();
void read_boardname ();
void set_gpio ();
void mini_help ();
void usage1 ();
void dt_overlays ();

/* Allwinner sun8i-h3, sun50i-h5 */
char	*pinctrl="pinctrl@1c20800\\|pinctrl@01c20800",
	*r_pinctrl="pinctrl@1f02c00\\pinctrl@01f02c00",
	*mmc0="mmc@1c0f000\\|mmc@01c0f000",
	*mmc1="mmc@1c10000\\|mmc@01c10000",
	*mmc2="mmc@1c11000\\|mmc@01c11000",
	*ir="ir@1f02000\\|ir@01f02000",
	*pwm="pwm@1c21400\\|pwm@01c21400",
	*i2c0="i2c@1c2ac00\\|i2c@01c2ac00",
	*i2c1="i2c@1c2b000\\|i2c@01c2b000",
	*i2c2="i2c@1c2b400\\|i2c@01c2b400",
	*i2s0="i2s@1c22000\\|i2s@01c22000",
	*i2s1="i2s@1c22400\\|i2s@01c22400",
	*camera="camera@1cb0000\\|camera@01cb0000",
	*codec="codec@1c22c00\\|codec@01c22c00",
	*uart0="serial@1c28000\\|serial@01c28000",
	*uart1="serial@1c28400\\|serial@01c28400",
	*uart2="serial@1c28800\\|serial@01c28800",
	*usb_otg="usb@1c19000\\|usb@01c19000",
	*ehci0="usb@1c1a000\\|usb@01c1a000",
	*ohci0="usb@1c1a400\\|usb@01c1a400",
	*ehci1="usb@1c1b000\\|usb@01c1b000",
	*ohci1="usb@1c1b400\\|usb@01c1b400",
	*ehci2="usb@1c1c000\\|usb@01c1c000",
	*ohci2="usb@1c1c400\\|usb@01c1c400",
	*ehci3="usb@1c1d000\\|usb@01c1d000",
	*ohci3="usb@1c1d400\\|usb@01c1d400",
	*spi0="spi@1c68000\\|spi@01c68000",
	*spi1="spi@1c69000\\|spi@01c69000",
	*ethernet0="ethernet@1c30000\\|ethernet@01c30000",
	*spdif="spdif@1c21000\\|spdif@01c21000",
/* End Allwinner sun8i-h3, sun50i-h5 */

	*otg_tipe="\"peripheral\"\\|\"host\"/\"otg\"",
	*host_tipe="\"peripheral\"\\|\"otg\"/\"host\"",
	*peripheral_tipe="\"host\"\\|\"otg\"/\"peripheral\"",

	*boot_dtb = "/boot/dtb",
	*tmp_dts = "/tmp/t-dts",
	*dtb_mtd = "/dev/mtd1",
	*dtb_tmp = "/tmp/dtb",
	*boardname,
	*tmp_files = "/tmp/tmp_files",
	*tmp_SPI = "/tmp/SPI",
	*moduls,
	*add_aliases,
	*pin_reg,
	tempraw[512];
int	size,
	ofset=0;


// Debugging & Return codes
int	set_gpio_debug	= 0;
int	set_gpio_return	= 0;
/*  ================================================================  */
