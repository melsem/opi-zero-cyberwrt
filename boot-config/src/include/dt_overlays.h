void dts_add_overlays (int i)
{
  FILE	*fd ;
	if ((fd = fopen (tmp_files, "r")) != NULL) {
		fseek (fd, 0, SEEK_END);
		size = ftell (fd);
		rewind (fd);
		if (i == 0) {
			moduls = calloc (size + 1, 1);
			fread (moduls, 1, size-1, fd);
		} else if (i == 1) {
			add_aliases = calloc (size + 1, 1);
			fread (add_aliases, 1, size-1, fd);
		} else if (i == 2) {
			pin_reg = calloc (size + 1, 1);
			fread (pin_reg, 1, size-1, fd);
		}
		fclose (fd);
		remove(tmp_files);
	}

};

void dt_overlays (int argc,char *argv []) {

  char	number_of_strings [4];	/* количество строк */
  FILE	*fd ;
	if (((fd = fopen (tmp_SPI, "r")) == NULL) && ((fd = fopen(tmp_dts, "r")) == NULL))
									test_dts (TEST_DTS_MMC, DTB_TO_DTS);
	else if (((fd = fopen (tmp_SPI, "r")) != NULL) && ((fd = fopen(tmp_dts, "r")) == NULL))
									test_dts (TEST_DTS_SPI, DTB_TO_DTS);

  sprintf (tempraw,"sed -n '/\\(.*\\) {/,/;/p' %s | sed -n 's/\\(.*\\) {/\\1/p; s/^[ \\t]*//' | sed -n '$=' > %s", tmp_dts, tmp_files);
  system (tempraw);
  if ((fd = fopen (tmp_files, "r")) != NULL) {
		if ( fgets (number_of_strings, 4, fd) ){;} fclose (fd); remove(tmp_files); }
  else exit (EXIT_FAILURE);
			/* number_of_strings - OK */

	int nmbstr = atoi (number_of_strings);
	for (int i = 1; i <= nmbstr; i++) {

		sprintf (tempraw,"sed ''%d'!d' %s > %s", i, f_moduls, tmp_files);
		system (tempraw);
		dts_add_overlays (0);
		if ( size <= 1 ) goto end_for;
			/* moduls - OK */


		sprintf (tempraw,"sed ''%d'!d' %s > %s", i, f_aliases, tmp_files);
		system (tempraw);
		dts_add_overlays (1);
		if ( size <= 1 ) goto end_for;
			/* add_aliases - OK */


		sprintf (tempraw,"sed -n '/%s {/,/;/p' %s | sed -n 's/\\(.*\\) {/\\1/p' | sed 's/^[ \\t]*//' > %s", moduls, tmp_dts, tmp_files);
		system (tempraw);
		dts_add_overlays (2);
		if ( size <= 1 ) goto end_for;
			/* pin_reg - OK */


		sprintf (tempraw,"sed -i '/%s {/,/\\};/ {s/%s {/%s %s {/}' %s", moduls, pin_reg, add_aliases, pin_reg, tmp_dts);
		system (tempraw);
			// printf ("     -- %d --\n  line: %d - %s\n\n", i, __LINE__,tempraw);
		end_for: ;
	}

	sprintf (tempraw,"cat /etc/dt-overlays/%s_overlays.dts >> %s &> %s", argv [2+ofset], tmp_dts, dev_null);

	if (system (tempraw))
		printf ("\n  Error: %s_overlays.dts is missing\n  or check the name you entered.\n\n", argv [2+ofset]);
	else
	{
//		sleep (1);
		if (((fd = fopen (tmp_SPI, "r")) == NULL) && ((fd = fopen(tmp_dts, "r")) != NULL))
									test_dts (DTS_TO_DTB_MMC, OVERLAYS);
		else if (((fd = fopen (tmp_SPI, "r")) != NULL) && ((fd = fopen(tmp_dts, "r")) != NULL))
									test_dts (DTS_TO_DTB_SPI, OVERLAYS);
	}
	exit (EXIT_FAILURE);
};

