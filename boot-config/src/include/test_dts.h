
/*  ================================================================  */
void test_dts (int file_dts,int exi_dts) {
  FILE *tempfile;

	if (((tempfile = fopen(tmp_dts, "r")) == NULL) || 
				(file_dts == TEST_DTS_MMC) || (file_dts == TEST_DTS_SPI) || 
						(file_dts == DTB_TO_DTS_MMC) || (file_dts == DTB_TO_DTS_SPI))
	{
		if ((tempfile = fopen(tmp_dts, "r")) == NULL)
				printf ("   Failed to open missing file: %s\n", tmp_dts);
//		sleep (1);

		if ((file_dts == TEST_DTS_MMC) || (file_dts == DTB_TO_DTS_MMC))	/* --- file converted. mmc boot_dtb -> dts ------- */
		{	sprintf (tempraw,"dtc -I dtb -O dts -o %s %s &> %s", tmp_dts, boot_dtb, dev_null);
			system (tempraw);
//		sleep (1);
		}

		else if ((file_dts == TEST_DTS_SPI) || (file_dts == DTB_TO_DTS_SPI)) /* --- file converted. spi dtb_mtd -> dts ------- */
		{	sprintf (tempraw,"dtc -I dtb -O dts -o %s %s &> %s", tmp_dts, dtb_mtd, dev_null);
			system (tempraw);
//		sleep (1);
		}


		if ((tempfile = fopen(tmp_dts, "r")) == NULL) {	// Missing file tmp_dts??
			printf ("  Error: Missing file: %s !!\n  DTB is not converted..\n   EXIT!!\n", tmp_dts);
			if (exi_dts != OVERLAYS)
					exit (EXIT_FAILURE);
		}
		else		// File converted dtb_mtd to tmp_dts - OK
		{
			printf ("   %s file converted.. OK\n", tmp_dts);
			if (exi_dts == DTB_TO_DTS)
						exit (EXIT_FAILURE);
		}
	}

//	*****************	//

	if ((file_dts == DTS_TO_DTB_MMC) || (file_dts == DTS_TO_DTB_SPI))
	{
		sprintf (tempraw,"dtc -I dts -O dtb -o %s %s &> %s",dtb_tmp, tmp_dts, dev_null);
		system (tempraw);

		if ((tempfile = fopen(dtb_tmp, "r")) == NULL) {	// Missing file dtb_tmp??
			printf ("  Error: Missing file: %s !!\n  dtb_tmp is not converted..\n   EXIT!!\n", dtb_tmp);
			if (exi_dts != OVERLAYS)
					exit (EXIT_FAILURE);
		}
		else		// File converted dts to dtb_mtd - OK //Writing a dtb_tmp file to a flash drive. - OK.
		{
			printf ("   File %s converted to %s - OK.\n", tmp_dts, dtb_tmp);

			if (file_dts == DTS_TO_DTB_MMC)			// Writing a dtb_tmp file to a MMC-flash drive.
					sprintf (tempraw,"dtc -I dts -O dtb -o %s %s &> %s",boot_dtb, tmp_dts, dev_null);
			else if (file_dts == DTS_TO_DTB_SPI)		// Writing a dtb_tmp file to a SPI-flash drive.
					sprintf (tempraw,"mtd -e dtb write %s dtb &> %s",dtb_tmp, dev_null);

			system (tempraw);

			printf ("   Writing a %s file to a flash drive. - OK.\n", dtb_tmp);
			remove (dtb_tmp);
			remove (tmp_dts);
			remove (tmp_SPI);
			if (exi_dts == CONV)
					exit (EXIT_FAILURE);
		}
	}
};
/*  ================================================================  */
