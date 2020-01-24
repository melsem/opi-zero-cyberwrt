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
	print "<b>backup - OK</b><br>"
	system("dtc -I dtb -O dts -o /boot/dts.backup /boot/dtb")
	system("dtc -I dts -O dtb -o /boot/dtb.backup /boot/dts.backup")
	}

	if($1 == "conv") {
		value=unescape($2)
	if(value == "dts") system("dtc -I dtb -O dts -o /tmp/t-dts /boot/dtb")
	}

	if($1 == "reconv") {
		rebut=1
		value=unescape($2)
	if(value == "dtb") system("dtc -I dts -O dtb -o /boot/dtb /tmp/t-dts")
	}

}
END
{
		system("sleep 1s")
		system("/www/cgi-bin/modules/boot-config/index.html "rebut"")
}



