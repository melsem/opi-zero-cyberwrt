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

	if($1 == "device") system("uci set i2c_HD44780.@display[0].device='"unescape($2)"'")
	if($1 == "address") system("uci set i2c_HD44780.@display[0].address='"unescape($2)"'")
	if($1 == "sda_gpio") system("uci set i2c_HD44780.@display[0].sda_gpio='"unescape($2)"'")
	if($1 == "scl_gpio") system("uci set i2c_HD44780.@display[0].scl_gpio='"unescape($2)"'")
	if($1 == "lcd_number") system("uci set i2c_HD44780.@display[0].lcd_number='"unescape($2)"'")
	if($1 == "gros_simvol") system("uci set i2c_HD44780.@display[0].gros_simvol='"unescape($2)"'")
	if($1 == "info_banner") system("uci set i2c_HD44780.@display[0].info_banner='"unescape($2)"'")
	if($1 == "read_file") system("uci set i2c_HD44780.@display[0].read_file='"unescape($2)"'")
	if($1 == "while") system("uci set i2c_HD44780.@displ_tip[0].while='"unescape($2)"'")
	if($1 == "clock") system("uci set i2c_HD44780.@displ_tip[0].clock='"unescape($2)"'")
	if($1 == "led_on") system("uci set i2c_HD44780.@displ_tip[0].led_on='"unescape($2)"'")
	if($1 == "on_read") system("uci set i2c_HD44780.@displ_tip[0].on_read='"unescape($2)"'")
	if($1 == "on_ds18") system("uci set i2c_HD44780.@displ_tip[0].on_ds18='"unescape($2)"'")
	if($1 == "w1_gpio") system("uci set i2c_HD44780.@ds18b20[0].w1_gpio='"unescape($2)"'")
	if($1 == "usb_w1_uart") system("uci set i2c_HD44780.@ds18b20[0].usb_w1_uart='"unescape($2)"'")

	if($1 == "name_snr1") system("uci set i2c_HD44780.@ds18b20[0].name_snr1='"unescape($2)"'")
	if($1 == "name_snr2") system("uci set i2c_HD44780.@ds18b20[0].name_snr2='"unescape($2)"'")
	if($1 == "name_snr3") system("uci set i2c_HD44780.@ds18b20[0].name_snr3='"unescape($2)"'")
	if($1 == "name_snr4") system("uci set i2c_HD44780.@ds18b20[0].name_snr4='"unescape($2)"'")

	if($1 == "sensor1") system("uci set i2c_HD44780.@ds18b20[0].sensor1='"unescape($2)"'")
	if($1 == "sensor2") system("uci set i2c_HD44780.@ds18b20[0].sensor2='"unescape($2)"'")
	if($1 == "sensor3") system("uci set i2c_HD44780.@ds18b20[0].sensor3='"unescape($2)"'")
	if($1 == "sensor4") system("uci set i2c_HD44780.@ds18b20[0].sensor4='"unescape($2)"'")

	if($1 == "histe1") system("uci set i2c_HD44780.@ds18b20[0].histe1='"unescape($2)"'")
	if($1 == "histe2") system("uci set i2c_HD44780.@ds18b20[0].histe2='"unescape($2)"'")
	if($1 == "histe3") system("uci set i2c_HD44780.@ds18b20[0].histe3='"unescape($2)"'")
	if($1 == "histe4") system("uci set i2c_HD44780.@ds18b20[0].histe4='"unescape($2)"'")

	if($1 == "alarm1") system("uci set i2c_HD44780.@ds18b20[0].alarm1='"unescape($2)"'")
	if($1 == "alarm2") system("uci set i2c_HD44780.@ds18b20[0].alarm2='"unescape($2)"'")
	if($1 == "alarm3") system("uci set i2c_HD44780.@ds18b20[0].alarm3='"unescape($2)"'")
	if($1 == "alarm4") system("uci set i2c_HD44780.@ds18b20[0].alarm4='"unescape($2)"'")

	if($1 == "tip_ds1") system("uci set i2c_HD44780.@displ_tip[0].tip_ds1='"unescape($2)"'")
	if($1 == "tip_ds2") system("uci set i2c_HD44780.@displ_tip[0].tip_ds2='"unescape($2)"'")
	if($1 == "tip_ds3") system("uci set i2c_HD44780.@displ_tip[0].tip_ds3='"unescape($2)"'")
	if($1 == "tip_ds4") system("uci set i2c_HD44780.@displ_tip[0].tip_ds4='"unescape($2)"'")

	if($1 == "nGp1_out") system("uci set i2c_HD44780.@ds18b20[0].nGp1_out='"unescape($2)"'")
	if($1 == "nGp2_out") system("uci set i2c_HD44780.@ds18b20[0].nGp2_out='"unescape($2)"'")
	if($1 == "nGp3_out") system("uci set i2c_HD44780.@ds18b20[0].nGp3_out='"unescape($2)"'")
	if($1 == "nGp4_out") system("uci set i2c_HD44780.@ds18b20[0].nGp4_out='"unescape($2)"'")

	if($1 == "invers1") system("uci set i2c_HD44780.@ds18b20[0].invers1='"unescape($2) "'")
	if($1 == "invers2") system("uci set i2c_HD44780.@ds18b20[0].invers2='"unescape($2) "'")
	if($1 == "invers3") system("uci set i2c_HD44780.@ds18b20[0].invers3='"unescape($2) "'")
	if($1 == "invers4") system("uci set i2c_HD44780.@ds18b20[0].invers4='"unescape($2) "'")

	if($1 == "cozapusk") system("/etc/init.d/i2c_HD44780.init start")
	if($1 == "coostanov") system("/etc/init.d/i2c_HD44780.init stop")
	if($1 == "staron") system("/etc/init.d/i2c_HD44780.init enable")
	if($1 == "staroff") system("/etc/init.d/i2c_HD44780.init disable")
	if($1 == "save_commit") system("uci commit i2c_HD44780")

	if($1 == "save_com_restart")
	{
	system("uci commit i2c_HD44780")
	system("/etc/init.d/i2c_HD44780.init stop")
	system("sleep 1s")
	system("/etc/init.d/i2c_HD44780.init start")
	}
}
END
{
	system("sleep 1s")
	system("/www/cgi-bin/modules/i2c_hd44780/index.html")
}


