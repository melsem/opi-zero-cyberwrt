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
overl_ok=0
}
{
	if ($1 == "overlays") {
		overlays = "overlays"
		name=unescape($2)
		overl_ok=1
	}
	else {
		driv = $1
		stats = unescape($2)
		system("boot-config "driv" "stats" &> /dev/null")
	}
}
END
{
	if (overl_ok == "1") {
		print "<b>"overlays" -- "name"</b><br>"
		system("/www/cgi-bin/modules/boot-config/index.html "overlays" "name"")
	}

	else system("/www/cgi-bin/modules/boot-config/index.html save_okay")
}

