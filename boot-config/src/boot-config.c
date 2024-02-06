
/*
* 
*/

#include <stdio.h>  
#include <string.h>
#include <strings.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <sys/socket.h>
#include <sys/wait.h> 
#include <fcntl.h>  
#include <termios.h>  
#include <signal.h>  
#include <unistd.h>  
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <ctype.h>

#include <asm/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <errno.h>
#include <linux/version.h>
#include <linux/types.h>

/*  ================================================================  */
#include "include/boot-config.h"

#include "include/overlays_set_gpio.h"		/* edit overlays w1-gpio */

#include "include/test_dts.h"

#include "include/okay_disabled.h"

#include "include/read_boardname.h"

#include "include/err_help.h"

#include "include/dt_overlays.h"
/*  ================================================================  */

/*  ================================================================  */
int main (int argc, char *argv [])
{
  FILE	*fd ;
  char *on_off;

  read_boardname ();

  sprintf (tempraw,"df -h | grep overlayfs &> /dev/null; if [ $? != 1 ]; then echo spi-nor-kernel > %s; fi", tmp_SPI);
  system (tempraw);


	if ((argc == 2) && (strcasecmp (argv [1+ofset], "dtb-dts") == 0)) {
			if ((fd = fopen (tmp_SPI, "r")) == NULL) test_dts (DTB_TO_DTS_MMC, DTB_TO_DTS);
				else if ((fd = fopen (tmp_SPI, "r")) != NULL) test_dts (DTB_TO_DTS_SPI, DTB_TO_DTS); }
	else if ((argc == 2) && (strcasecmp (argv [1+ofset], "dts-dtb") == 0)) {
			if ((fd = fopen (tmp_SPI, "r")) == NULL) test_dts (DTS_TO_DTB_MMC,CONV);
				else if ((fd = fopen (tmp_SPI, "r")) != NULL) test_dts (DTS_TO_DTB_SPI, CONV); }
	else if ((fd = fopen(tmp_dts, "r")) == NULL) {
			if ((fd = fopen (tmp_SPI, "r")) == NULL) test_dts (TEST_DTS_MMC,CONV);
				else if ((fd = fopen (tmp_SPI, "r")) != NULL) test_dts (TEST_DTS_SPI, CONV); }


	if ((argc == 3) && (strcasecmp (argv [1+ofset], "overlays") == 0)) dt_overlays (argc,argv);

	if (argc != 3)				/* Not '3' input arguments. */
	{
		if (argc == 1) err_help (ERROR_ON_OFF,argv);
		else if ((strcasecmp (argv [1+ofset], "-h") == 0) || (argc == 2)) mini_help (HELP,argv);
		else if ((strcasecmp (argv [1+ofset], "-g") == 0) && (argc == 4)) ofset=1;
		else err_help (ERROR_ON_OFF,argv);
	} else if (strcasecmp (argv [1+ofset], "-g") == 0) err_help (NUM_GPIO,argv);	/* '3' input arguments. */

			/* edit overlays w1-gpio gpio */
	if ((strcasecmp (argv [1+ofset], "w1pin") == 0) || \
		 (strcasecmp (argv [1+ofset], "w1") == 0)) set_gpio (argv[1+ofset], argc, argv);
			/* edit overlays sda-scl i2c4 gpio */
	else if ((strcasecmp (argv [1+ofset], "sda_i2c4") == 0) || \
		 (strcasecmp (argv [1+ofset], "scl_i2c4") == 0) || \
			(strcasecmp (argv [1+ofset], "sda-i2c4") == 0) || \
			(strcasecmp (argv [1+ofset], "scl-i2c4") == 0)) set_gpio (argv [1+ofset], argc,argv);
			/* edit overlays pps gpio */
	else if ((strcasecmp (argv [1+ofset], "gpios_pps") == 0) || \
		 (strcasecmp (argv [1+ofset], "gpios-pps") == 0) || \
			(strcasecmp (argv [1+ofset], "echo_pps") == 0) || \
			(strcasecmp (argv [1+ofset], "echo-pps") == 0)) set_gpio (argv [1+ofset],argc,argv);

	if ((argc >= 4) || (strcasecmp (argv [1+ofset], "-g") == 0)) err_help (MODULE,argv); /* '4' or more input arguments. */

   /* ********************************************************************************* */
   /* ********************* Test argv [2+ofset]: okay or disabled ********************* */
	if ((argc = 3) && ((strcasecmp (argv [2+ofset], "disabled") == 0) || (strcasecmp (argv [2+ofset], "off") == 0) || \
					(strcasecmp (argv [2+ofset], "OFF") == 0)))
			on_off=OFF_STATUS;
	else if ((argc = 3) && ((strcasecmp (argv [2+ofset], "okay") == 0) || (strcasecmp (argv [2+ofset], "on") == 0) || \
					(strcasecmp (argv [2+ofset], "ON") == 0)))
			on_off=ON_STATUS;
	else err_help (ERROR_ON_OFF,argv);
		/* End test argv [2+ofset]: okay or disabled */

		/* USB */
	     if (strcasecmp (argv [1+ofset], "usb0") == 0) { okay_disabled (ehci0, on_off); okay_disabled (ohci0, on_off); }
	else if (strcasecmp (argv [1+ofset], "usb1") == 0) { okay_disabled (ehci1, on_off); okay_disabled (ohci1, on_off); }
	else if (strcasecmp (argv [1+ofset], "usb2") == 0) { okay_disabled (ehci2, on_off); okay_disabled (ohci2, on_off); }
	else if (strcasecmp (argv [1+ofset], "usb3") == 0) { okay_disabled (ehci3, on_off); okay_disabled (ohci3, on_off); }
		/* End USB */

		/* peripheral, host, otg */
	else if ((strcasecmp (argv [1+ofset], "usb_otg") == 0) || (strcasecmp (argv [1+ofset], "otg") == 0) || \
		(strcasecmp (argv [1+ofset], "usb_host") == 0) || (strcasecmp (argv [1+ofset], "host") == 0) || \
		(strcasecmp (argv [1+ofset], "usb_peripheral") == 0) || (strcasecmp (argv [1+ofset], "peripheral") == 0))
	{
			okay_disabled (usb_otg, on_off);		/* on - off */
		if ((strcasecmp (argv [1+ofset], "usb_otg") == 0) || (strcasecmp (argv [1+ofset], "otg") == 0))
			okay_disabled (usb_otg, otg_tipe);		/* otg */
		else if ((strcasecmp (argv [1+ofset], "usb_host") == 0) || (strcasecmp (argv [1+ofset], "host") == 0))
			okay_disabled (usb_otg, host_tipe);		/* host */
		else if ((strcasecmp (argv [1+ofset], "usb_peripheral") == 0) || (strcasecmp (argv [1+ofset], "peripheral") == 0))
			okay_disabled (usb_otg, peripheral_tipe);	/* peripheral */
	}	/* End peripheral, host, otg */

	else if ((strcasecmp (argv [1+ofset], "audcodec") == 0) || (strcasecmp (argv [1+ofset], "codec") == 0) || \
							(strcasecmp (argv [1+ofset], "audio") == 0))
			okay_disabled (codec, on_off);	
		/* Uart0 */
	else if (strcasecmp (argv [1+ofset], "uart0") == 0) {
		okay_disabled (uart0, on_off);
		if ((strcasecmp (argv [2+ofset], "okay") == 0) || (strcasecmp (argv [2+ofset], "on") == 0) || \
							(strcasecmp (argv [2+ofset], "ON") == 0))
		{	/* Disable "pwm" only if "uart0" is enabled. */
			on_off=OFF_STATUS;
			okay_disabled (pwm, on_off);
		}
	}	/* End uart0 */

		/* pwm */
	else if (strcasecmp (argv [1+ofset], "pwm") == 0) {
		okay_disabled (pwm, on_off);
		if ((strcasecmp (argv [2+ofset], "okay") == 0) || (strcasecmp (argv [2+ofset], "on") == 0) || \
							(strcasecmp (argv [2+ofset], "ON") == 0))
		{	/* Disable "uart0" only if "pwm" is enabled. */
			on_off=OFF_STATUS;
			okay_disabled (uart0, on_off);
		}
	}	/* End pwm */

	else if (strcasecmp (argv [1+ofset], "uart1") == 0) okay_disabled (uart1, on_off);
	else if (strcasecmp (argv [1+ofset], "uart2") == 0) okay_disabled (uart2, on_off);
	else if (strcasecmp (argv [1+ofset], "spi0") == 0) okay_disabled (spi0, on_off);
	else if (strcasecmp (argv [1+ofset], "spi1") == 0) okay_disabled (spi1, on_off);
	else if (strcasecmp (argv [1+ofset], "ethernet0") == 0) okay_disabled (ethernet0, on_off);
	else if (strcasecmp (argv [1+ofset], "i2s0") == 0) okay_disabled (i2s0, on_off);
	else if (strcasecmp (argv [1+ofset], "i2s1") == 0) okay_disabled (i2s1, on_off);
	else if (strcasecmp (argv [1+ofset], "camera") == 0) okay_disabled (camera, on_off);
	else if (strcasecmp (argv [1+ofset], "ir") == 0) okay_disabled (ir, on_off);
	else if (strcasecmp (argv [1+ofset], "i2c0") == 0) okay_disabled (i2c0, on_off);
	else if (strcasecmp (argv [1+ofset], "i2c1") == 0) okay_disabled (i2c1, on_off);
	else if (strcasecmp (argv [1+ofset], "i2c2") == 0) okay_disabled (i2c2, on_off);
	else if (strcasecmp (argv [1+ofset], "ehci0") == 0) okay_disabled (ehci0, on_off);
	else if (strcasecmp (argv [1+ofset], "ohci0") == 0) okay_disabled (ohci0, on_off);
	else if (strcasecmp (argv [1+ofset], "ehci1") == 0) okay_disabled (ehci1, on_off);
	else if (strcasecmp (argv [1+ofset], "ohci1") == 0) okay_disabled (ohci1, on_off);
	else if (strcasecmp (argv [1+ofset], "ehci2") == 0) okay_disabled (ehci2, on_off);
	else if (strcasecmp (argv [1+ofset], "ohci2") == 0) okay_disabled (ohci2, on_off);
	else if (strcasecmp (argv [1+ofset], "ehci3") == 0) okay_disabled (ehci3, on_off);
	else if (strcasecmp (argv [1+ofset], "ohci3") == 0) okay_disabled (ohci3, on_off);

		/* MMC0 - MMC1 - MMC2 */
	else if (strcasecmp (argv [1+ofset], "mmc0") == 0) okay_disabled (mmc0, on_off);	/* End mmc0 - cd-flash */
	else if ((strcasecmp (argv [1+ofset], "wifi") == 0) || (strcasecmp (argv [1+ofset], "mmc1") == 0))
			okay_disabled (mmc1, on_off);						/* End mmc1 */
	else if (strcasecmp (argv [1+ofset], "mmc2") == 0) okay_disabled (mmc2, on_off);	/* End mmc2 */

		/* dt_overlays. */

	else if (strcasecmp (argv [1+ofset], "spdif") == 0) {
		okay_disabled (spdif, on_off); 
		if ((strcasecmp (argv [2+ofset], "okay") == 0) || (strcasecmp (argv [2+ofset], "on") == 0) || \
							(strcasecmp (argv [2+ofset], "ON") == 0))
		{	/* Disable "uart0" only if "pwm" is enabled. */
			on_off=OFF_STATUS;
			okay_disabled (status_led, on_off);
		}
	}

		/* LED ON/OFF */
	else if ((strcasecmp (argv [1+ofset], "pwr_led") == 0) || \
			(strcasecmp (argv [1+ofset], "green-led") == 0)) okay_disabled (pwr_led, on_off);
	else if ((strcasecmp (argv [1+ofset], "status_led") == 0) || \
			(strcasecmp (argv [1+ofset], "red-led") == 0)) {
		okay_disabled (status_led, on_off); 
		if ((strcasecmp (argv [2+ofset], "okay") == 0) || (strcasecmp (argv [2+ofset], "on") == 0) || \
							(strcasecmp (argv [2+ofset], "ON") == 0))
		{	/* Disable "uart0" only if "pwm" is enabled. */
			on_off=OFF_STATUS;
			okay_disabled (spdif, on_off);
		}
	}
		/* END LED ON/OFF */

	else if ((strcasecmp (argv [1+ofset], "w1") == 0) || (strcasecmp (argv [1+ofset], "w1pin") == 0)) okay_disabled (w1, on_off);

	else if ((strcasecmp (argv [1+ofset], "pps") == 0) || (strcasecmp (argv [1+ofset], "ppspin") == 0)) okay_disabled (pps, on_off);

	else if ((strcasecmp (argv [1+ofset], "gpio_keys_user") == 0) || (strcasecmp (argv [1+ofset], "gpio_keys") == 0) || \
			(strcasecmp (argv [1+ofset], "keys") == 0) || (strcasecmp (argv [1+ofset], "keys_user") == 0))
			okay_disabled (gpio_keys_user, on_off);

	else if ((strcasecmp (argv [1+ofset], "rotary") == 0) || (strcasecmp (argv [1+ofset], "rotary_button") == 0) || \
							(strcasecmp (argv [1+ofset], "rotary_axis") == 0))
	{
		if ((strcasecmp (argv [1+ofset], "rotary") == 0) || (strcasecmp (argv [1+ofset], "rotary_button") == 0))
			okay_disabled (rotary_button, on_off);
		if ((strcasecmp (argv [1+ofset], "rotary") == 0) || (strcasecmp (argv [1+ofset], "rotary_axis") == 0))
			okay_disabled (rotary_axis, on_off);
	}

	else if (strcasecmp (argv [1+ofset], "i2c4") == 0) okay_disabled (i2c4, on_off);

	else if ((argc = 3) && ((strcasecmp (argv [2+ofset], "disabled") == 0) || (strcasecmp (argv [2+ofset], "off") == 0) || \
					(strcasecmp (argv [2+ofset], "OFF") == 0)))
			{ on_off=OFF_STATUS; okay_disabled (argv [1+ofset], on_off); }
	else if ((argc = 3) && ((strcasecmp (argv [2+ofset], "okay") == 0) || (strcasecmp (argv [2+ofset], "on") == 0) || \
					(strcasecmp (argv [2+ofset], "ON") == 0)))
			{ on_off=ON_STATUS; okay_disabled (argv [1+ofset], on_off); }

	else err_help (MODULE,argv);
};
