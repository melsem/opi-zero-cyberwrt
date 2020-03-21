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
chrony_conf="/etc/chrony/chrony.conf"
sysntpd="/etc/init.d/sysntpd"
chronyd="/etc/init.d/chronyd"
gpsd="/etc/init.d/gpsd"
sost = 0
}
{
	if($1 == "staron_chronyd") { system(""sysntpd" stop") system(""sysntpd" disable")
					system(""chronyd" enable") system(""chronyd" start") }
	if($1 == "staroff_chronyd") { system(""sysntpd" enable") system(""sysntpd" start")
					system(""chronyd" stop") system(""chronyd" disable") }

	if($1 == "staron_gpsd") { system(""gpsd" enable") system(""gpsd" start") }
	if($1 == "staroff_gpsd") { system(""gpsd" stop") system(""gpsd" disable") }

	if($1 == "refclock_shm") refclock_shm = unescape($2)
	if($1 == "refclock_pps") refclock_pps = unescape($2)
	if($1 == "refclock_setsave")
	{
		system("sed -i '/refclock SHM 0/d' "chrony_conf)
		system("echo "refclock_shm" >> "chrony_conf)
		system("sed -i '/refclock PPS /d' "chrony_conf)
		system("echo "refclock_pps" >> "chrony_conf)
#		print "<b> Save refclock <br> OK</b><br>"
	}

	if($1 == "gpsd_enabled") gpsd_enabled = unescape($2)
	if($1 == "device") device = unescape($2)
	if($1 == "devPort") devPort = unescape($2)
	if($1 == "listen_globally") listen_globally = unescape($2)
	if($1 == "setsave")
	{
		system("uci set gpsd.@gpsd[0].enabled='"gpsd_enabled"'")
		system("uci set gpsd.@gpsd[0].device='"device"'")
		system("uci set gpsd.@gpsd[0].port='"devPort"'")
		system("uci set gpsd.@gpsd[0].listen_globally='"listen_globally"'")
#		print "<b> Save web-pps-gps-init OK</b><br>"
	}
}
END
{
	system("/www/cgi-bin/modules/web-pps-gps/index.html")
}
