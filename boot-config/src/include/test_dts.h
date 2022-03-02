
/*  ================================================================  */
void test_dts (int file_dts,int exi_dts) {
  FILE *tempfile;

	if ((file_dts == TEST_DTS_MMC) || (file_dts == TEST_DTS_SPI))	/* --- file converted. mmc/dtb -> dts ------- */
	{
	    if ((tempfile = fopen(tmp_dts, "r")) == NULL) {			/* Missing file dts?? */
		printf ("   Failed to open missing file: %s\n", tmp_dts);
		sleep (1);

		if ((tempfile = fopen(boot_dtb, "r")) == NULL) {	/* Missing file dtb?? */
			printf ("   Missing file: %s !!\n   Disk /boot is not mounted..\n   EXIT!!\n", boot_dtb);
			if (exi_dts != OVERLAYS)
					exit (EXIT_FAILURE);
		}		/* File dtb - OK */

		if (file_dts == TEST_DTS_MMC)	/* --- file converted. mmc/dtb -> dts ------- */
		{
			sprintf (tempraw,"dtc -I dtb -O dts -o %s %s 2> /dev/null", tmp_dts, boot_dtb);
			system (tempraw);
			printf ("  /boot/dtb to %s file converted..\n", tmp_dts);
			sleep (1);
			printf ("   We execute the command.\n");
		}

		/* else if (file_dts == TEST_DTS_SPI)	// --- file converted. spi dtb_mtd -> dts ------- 
		{	
			sprintf (tempraw,"dtc -I dtb -O dts -o %s %s 2> /dev/null", tmp_dts, dtb_mtd);
			system (tempraw);
			printf ("   %s file converted..\n", tmp_dts);
		}	*/
	    }
	}

	else if (file_dts == DTB_TO_DTS_MMC)	/* --- file converted. mmc/dtb -> dts ------- */
	{
		sprintf (tempraw,"dtc -I dtb -O dts -o %s %s 2> /dev/null", tmp_dts, boot_dtb);
		system (tempraw);
		sleep (1);
		if ((tempfile = fopen(tmp_dts, "r")) == NULL) {	/* Missing file dtb?? */
			printf ("  Error: Missing file: %s !!\n  /boot/dtb is not converted..\n   EXIT!!\n", tmp_dts);
			if (exi_dts != OVERLAYS)
					exit (EXIT_FAILURE);
		}
		else		/* File converted /boot/dtb to tmp_dts - OK */
		{
			printf ("  /boot/dtb file converted to %s..\n", tmp_dts);
			if (exi_dts != OVERLAYS)
					exit (EXIT_FAILURE);
		}
	}

	else if (file_dts == DTS_TO_DTB_MMC)
	{
		sprintf (tempraw,"dtc -I dts -O dtb -o %s %s 2> /dev/null",boot_dtb,tmp_dts);
		system (tempraw);

		if ((tempfile = fopen(boot_dtb, "r")) == NULL) {	/* Missing file dtb?? */
			printf ("  Error: Missing file: %s !!\n  /boot/dtb is not converted..\n   EXIT!!\n", boot_dtb);
			if (exi_dts != OVERLAYS)
					exit (EXIT_FAILURE);
		}
		else		/* File converted dts to dtb - OK */
		{
			if (exi_dts != OVERLAYS)
			{
				printf ("  %s file converted to /boot/dtb.\n", tmp_dts);
					exit (EXIT_FAILURE);
			}
		}
	}

	/* else if (file_dts == DTB_TO_DTS_SPI)	// --- file converted. spi dtb_mtd -> dts ------- //
	{	
		sprintf (tempraw,"dtc -I dtb -O dts -o %s %s 2> /dev/null", tmp_dts, dtb_mtd);
		system (tempraw);
		if ((tempfile = fopen(tmp_dts, "r")) == NULL) {	// Missing file dtb-mtd??
			printf ("  Error: Missing file: %s !!\n  dtb_mtd is not converted..\n   EXIT!!\n", tmp_dts);
			if (exi_dts != OVERLAYS)
					exit (EXIT_FAILURE);
		}
		else		// File converted dtb_mtd to tmp_dts - OK
		{
			printf ("   %s file converted..\n", tmp_dts);
			if (exi_dts != OVERLAYS)
					exit (EXIT_FAILURE);
		}
	}
	else if (file_dts == DTS_TO_DTB_SPI)
	{
		sprintf (tempraw,"dtc -I dts -O dtb -o %s %s 2> /dev/null",dtb_mtd,tmp_dts);
		system (tempraw);

		if ((tempfile = fopen(dtb_mtd, "r")) == NULL) {	// Missing file dtb-mtd??
			printf ("  Error: Missing file: %s !!\n  dtb_mtd is not converted..\n   EXIT!!\n", dtb_mtd);
			if (exi_dts != OVERLAYS)
					exit (EXIT_FAILURE);
		}
		else		// File converted dts to dtb_mtd - OK
		{
			printf ("   %s file converted to dtb_mtd - OK.\n", tmp_dts);
			if (exi_dts != OVERLAYS)
					exit (EXIT_FAILURE);
		}
	}	*/
};
/*  ================================================================  */
