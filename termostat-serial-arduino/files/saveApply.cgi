#!/usr/bin/awk -f
function dannie(s)
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
cbr=0
menu1=0
menu1nosynk=0
tbr=0 # edit conf-file port_listen
abr=0 # edit conf-file termostat-serial-arduino
menu2=0
dsm=0 # edit conf-file termostat-serial-arduino
}
{
	if($1 == "prbor") {
	tpbr=dannie($2)
	system("uci set termostat-serial-arduino.@displ_tip[0].prbor='"dannie($2)"'")
	kdl=length(tpbr)	# подcчет символов в имени
	if(kdl > 54) kdl = 54	# максимально 54
	}
	if(($1 == "tprbor") && (tpbr != dannie($2))) cbr+=1
	if($1 == "fPort") cfport=dannie($2)
	if(($1 == "save_prbor") && (cbr != 0)) {
	system("uci commit termostat-serial-arduino")
	system("echo d+=0 "kdl" "tpbr" > /dev/tty"cfport"")
#print "<b>echo d+=0 "kdl" "tpbr" > /dev/tty"cfport"</b><br>"
	}
#menu1
	if($1 == "speed") {
	nevSpeed=dannie($2)
	system("uci set port_listen.@displ_tip[0].speed='"nevSpeed"'")
	}
	if(($1 == "tspeed") && (nevSpeed != dannie($2))) tbr+=1
	if($1 == "devPort") {
	nevDevPort=dannie($2)
	system("uci set port_listen.@displ_tip[0].devPort='"nevDevPort"'")
	system("uci set termostat-serial-arduino.@displ_tip[0].updateRead='""'")
	}
	if($1 == "tdevPort") {
	stariyDevPort=dannie($2)
	if(nevDevPort != stariyDevPort) tbr+=1
	}
	if($1 == "t_file") {
	arfile=dannie($2)
	system("uci set port_listen.@displ_tip[0].t_file='"dannie($2)"'")
	}
	if(($1 == "tt_file") && (arfile != dannie($2))) tbr+=1
	if($1 == "i2c_adres") {
	system("uci set termostat-serial-arduino.@displ_tip[0].i2c_adres='"dannie($2)"'")
	ic=dannie($2)
	}
	if(($1 == "ti2c_adres") && (ic != dannie($2))) abr+=1
	if($1 == "simvol") {
	system("uci set termostat-serial-arduino.@displ_tip[0].simvol='"dannie($2)"'")
	sm=dannie($2)
	}
	if(($1 == "tsimvol") && (sm != dannie($2))) abr+=1
	if($1 == "strok") {
	system("uci set termostat-serial-arduino.@displ_tip[0].strok='"dannie($2)"'")
	sr=dannie($2)
	}
	if(($1 == "tstrok") && (sr != dannie($2))) abr+=1
	if($1 == "w1_gpio") {
	system("uci set termostat-serial-arduino.@arduino[0].w1_gpio='"dannie($2)"'")
	wr=dannie($2)
	}
	if(($1 == "tw1_gpio") && (wr != dannie($2))) abr+=1
	if($1 == "save_commit") menu1+=1
	else if($1 == "save_commit_synk") {
			menu1+=1
			menu1nosynk+=1
			}
#menu2
	if($1 == "poryadk") {
	mc=dannie($2)-1
	aa=dannie($2)
	}
	if($1 == "dsPort") comport=dannie($2)
	if($1 == "name_snr"aa) {
	name=dannie($2)
	dl=length(name)	# подcчет символов в имени
	if(dl > 13) dl = 13	# максимально 13
	system("uci set termostat-serial-arduino.@arduino[0].name_snr'"aa"'='"name"'")
	}
	if(($1 == "tname_snr"aa) && (name != dannie($2))) dsm+=1
	if($1 == "MAC"aa) {
	mac=dannie($2)
	system("uci set termostat-serial-arduino.@arduino[0].MAC'"aa"'='"mac"'")
	}
	if(($1 == "tMAC"aa) && (mac != dannie($2))) dsm+=1
	if($1 == "tip_ds"aa) {
	tip=dannie($2)
	system("uci set termostat-serial-arduino.@arduino[0].tip_ds'"aa"'='"tip"'")
	}
	if(($1 == "ttip_ds"aa) && (tip != dannie($2))) dsm+=1
	if($1 == "nGp"aa"_out") {
	nGp=dannie($2)
	system("uci set termostat-serial-arduino.@arduino[0].nGp'"aa"'_out='"nGp"'")
	}
	if(($1 == "tnGp"aa"_out") && (nGp != dannie($2))) dsm+=1
	if($1 == "invers"aa)
	{
	  	invers=dannie($2)
		system("uci set termostat-serial-arduino.@arduino[0].invers'"aa"'='"invers"'")
	}
	if(($1 == "tinvers"aa) && (invers != dannie($2))) dsm+=1
	if($1 == "alarm"aa) {
	 	allar=dannie($2)*10
		system("uci set termostat-serial-arduino.@arduino[0].alarm'"aa"'='"allar"'")
	}
	if(($1 == "talarm"aa) && (allar != dannie($2))) dsm+=1
	if($1 == "histe"aa"") {
	hist=dannie($2)*10
	system("uci set termostat-serial-arduino.@arduino[0].histe'"aa"'='"hist"'")
	}
	if(($1 == "thiste"aa) && (hist != dannie($2))) dsm+=1
	if($1 == "save_commit_sensor") {
	if(dsm != 0) {
	if(mc < 4) system("echo b+="mc" "dl" "name" "mac" "tip" "nGp" "invers" "allar" "hist"/ > /dev/tty"comport"")
	else system("echo b+="mc" "dl" "name" "mac"/ > /dev/tty"comport"")
	menu2=1
	sleep 2
	system("uci commit termostat-serial-arduino")
	}
	}
}
END
{
	if (menu1 != 0) {
	  if((abr != 0) || (tbr != 0)) system("echo a+=0 "nevSpeed" "sm" "sr" "wr" "ic" > /dev/tty"stariyDevPort"")
	  if(abr != 0) system("uci commit termostat-serial-arduino")
	  if(tbr != 0) {
	    system("uci commit port_listen")
	    sleep 1
	    system("/etc/init.d/port_listen.init restart")
	    sleep 2
	  }
#	  print "<b>echo a+=0 "nevSpeed" "sm" "sr" "wr" "ic" > /dev/tty"stariyDevPort"</b><br>"
	  if (menu1nosynk != 0) system("/www/cgi-bin/modules/termostat-serial-arduino/sync.cgi 1")
	  else system("/www/cgi-bin/modules/termostat-serial-arduino/setupMenu.cgi")
	}
	else if (menu2 != 0) system("/www/cgi-bin/modules/termostat-serial-arduino/sync.cgi 0")
	else system("/www/cgi-bin/modules/termostat-serial-arduino/setupMenu.cgi")
}


