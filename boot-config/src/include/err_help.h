
void mini_help (int info,char *argv [])
{
    if (info == ERROR_ON_OFF) usage1 (info,argv);

    else if (info == HELP) {
	printf ("\n\n     %s\n",boardname);
	printf (" Convert dtb to dts Example:   %s dtb-dts\n", argv [0]);
	printf (" Convert dts to dtb Example:   %s dts-dtb\n\n", argv [0]);

	printf ("   ------ Overlay-module names. ------\n");
	printf ("   mmc0 / mmc1 / mmc2 / ir / pwm / i2c0 / i2c1 / i2c2 / camera / audcodec\n");
	printf ("   i2s0 / i2s1 / uart0 / uart1 / uart2 / otg / host / peripheral / spdif\n");
	printf ("   ehci0 / ohci0 / ehci1 / ohci1 / ehci2 / ohci2 / ehci3 / ohci3\n");
	printf ("   usb0 / usb1 / usb2 / usb3 / spi0 / spi1 / ethernet0\n");
	printf ("   otg_tipe / host_tipe / peripheral_tipe\n");
	printf ("   w1 / pps / i2c4 / gpio_keys_user\n");
	printf ("   rotary or rotary_axis\n\n");

	usage1 (info,argv);
	printf (" ----------------------------------------\n\n");
    }

    else if (info == OVERLAYS)
    {
	printf ("\n Overlays-module: < %s > does not exist.\n   Check the names in the help: ' %s -h '\n\n", argv [2+ofset], argv [0]);
	exit (EXIT_FAILURE);
    }

    else if (info == MODULE)
	printf ("\n   Module: < %s > does not exist.\n   Check the names in the help: ' %s -h '\n\n", argv [1+ofset], argv [0]);
    else if (info == SET_GPIO)
	printf ("\n   < %s > module: pins cannot be changed.\n\n", argv [1+ofset]);
    else if (info == PIN_NAME)
	printf ("\n   Invalid PINCTRL_PIN name: < %s >\n\n",strdup(argv [2+ofset]));
    else if (info == NUM_GPIO)
 	printf ("\n   The gpio: < %d > number does not match the pinctrl SoC driver.\n\n",atoi(argv [2+ofset]));

    exit (EXIT_FAILURE);
};

void usage1 (int info,char *argv [])
{
	printf (" Modules that can be controlled by commands: <on / ON / okay> or <off / OFF / disabled>.\n");
	printf ("   Usage:   %s <module> <on / ON / okay>\n", argv [0]);
	printf ("   Or:      %s <module> <off / OFF / disabled>\n", argv [0]);
	printf ("   Example1: %s i2c0 on\n", argv [0]);
	printf ("   Example2: %s usb2 off\n\n", argv [0]);
	printf ("     ------------------------------------------------\n");
	printf ("  Example3: %s otg_tipe on\n", argv [0]);
	printf ("  Or:       %s host_tipe on\n", argv [0]);
	printf ("  Or:       %s peripheral_tipe on\n\n", argv [0]);
	printf ("  Or all USB0 tips disable (otg, host, peripheral).\n");
	printf ("  example:  %s host_tipe off\n", argv [0]);
	printf ("     ------------------------------------------------\n\n");

    if (info == HELP) printf (" In the modules 'w1' you can still change the pins.\n");
	printf ("   Add or edit single one-wire gpio-contacts.\n\n");
	printf ("   Usage:    %s -g <module> <gpio number>\n", argv [0]);
	printf ("   Example4: %s -g w1 10\n\n", argv [0]);
	printf ("   Usage:    %s <module> <gpio name>\n", argv [0]);
	printf ("   Example5: %s w1 PA10\n", argv [0]);
	printf ("     ------------------------------------------------\n\n");
	printf (" Edit SDA or SCL gpio i2c4.\n");
	printf ("   sda_i2c4 / sda-i2c4 / scl-i2c4 / scl_i2c4\n");
	printf ("   Usage:   %s <edit gpio i2c4> <gpio name>\n", argv [0]);
	printf ("   Example6:      %s sda_i2c4 PA18\n", argv [0]);
	printf ("   Example6:      %s scl_i2c4 PA19\n", argv [0]);
	printf ("   Example7 - or: %s -g sda_i2c4 18\n", argv [0]);
	printf ("   Example7 - or: %s -g scl_i2c4 19\n\n", argv [0]);

	printf (" Edit gpios or echo-gpios pps.\n");
	printf ("   gpios_pps / gpios-pps / echo_pps / echo-pps\n");
	printf ("   Usage:   %s <edit gpio pps> <gpio name>\n", argv [0]);
	printf ("   Example8:      %s gpios_pps PA6\n", argv [0]);
	printf ("   Example8:      %s echo_pps PA7\n", argv [0]);
	printf ("   Example9 - or: %s -g gpios_pps 6\n", argv [0]);
	printf ("   Example9 - or: %s -g echo_pps 7\n\n", argv [0]);

	printf (" Add overlays modules.\n");
	printf ("   Usage:          %s overlays <overlays name>\n", argv [0]);
	printf ("   Example10:      %s overlays i2c4\n", argv [0]);
	printf ("   Example10 - or: %s overlays pps\n\n", argv [0]);

	printf (" Erase overlays modules.\n");
	printf ("   Usage:   %s erase <overlays name>\n", argv [0]);
	printf ("   Usage:   %s del <overlays name>\n", argv [0]);
	printf ("   Usage:   %s delete <overlays name>\n", argv [0]);
	printf ("   Example11:      %s erase i2c4\n", argv [0]);
	printf ("   Example11 - or: %s del pps\n", argv [0]);
	printf ("   Example11 - or: %s delete w1\n\n", argv [0]);

	printf (" Convert dtb to dts Example12:   %s dtb-dts\n", argv [0]);
	printf (" Convert dts to dtb Example12:   %s dts-dtb\n\n", argv [0]);

    if (info == ERROR_ON_OFF) exit (EXIT_FAILURE);

};

