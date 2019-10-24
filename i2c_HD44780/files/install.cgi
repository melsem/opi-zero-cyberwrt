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


END
{
	system("cp -f /www/cgi-bin/modules/i2c_hd44780/pp.cgi /tmp/pp.sh")
	system("chmod 755 /tmp/pp.sh")
	system("sh /tmp/pp.sh &")
	print "<br><b>Please wait 10 sek.</b><br>"
	system("/www/cgi-bin/modules/manmod/index.cgi")
}
