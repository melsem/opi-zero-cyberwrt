#!/usr/bin/awk -f
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
function unescape(s)
{
gsub(/\+/," ",s)
res = ""
	do {
	  p = match(s,/%[0-9a-fA-F]{2}/)
	  if(p>0) {
	  res = res substr(s,0,p-1) sprintf("%c",0+("0x" substr(s,p+1,2)))
	  s = substr(s,p+3)
	  }
	} while(p>0)
	return res s
}
BEGIN
{
RS = "&"
FS = "="
print "Content-type: text/html; charset=utf-8"
print ""
}
{
	if($1 == "search")
	{
	system("uci set ds18b20a_conf.@Gpio1Wr[0].search='"unescape($2) "'")
	}
	if($1 == "enbled0")
	{
	system("uci set ds18b20a_conf.@Gpio1Wr[0].enabled0='"unescape($2) "'")
	}
	if($1 == "ftp_svr")
	{
	system("uci set ds18b20a_conf.@Gpio1Wr[0].ftp_svr='"unescape($2) "'")
	}
	if($1 == "pcf8574")
	{
	system("uci set ds18b20a_conf.@Gpio1Wr[0].pcf8574='" unescape($2) "'")
	}
	if($1 == "interval")
	{
	system("uci set ds18b20a_conf.@Gpio1Wr[0].times='" unescape($2) "'")
	}
	if($1 == "init_port")
	{
	system("uci set ds18b20a_conf.@Gpio1Wr[0].gpio1wr='" unescape($2) "'")
	}
	if($1 == "srv_d")
	{
	system("uci set ds18b20a_conf.@Gpio1Wr[0].serv_d='" unescape($2) "'")
	}
	if($1 == "srv_port")
	{
	system("uci set ds18b20a_conf.@Gpio1Wr[0].serv_port='" unescape($2) "'")
	}
	if($1 == "adres_i2c")
	{
	system("uci set ds18b20a_conf.@Gpio1Wr[0].hexadr='" unescape($2) "'")
	}
	if($1 == "sda_scl_i2c")
	{
	system("uci set ds18b20a_conf.@Gpio1Wr[0].sda_scl='" unescape($2) "'")
	}
	if($1 == "allset")
	{
	system("uci set ds18b20a_conf.@Gpio1Wr[0].allset='" unescape($2) "'")
	}
	if($1 == "pu_Skryp")
	{
	system("uci set ds18b20a_conf.@Gpio1Wr[0].pu_Skryp='" unescape($2) "'")
	}
	if($1 == "imya_skript")
	{
	system("uci set ds18b20a_conf.@Gpio1Wr[0].imya_skript='" unescape($2) "'")
	}
	if($1 == "staron")
	{
	system("/etc/init.d/dserr18usba_init enable")
	system("/etc/init.d/ds18b20usba_init enable")
	print "<br>AUTO-START ENABLE"
	}
	if($1 == "staroff")
	{
	system("/etc/init.d/dserr18usba_init disable")
	system("/etc/init.d/ds18b20usba_init disable")
	print "<br>AUTO-START DISABLE"
	}
	if($1 == "commit")
	{
	system("uci commit ds18b20a_conf")
	print "<br>Setup Global."
	print "<br>Save OK."
	}

	if($1 == "cozapusk")
	{
	system("/etc/init.d/dserr18usba_init restart")
	system("/etc/init.d/ds18b20usba_init restart")
	print "<br>START - OK."
	system("sleep 2s")
	}
	if($1 == "coostanov")
	{
	system("/etc/init.d/dserr18usba_init stop")
	system("/etc/init.d/ds18b20usba_init stop")
	print "STOP - OK."
	}

	if($1 == "text")
	{
	print unescape($2) | "sed 's/\r//' > /usr/sbin/lcd_usb_hd44780_term"
	system("chmod 755 /usr/sbin/lcd_usb_hd44780_term")
	print "<br>Save OK 2."
	}

}
END
{
	system("sleep 1")
	system("/www/cgi-bin/modules/termostatUsb/Sset.cgi")
}
