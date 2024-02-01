

/*  ================================================================  */
/* Enable-disable modules */
void okay_disabled (char *name_moduls, char *on_off)
{	/* ------- Recording "okay" or "disabled" ------- */
	sprintf (tempraw,"sed -i '/%s {/,/\\};/ {s/%s/}' %s", name_moduls, on_off, tmp_dts);
	system (tempraw);
	// printf ("  line: %d  %s\n", __LINE__,tempraw);
};
/* END Enable-disable modules */
/*  ================================================================  */

