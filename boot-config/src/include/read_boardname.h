
/*  ================================================================  */
/* Read boardname */

void read_boardname ()
{
  FILE *tempfile ;
	if ((tempfile = fopen ("/proc/device-tree/model", "r")) != NULL)
	{
		fseek (tempfile, 0, SEEK_END);
		size = ftell (tempfile);
		rewind (tempfile);
		boardname = calloc (size + 1, 1);
		fread (boardname, 1, size, tempfile);
		fclose (tempfile);
		size -= 1;
	}
	else exit (EXIT_FAILURE);
};
/* End read_boardname */
/*  ================================================================  */

