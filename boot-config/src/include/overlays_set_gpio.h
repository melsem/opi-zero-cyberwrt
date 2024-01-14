
/*  ================================================================  */
/* Edit overlay gpio pps and w1-gpio */
void set_gpio (char *name_moduls,int argc,char *argv [])
{
   if ((strcasecmp (argv [2+ofset], "disabled") == 0) || (strcasecmp (argv [2+ofset], "off") == 0) || \
      (strcasecmp (argv [2+ofset], "OFF") == 0) || (strcasecmp (argv [2+ofset], "okay") == 0) || \
				(strcasecmp (argv [2+ofset], "on") == 0) || (strcasecmp (argv [2+ofset], "ON") == 0))
			return;

   if ((strcasecmp (argv [1+ofset], "ppspin") == 0) || (strcasecmp (argv [1+ofset], "pps") == 0)) err_help (SET_GPIO,argv);

  /* ORANGEPI_ZERO && ORANGEPI_ZERO_LTS && ORANGEPI_ZERO_H3 && ORANGEPI_ZERO_LTS && ORANGEPI_R1 && ORANGEPI_LITE2 
   * && ORANGEPI_ZEROPLUS2_H3 && ORANGEPI_3 && ORANGEPI_ZEROPLUS2_H5 && ORANGEPI_ZEROPLUS
   			* 26_pins_GPIO_HEADER */
  // int gpio_array [GPIO_20_HEADER_26] = { 0, 1, 2, 3, 5, 6, 7,10,11,12,13,14,15,16,17,18,19,198,199,362 };

  char gpio_name_array [GPIO_20_HEADER_26] [5] = {
	"PA0", "PA1", "PA2", "PA3", "PA5", "PA6", "PA7", "PA10", "PA11", "PA12",  /* ORANGEPI_ZERO - ORANGEPI_ZERO_LTS */
     "PA13", "PA14", "PA15", "PA16", "PA17", "PA18", "PA19", "PG6", "PG7", "PL10"  /* ORANGEPI_ZERO - ORANGEPI_ZERO_LTS */
  };

  int	size_name_moduls = strlen(name_moduls),
	gpio_bank,
	gpio_numb;
  char	x_pr [size_name_moduls],
	x_pins [size_name_moduls+=5],
	phandle [5],
	gpio_active [5],
	*tmp_gpio,
	gpio_name [8];
  FILE	*fd ;

    if ((strcasecmp (argv [1], "-g") == 0) && (argc == 4))
    {
	if ((strcasecmp (argv [1], "-g") == 0) && (strcasecmp (argv [1+ofset], "w1") != 0)) err_help (MODULE,argv);

	int pin = atoi (argv [2+ofset]);

	/* for (int i = 0 ; i < GPIO_20_HEADER_26 ; ++i) {
		if (atoi (argv [2+ofset]) == gpio_array [0] [i])
				printf ("  gpio_array: %d \n", gpio_array [0] [i]);
	   }; */

	gpio_bank = pin >> 5;
	if (gpio_bank > 11) err_help (NUM_GPIO,argv);
	gpio_numb = pin - (gpio_bank << 5);
			//printf ("\n    line: %d pin:%d gpio_bank:%d gpio_numb:%d\n", __LINE__, pin, gpio_bank, gpio_numb);

	if ((gpio_bank == 0) && (gpio_numb <= 21)) tmp_gpio = "PA";
	else if (gpio_bank == 1) err_help (NUM_GPIO,argv);	// "PB"
	else if ((gpio_bank == 2) && (gpio_numb <= 16)) tmp_gpio = "PC";
	else if ((gpio_bank == 3) && (gpio_numb <= 17)) tmp_gpio = "PD";
	else if ((gpio_bank == 4) && (gpio_numb <= 15)) tmp_gpio = "PE";
	else if ((gpio_bank == 5) && (gpio_numb <= 6)) tmp_gpio = "PF";
	else if ((gpio_bank == 6) && (gpio_numb <= 13)) tmp_gpio = "PG";
	else if (gpio_bank == 7) err_help (NUM_GPIO,argv);	// "PH"
	else if (gpio_bank == 8) err_help (NUM_GPIO,argv);	// "PI"
	else if (gpio_bank == 9) err_help (NUM_GPIO,argv);	// "PJ"
	else if (gpio_bank == 10) err_help (NUM_GPIO,argv);	// "PK"
	else if ((gpio_bank == 11) && (gpio_numb <= 11)) tmp_gpio = "PL";
	else err_help (NUM_GPIO,argv);
	sprintf (gpio_name,"%s%d",tmp_gpio,gpio_numb);
    }
    else if (argc == 3)
    {
	if (strncasecmp (strdup(argv [2+ofset]), "PA",2) == 0) { gpio_bank = 0x00; tmp_gpio = "PA"; }
	else if (strncasecmp (strdup(argv [2+ofset]), "PB",2) == 0) err_help (PIN_NAME,argv);	// "PB"
	else if (strncasecmp (strdup(argv [2+ofset]), "PC",2) == 0) { gpio_bank = 0x02; tmp_gpio = "PC"; }
	else if (strncasecmp (strdup(argv [2+ofset]), "PD",2) == 0) { gpio_bank = 0x03; tmp_gpio = "PD"; }
	else if (strncasecmp (strdup(argv [2+ofset]), "PE",2) == 0) { gpio_bank = 0x04; tmp_gpio = "PE"; }
	else if (strncasecmp (strdup(argv [2+ofset]), "PF",2) == 0) { gpio_bank = 0x05; tmp_gpio = "PF"; }
	else if (strncasecmp (strdup(argv [2+ofset]), "PG",2) == 0) { gpio_bank = 0x06; tmp_gpio = "PG"; }
	else if (strncasecmp (strdup(argv [2+ofset]), "PH",2) == 0) err_help (PIN_NAME,argv);	// "PH"
	else if (strncasecmp (strdup(argv [2+ofset]), "PI",2) == 0) err_help (PIN_NAME,argv);	// "PI"
	else if (strncasecmp (strdup(argv [2+ofset]), "PJ",2) == 0) err_help (PIN_NAME,argv);	// "PJ"
	else if (strncasecmp (strdup(argv [2+ofset]), "PK",2) == 0) err_help (PIN_NAME,argv);	// "PK"
	else if (strncasecmp (strdup(argv [2+ofset]), "PL",2) == 0) { gpio_bank = 0x0b; tmp_gpio = "PL"; }
	else err_help (PIN_NAME,argv);

	char t_gpio_name[5], t_gpio_num[2];

	int size_gpio_name = strlen(strdup(argv [2+ofset]));

	sprintf (t_gpio_name,"%s",strdup(argv [2+ofset]));

	if ((size_gpio_name == 3) || (size_gpio_name == 4)) {
		t_gpio_num [0] = t_gpio_name [2];
		t_gpio_num [1] = t_gpio_name [3];
	} else err_help (PIN_NAME,argv);

	if ((atoi (t_gpio_num) < 0) || (atoi (t_gpio_num) > 31)) err_help (PIN_NAME,argv);

	gpio_numb = atoi (t_gpio_num);

	if ((gpio_bank == 0x0) && (gpio_numb > 21)) err_help (PIN_NAME,argv);		// "PA"
	else if ((gpio_bank == 0x02) && (gpio_numb > 16)) err_help (PIN_NAME,argv);	// "PC"
	else if ((gpio_bank == 0x03) && (gpio_numb > 17)) err_help (PIN_NAME,argv);	// "PD"
	else if ((gpio_bank == 0x04) && (gpio_numb > 15)) err_help (PIN_NAME,argv);	// "PE"
	else if ((gpio_bank == 0x05) && (gpio_numb > 6)) err_help (PIN_NAME,argv);	// "PF"
	else if ((gpio_bank == 0x06) && (gpio_numb > 13)) err_help (PIN_NAME,argv);	// "PG"
	else if ((gpio_bank == 0x0b) && (gpio_numb > 11)) err_help (PIN_NAME,argv);	// "PL"
	sprintf (gpio_name,"%s%d",tmp_gpio,gpio_numb);
    }
		//printf ("\n  line: %d  gpio_name: %s\n  gpio_bank: %d\n  gpio_numb: %d\n\n", __LINE__,gpio_name,gpio_bank,gpio_numb);

   for (int i = 0 ; i < GPIO_20_HEADER_26 ; ++i) 
   {
	if (strcasecmp (gpio_name, gpio_name_array [i]) == 0) {

	sprintf (x_pr,"%s", name_moduls);
	sprintf (x_pins,"%s_pins", name_moduls);

		/* ------- Recording gpio_name ------- */
	sprintf (tempraw,"sed -i '/%s {/,/\\};/ {s/pins.*\"\\;/pins = \"%s\"\\;/}' %s", x_pins, gpio_name, tmp_dts);
	system (tempraw);  //printf ("\n--- Recording gpio_name --\n  line: %d < %s >\n\n", __LINE__,tempraw);

		/* ---- Calculate phandle------- */
	sprintf (tempraw,"sed -n '/%s {/,/\\phandle/p' %s | sed -n 's/.*phandle = <\\(.*\\)>\\;/\\1/p' > /tmp/phandle", pinctrl, tmp_dts);
	system (tempraw); //printf ("\n--- Calculate phandle --\n  line: %d < %s >\n\n", __LINE__,tempraw);
	if ((fd = fopen ("/tmp/phandle", "r")) != NULL) { if ( fgets (phandle, 5, fd) ){;} fclose (fd); remove("/tmp/phandle"); }

		/* -- Calculate gpio_active------ */
	sprintf (tempraw,"sed -n '/%s {/,/gpios/p' %s | sed -n 's/.*gpios = <%s \\(.*\\) \\(.*\\) \\(.*\\)>\\;/\\3/p' > /tmp/gpio_active", x_pr, tmp_dts, phandle);
	system (tempraw);  //printf ("\n--- Calculate gpio_active --\n  line: %d < %s >\n\n", __LINE__,tempraw);
	if ((fd = fopen ("/tmp/gpio_active", "r")) != NULL) { if ( fgets (gpio_active, 5, fd) ){;} fclose (fd); remove("/tmp/gpio_active"); }

		/* ------- Recording gpio_bank, gpio_numb ------- */
	sprintf (tempraw,"sed -i '/%s {/,/\\};/ {s/gpios = <%s .* .* %s/gpios = <%s %#x %#x %s/}' %s", \
				 x_pr, phandle, gpio_active, phandle, gpio_bank, gpio_numb, gpio_active, tmp_dts);
	system (tempraw);  //printf ("\n--- Recording gpio_bank, gpio_numb --\n  line: %d < %s >\n", __LINE__,tempraw);
	exit (EXIT_FAILURE);
	};
    };

    if ((strcasecmp (argv [1], "-g") == 0) && (argc == 4)) err_help (NUM_GPIO,argv);
    else err_help (PIN_NAME,argv);
};
/*  ================================================================  */

