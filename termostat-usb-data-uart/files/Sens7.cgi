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
	if($1 == "id_dev")
	{
	system("uci set ds18b20a_conf.@ds18b20_s7[0].id_sensor='"unescape($2) "'")
	}
	if($1 == "hster")
	{
	system("uci set ds18b20a_conf.@ds18b20_s7[0].hister='" unescape($2) "'")
	}
	if($1 == "grdus")
	{
	system("uci set ds18b20a_conf.@ds18b20_s7[0].gradus='" unescape($2) "'")
	}
	if($1 == "pwut")
	{
	system("uci set ds18b20a_conf.@ds18b20_s7[0].pwout='" unescape($2) "'")
	}
	if($1 == "tipsnsr")
	{
	system("uci set ds18b20a_conf.@ds18b20_s7[0].tipsensr='" unescape($2) "'")
	}
	if($1 == "flder")
	{
	system("uci set ds18b20a_conf.@ds18b20_s7[0].folder='" unescape($2) "'")
	}

	if($1 == "narmon7")
	{
	system("uci set ds18b20a_conf.@ds18b20_s7[0].enab_nar='" unescape($2) "'")
	system("uci commit ds18b20a_conf")
	print "<br>narodmon - Sensor 7 Save OK."
	}
	if($1 == "commit7")
	{
	system("uci commit ds18b20a_conf")
	print "<br>Sensor 7 Save OK."
	}
}
END
{
	system("sleep 1")
	system("/www/cgi-bin/modules/termostatUsb/Sset.cgi")
}
