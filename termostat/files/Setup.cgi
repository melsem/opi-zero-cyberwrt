#!/usr/bin/awk -f
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
	system("uci set ds18b20.@Gpio1Wr[0].search='"unescape($2) "'")
	}
	if($1 == "enbled0")
	{
	system("uci set ds18b20.@Gpio1Wr[0].enabled0='"unescape($2) "'")
	}
	if($1 == "ftp_svr")
	{
	system("uci set ds18b20.@Gpio1Wr[0].ftp_svr='"unescape($2) "'")
	}
	if($1 == "pcf8574")
	{
	system("uci set ds18b20.@Gpio1Wr[0].pcf8574='" unescape($2) "'")
	}
	if($1 == "interval")
	{
	system("uci set ds18b20.@Gpio1Wr[0].times='" unescape($2) "'")
	}
	if($1 == "init_port")
	{
	system("uci set ds18b20.@Gpio1Wr[0].gpio1wr='" unescape($2) "'")
	}
	if($1 == "srv_d")
	{
	system("uci set ds18b20.@Gpio1Wr[0].serv_d='" unescape($2) "'")
	}
	if($1 == "srv_port")
	{
	system("uci set ds18b20.@Gpio1Wr[0].serv_port='" unescape($2) "'")
	}

	if($1 == "d_ay")
	{
	system("uci set ds18b20.@Gpio1Wr[0].d_ay='" unescape($2) "'")
	}

	if($1 == "adres_i2c")
	{
	system("uci set ds18b20.@Gpio1Wr[0].hexadr='" unescape($2) "'")
	}
	if($1 == "sda_scl_i2c")
	{
	system("uci set ds18b20.@Gpio1Wr[0].sda_scl='" unescape($2) "'")
	}
	if($1 == "allset")
	{
	system("uci set ds18b20.@Gpio1Wr[0].allset='" unescape($2) "'")
	}

	if($1 == "oInvert")
	{
	system("uci set ds18b20.@Gpio1Wr[0].oInvert='" unescape($2) "'")
	}

	if($1 == "pu_Skryp")
	{
	system("uci set ds18b20.@Gpio1Wr[0].pu_Skryp='" unescape($2) "'")
	}
	if($1 == "imya_skript")
	{
	system("uci set ds18b20.@Gpio1Wr[0].imya_skript='" unescape($2) "'")
	}
	if($1 == "staron")
	{
	system("/etc/init.d/ds18b20_init enable")
	print "<br>AUTO-START ENABLE"
	}
	if($1 == "staroff")
	{
	system("/etc/init.d/ds18b20_init disable")
	print "<br>AUTO-START DISABLE"
	}
	if($1 == "commit")
	{
	system("uci commit ds18b20")
	print "<br>Setup Global."
	print "<br><b>Save OK.</b>"
	}
	if($1 == "cozapusk")
	{
	system("/etc/init.d/ds18b20_init restart")
	print "<br><b>START - OK.</b><br>"
	print "<b>search sensor:<b><br> >>"
	system("sleep 4s")
	}
	if($1 == "coostanov")
	{
	system("/etc/init.d/ds18b20_init stop")
	print "STOP - OK."
	}
	if($1 == "text")
	{
	print unescape($2) | "sed 's/\r//' > /usr/sbin/skript_term"
	system("chmod 755 /usr/sbin/skript_term")
	print "<br><b> Save Script OK</b>"
	}
}
END
{
print "<br><br><br><table align=center border=0 size=18 width=><tr><td align=center bgcolor=#8891f1><b><a href=/cgi-bin/modules/termostatGpio/Sset.cgi>In settings</a></b></td></tr><tr><td></td></tr><tr><td align=center bgcolor=#90f1f1><b><a href=/cgi-bin/modules/termostatGpio/index.cgi>Exit settings</a></b></td></tr></table>"
}



