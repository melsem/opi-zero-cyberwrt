
void mini_help (int info,char *argv [])
{
    if (info == ERROR_ON_OFF) usage1 (info,argv);

    else if (info == HELP) {
	printf ("\n\n ------------------->  %-35s  <-------------------\n",boardname);
	printf (" Convert dtb file to dts:\n");
	printf ("   Example: %s dtb-dts-mmc\n", argv [0]);
	printf (" Convert dtb file to dts:\n");
	printf ("   Example: %s dts-dtb-mmc\n\n", argv [0]);
	printf (" Modules that can be controlled by commands: <on/ON/okay> or <off/OFF/disabled>.\n\n");
	printf ("   mmc0/mmc1/mmc2/ir/pwm/i2c0/i2c1/i2c2/camera/audcodec\n");
	printf ("   i2s0/i2s1/uart0/uart1/uart2/otg/host/peripheral/spdif\n");
	printf ("   ehci0/ohci0/ehci1/ohci1/ehci2/ohci2/ehci3/ohci3\n");
	printf ("   usb0/usb1/usb2/usb3/spi0/spi1/ethernet0\n");
	printf ("   otg_tipe/host_tipe/peripheral_tipe\n");
#if CONFIG_overlays	/* dt_overlays. */
#if CONFIG_overlay_1wire || CONFIG_overlay_pps || CONFIG_overlay_gpio_keys_user || \
						CONFIG_overlay_rotary_encoder || CONFIG_overlay_i2c4
	printf ("   w1/pps/i2c4/gpio-keys-user/rotary/rotary_button/rotary_axis\n");
#endif
#endif			/* End dt_overlays. */
	usage1 (info,argv);
	printf (" -------------------------------------------------------------------------------\n\n");
    }

#if CONFIG_overlays	/* dt_overlays. */
#if CONFIG_overlay_1wire || CONFIG_overlay_pps || \
			CONFIG_overlay_gpio_keys_user || \
				CONFIG_overlay_rotary_encoder || CONFIG_overlay_i2c4
    else if (info == OVERLAYS)
    {
	printf ("\n Overlays-module: <<< %s >>> does not exist.\n   Check the names in the help: ' %s -h '\n\n", argv [2+ofset], argv [0]);
	exit (EXIT_FAILURE);
    }
#endif
#endif			/* End dt_overlays. */

#if CONFIG_overlays	/* dt_overlays. */
#if CONFIG_overlay_1wire || CONFIG_overlay_pps
    else if (info == MODULE)
	printf ("\n   Module: <<< %s >>> does not exist.\n   Check the names in the help: ' %s -h '\n\n", argv [1+ofset], argv [0]);
    else if (info == PIN_NAME)
	printf ("\n   Invalid PINCTRL_PIN name: <<< %s >>>\n\n",strdup(argv [2+ofset]));
    else if (info == NUM_GPIO)
 	printf ("\n   The gpio: <<< %d >>> number does not match the pinctrl SoC driver.\n\n",atoi(argv [2+ofset]));
#endif
#endif			/* End dt_overlays. */
    exit (EXIT_FAILURE);
};

void usage1 (int info,char *argv [])
{
	printf ("\n   Usage:   %s <module> <on/ON/okay>\n", argv [0]);
	printf ("   Or:      %s <module> <off/OFF/disabled>\n", argv [0]);
	printf ("   Example1: %s i2c0 on\n", argv [0]);
	printf ("   Example2: %s usb2 off\n\n", argv [0]);
	printf ("     ------------------------------------------------\n");
	printf ("       Example3: %s otg_tipe on\n", argv [0]);
	printf ("       Or:       %s host_tipe on\n", argv [0]);
	printf ("       Or:       %s peripheral_tipe on\n\n", argv [0]);
	printf ("     Or all USB0 tips disable (otg, host, peripheral).\n");
	printf ("       example:  %s host_tipe off\n", argv [0]);
	printf ("     ------------------------------------------------\n\n");
#if CONFIG_overlays	/* dt_overlays. */
#if CONFIG_overlay_1wire || CONFIG_overlay_pps
    if (info == HELP) printf (" In the modules 'w1', 'pps' you can still change the pins.\n");
	printf ("   Add or edit single one-wire, pps contacts.\n\n");
	printf ("   Usage:   %s -g <module> <gpio number>\n", argv [0]);
	printf ("   Example4: %s -g w1 10\n\n", argv [0]);
	printf ("   Usage:   %s <module> <gpio name>\n", argv [0]);
	printf ("   Example5: %s w1 PA10\n\n", argv [0]);
#endif
#endif			/* End dt_overlays. */
    if (info == ERROR_ON_OFF) exit (EXIT_FAILURE);

};

