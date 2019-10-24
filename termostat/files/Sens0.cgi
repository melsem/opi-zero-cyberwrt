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
	if($1 == "logns")
	{
	system("uci set ds18b20.@ds18b20_s0[0].logns='"unescape($2) "'")
	}
	if($1 == "id_dev")
	{
	system("uci set ds18b20.@ds18b20_s0[0].id_sensor='"unescape($2) "'")
	}
	if($1 == "hster")
	{
	system("uci set ds18b20.@ds18b20_s0[0].hister='" unescape($2) "'")
	}
	if($1 == "gra0dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].gra0dus='" unescape($2) "'")
	}
	if($1 == "Tue_gra0dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Tue_gra0dus='" unescape($2) "'")
	}
	if($1 == "Wed_gra0dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Wed_gra0dus='" unescape($2) "'")
	}
	if($1 == "Thu_gra0dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Thu_gra0dus='" unescape($2) "'")
	}
	if($1 == "Fri_gra0dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Fri_gra0dus='" unescape($2) "'")
	}
	if($1 == "Sat_gra0dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sat_gra0dus='" unescape($2) "'")
	}
	if($1 == "Sun_gra0dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sun_gra0dus='" unescape($2) "'")
	}
	if($1 == "t1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].t1ime='" unescape($2) "'")
	}
	if($1 == "m1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].m1ime='" unescape($2) "'")
	}
	if($1 == "gradus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].gradus='" unescape($2) "'")
	}
	if($1 == "t2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].t2ime='" unescape($2) "'")
	}
	if($1 == "m2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].m2ime='" unescape($2) "'")
	}
	if($1 == "gra2dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].gra2dus='" unescape($2) "'")
	}
	if($1 == "t3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].t3ime='" unescape($2) "'")
	}
	if($1 == "m3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].m3ime='" unescape($2) "'")
	}
	if($1 == "gra3dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].gra3dus='" unescape($2) "'")
	}
	if($1 == "Tue_t1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Tue_t1ime='" unescape($2) "'")
	}
	if($1 == "Tue_m1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Tue_m1ime='" unescape($2) "'")
	}
	if($1 == "Tue_gradus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Tue_gradus='" unescape($2) "'")
	}
	if($1 == "Tue_t2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Tue_t2ime='" unescape($2) "'")
	}
	if($1 == "Tue_m2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Tue_m2ime='" unescape($2) "'")
	}
	if($1 == "Tue_gra2dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Tue_gra2dus='" unescape($2) "'")
	}
	if($1 == "Tue_t3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Tue_t3ime='" unescape($2) "'")
	}
	if($1 == "Tue_m3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Tue_m3ime='" unescape($2) "'")
	}
	if($1 == "Tue_gra3dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Tue_gra3dus='" unescape($2) "'")
	}
	if($1 == "Wed_t1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Wed_t1ime='" unescape($2) "'")
	}
	if($1 == "Wed_m1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Wed_m1ime='" unescape($2) "'")
	}
	if($1 == "Wed_gradus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Wed_gradus='" unescape($2) "'")
	}
	if($1 == "Wed_t2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Wed_t2ime='" unescape($2) "'")
	}
	if($1 == "Wed_m2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Wed_m2ime='" unescape($2) "'")
	}
	if($1 == "Wed_gra2dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Wed_gra2dus='" unescape($2) "'")
	}
	if($1 == "Wed_t3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Wed_t3ime='" unescape($2) "'")
	}
	if($1 == "Wed_m3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Wed_m3ime='" unescape($2) "'")
	}
	if($1 == "Wed_gra3dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Wed_gra3dus='" unescape($2) "'")
	}
	if($1 == "Thu_t1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Thu_t1ime='" unescape($2) "'")
	}
	if($1 == "Thu_m1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Thu_m1ime='" unescape($2) "'")
	}
	if($1 == "Thu_gradus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Thu_gradus='" unescape($2) "'")
	}
	if($1 == "Thu_t2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Thu_t2ime='" unescape($2) "'")
	}
	if($1 == "Thu_m2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Thu_m2ime='" unescape($2) "'")
	}
	if($1 == "Thu_gra2dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Thu_gra2dus='" unescape($2) "'")
	}
	if($1 == "Thu_t3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Thu_t3ime='" unescape($2) "'")
	}
	if($1 == "Thu_m3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Thu_m3ime='" unescape($2) "'")
	}
	if($1 == "Thu_gra3dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Thu_gra3dus='" unescape($2) "'")
	}
	if($1 == "Fri_t1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Fri_t1ime='" unescape($2) "'")
	}
	if($1 == "Fri_m1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Fri_m1ime='" unescape($2) "'")
	}
	if($1 == "Fri_gradus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Fri_gradus='" unescape($2) "'")
	}
	if($1 == "Fri_t2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Fri_t2ime='" unescape($2) "'")
	}
	if($1 == "Fri_m2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Fri_m2ime='" unescape($2) "'")
	}
	if($1 == "Fri_gra2dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Fri_gra2dus='" unescape($2) "'")
	}
	if($1 == "Fri_t3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Fri_t3ime='" unescape($2) "'")
	}
	if($1 == "Fri_m3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Fri_m3ime='" unescape($2) "'")
	}
	if($1 == "Fri_gra3dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Fri_gra3dus='" unescape($2) "'")
	}
	if($1 == "Sat_t1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sat_t1ime='" unescape($2) "'")
	}
	if($1 == "Sat_m1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sat_m1ime='" unescape($2) "'")
	}
	if($1 == "Sat_gradus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sat_gradus='" unescape($2) "'")
	}
	if($1 == "Sat_t2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sat_t2ime='" unescape($2) "'")
	}
	if($1 == "Sat_m2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sat_m2ime='" unescape($2) "'")
	}
	if($1 == "Sat_gra2dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sat_gra2dus='" unescape($2) "'")
	}
	if($1 == "Sat_t3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sat_t3ime='" unescape($2) "'")
	}
	if($1 == "Sat_m3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sat_m3ime='" unescape($2) "'")
	}
	if($1 == "Sat_gra3dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sat_gra3dus='" unescape($2) "'")
	}
	if($1 == "Sun_t1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sun_t1ime='" unescape($2) "'")
	}
	if($1 == "Sun_m1ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sun_m1ime='" unescape($2) "'")
	}
	if($1 == "Sun_gradus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sun_gradus='" unescape($2) "'")
	}
	if($1 == "Sun_t2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sun_t2ime='" unescape($2) "'")
	}
	if($1 == "Sun_m2ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sun_m2ime='" unescape($2) "'")
	}
	if($1 == "Sun_gra2dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sun_gra2dus='" unescape($2) "'")
	}
	if($1 == "Sun_t3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sun_t3ime='" unescape($2) "'")
	}
	if($1 == "Sun_m3ime")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sun_m3ime='" unescape($2) "'")
	}
	if($1 == "Sun_gra3dus")
	{
	system("uci set ds18b20.@ds18b20_s0[0].Sun_gra3dus='" unescape($2) "'")
	}
	if($1 == "pwut")
	{
	system("uci set ds18b20.@ds18b20_s0[0].pwout='" unescape($2) "'")
	}
	if($1 == "tipsensr")
	{
	system("uci set ds18b20.@ds18b20_s0[0].tipsensr='" unescape($2) "'")
	}
	if($1 == "flder")
	{
	system("uci set ds18b20.@ds18b20_s0[0].folder='" unescape($2) "'")
	}
	if($1 == "narmon0")
	{
	system("uci set ds18b20.@ds18b20_s0[0].enab_nar='" unescape($2) "'")
	system("uci commit ds18b20")
	print "<br>narodmon - Sensor 0 Save OK."
	}
	if($1 == "commit0")
	{
	system("uci commit ds18b20")
	print "<br>Sensor 0 Save OK."
	}
}
END
{
print "<br><br><br><table align=center border=0 size=18 width=><tr><td align=center bgcolor=#8891f1><b><a href=/cgi-bin/modules/termostatGpio/Sset.cgi>In settings</a></b></td></tr><tr><td></td></tr><tr><td align=center bgcolor=#90f1f1><b><a href=/cgi-bin/modules/termostatGpio/index.cgi>Exit settings</a></b></td></tr></table>"
}
