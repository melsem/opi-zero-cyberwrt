
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

#define OVERLAYS_ERASE	15

#define err_help	mini_help

#define GPIO_22_HEADER_26	22
#define GPIO_HEADER	26 /* ORANGEPI_ZERO && ORANGEPI_ZERO_LTS && ORANGEPI_ZERO_H3 && ORANGEPI_ZERO_LTS
			   && ORANGEPI_R1 && ORANGEPI_LITE2 && ORANGEPI_ZEROPLUS2_H3 && ORANGEPI_3
			   && ORANGEPI_ZEROPLUS2_H5 && ORANGEPI_ZEROPLUS
			   */
#define ON_STATUS	"disabled/okay"
#define OFF_STATUS	"okay/disabled"

void test_dts ();
void okay_disabled ();
void read_boardname ();
void set_gpio ();
void mini_help ();
void usage1 ();
void dt_overlays ();

/* Allwinner sun8i-h3, sun50i-h5 */
#define pinctrl		"pinctrl@1c20800\\|pinctrl@01c20800"
#define r_pinctrl	"pinctrl@1f02c00\\pinctrl@01f02c00"
#define mmc0		"mmc@1c0f000\\|mmc@01c0f000"
#define mmc1		"mmc@1c10000\\|mmc@01c10000"
#define mmc2		"mmc@1c11000\\|mmc@01c11000"
#define i2c0		"i2c@1c2ac00\\|i2c@01c2ac00"
#define i2c1		"i2c@1c2b000\\|i2c@01c2b000"
#define i2c2		"i2c@1c2b400\\|i2c@01c2b400"
#define i2s0		"i2s@1c22000\\|i2s@01c22000"
#define i2s1		"i2s@1c22400\\|i2s@01c22400"
#define camera		"camera@1cb0000\\|camera@01cb0000"
#define uart0		"serial@1c28000\\|serial@01c28000"
#define uart1		"serial@1c28400\\|serial@01c28400"
#define uart2		"serial@1c28800\\|serial@01c28800"
#define usb_otg		"usb@1c19000\\|usb@01c19000"
#define ehci0		"usb@1c1a000\\|usb@01c1a000"
#define ohci0		"usb@1c1a400\\|usb@01c1a400"
#define ehci1		"usb@1c1b000\\|usb@01c1b000"
#define ohci1		"usb@1c1b400\\|usb@01c1b400"
#define ehci2		"usb@1c1c000\\|usb@01c1c000"
#define ohci2		"usb@1c1c400\\|usb@01c1c400"
#define ehci3		"usb@1c1d000\\|usb@01c1d000"
#define ohci3		"usb@1c1d400\\|usb@01c1d400"
#define spi0		"spi@1c68000\\|spi@01c68000"
#define spi1		"spi@1c69000\\|spi@01c69000"
#define ethernet0	"ethernet@1c30000\\|ethernet@01c30000"
#define spdif		"spdif@1c21000\\|spdif@01c21000"

#define otg_tipe	"\"peripheral\"\\|\"host\"/\"otg\""
#define host_tipe	"\"peripheral\"\\|\"otg\"/\"host\""
#define peripheral_tipe	"\"host\"\\|\"otg\"/\"peripheral\""
/* End Allwinner sun8i-h3, sun50i-h5 */

/* DT-overlays. */
#define ir		"ir@1f02000\\|ir@01f02000"
#define codec		"codec@1c22c00\\|codec@01c22c00"
#define pwm		"pwm@1c21400\\|pwm@01c21400"
#define pwr_led		"pwr_led\\|green-led"
#define status_led	"status_led\\|red-led"
#define w1		"w1@0\\|w1"
#define pps		"pps@0\\|pps"
#define gpio_keys_user	"gpio_keys_user\\|gpio_keys_user@"
#define rotary		"rotary@0\\|rotary"
#define rotary_button	"rotary_button@0\\|rotary_button"
#define rotary_axis	"rotary_axis@0\\|rotary_axis"
#define i2c4		"i2c4@0\\|i2c4"

#define tmp_files	"/tmp/tmp_files"

#define boot_dtb	"/boot/dtb"
#define tmp_dts		"/tmp/t-dts"
#define dtb_mtd		"/dev/mtd1"
#define dtb_tmp		"/tmp/dtb"
#define tmp_SPI		"/tmp/SPI"

#define dev_null	"/dev/null"

#define f_moduls	"/etc/dt-overlays/moduls"
#define f_aliases	"/etc/dt-overlays/aliases"
/* End DT-overlays. */

char	*boardname,
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
