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
rebut=0
}
{

	if($1 == "backup") {
		value=unescape($2)
		if(value == "backdtb") {
			print "<b>backup - OK</b><br>"
			system("dtc -I dtb -O dts -o /boot/dts.backup /boot/dtb")
			system("dtc -I dts -O dtb -o /boot/dtb.backup /boot/dts.backup")
		}
	}

	if($1 == "resback") {
		value=unescape($2)
		if(value == "resdtb") {
			print "<b>Restore u-boot from backup - OK</b><br>"
			system("rm /tmp/t-dts")
			system("cp /boot/dtb.backup /boot/dtb")
		}
	}

#	************  max_freq  ***********
	if($1 == "max_freq") {
		set_max_freq=unescape($2)
		if(unescape($2) == "480MHz") max_freq=480000
		else if(unescape($2) == "648MHz") max_freq=648000
		else if(unescape($2) == "816MHz") max_freq=816000
		else if(unescape($2) == "1008MHz") max_freq=1008000
	}
	if($1 == "astart") if(unescape($2) == "noastar") system("/etc/init.d/cpufreq_scaling enable")
	if($1 == "set_cpu_freq") {
		if(max_freq != unescape($2)) {
			system("uci set cpufreq_scaling.@cpufreq[0].max_freq='"max_freq"'")
			system("uci commit cpufreq_scaling")
			print "<b>Scaling max_freq: "set_max_freq" changed.</b><br>"
			system("/etc/init.d/cpufreq_scaling start")
		}
		else print "<b>max_freq unchanged.</b><br>"
	}
#	************  end-max_freq  **********

	if($1 == "conv") {
		value=unescape($2)
		if(value == "dtsmmc") {
				system("dtc -I dtb -O dts -o /tmp/t-dts /boot/dtb")
print "<b>CONVERT-MMC-DTS</b><br>"
		}
		else if(value == "dtsspi") {
				system("dtc -I dtb -O dts -o /tmp/t-dts /dev/mtd1")
print "<b>CONVERT-SPI-DTS</b><br>"
		}
	}

	if($1 == "reconv") {
		rebut=1
		value=unescape($2)
		if(value == "dtbmmc") {
				system("dtc -I dts -O dtb -o /boot/dtb /tmp/t-dts")
print "<b>RE-CONVERT-MMC-DTB</b><br>"
		}
		else if(value == "dtbspi") {
				system("dtc -I dts -O dtb -o /tmp/dtb /tmp/t-dts")
				system("mtd -e dtb write /tmp/dtb dtb")
print "<b>RE-CONVERT-SPI-DTB</b><br>"
		}
	}

}
END
{
		if(rebut == "1") system("sleep 1s")
		system("/www/cgi-bin/modules/boot-config/index.html "rebut"")
}

