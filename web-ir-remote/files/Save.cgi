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
rckeymaps="/etc/rc_keymaps/keyes"
triggerhappy="/etc/triggerhappy/triggers.d/example.conf"
sost = 0
}
{

	if($1 == "nomerstroki") nomerstroki = unescape($2)
	if($1 == "ircodestar") ircodestar = unescape($2)
	if($1 == "keynamestar") keynamestar = unescape($2)
	if($1 == "keysoststar") keysoststar = unescape($2)
	if($1 == "commandstar") commandstar = unescape($2)

	if($1 == "ircode") ircode = unescape($2)
	if($1 == "keyname") keyname = unescape($2)
	if($1 == "keysost") keysost = unescape($2)
	if($1 == "command") command = unescape($2)
	if($1 == "edit")
	{
		system("ir-remote rckeymaps '"ircodestar"' '"nomerstroki"' '"ircode"' '"keyname"'")
		system("ir-remote triggerhappy '"keynamestar"' '"keysoststar"' '"keyname"' '"keysost"' '"command"'")
		system("/etc/init.d/web-ir-remote restart")
	}
	if($1 == "delete")
	{
		system("ir-remote rckeymapsdelete '"ircodestar"' '"nomerstroki"'")
		system("ir-remote triggerhappydelete '"keynamestar"' '"keysoststar"' '"commandstar"'")
		#print "DELETE-2<br>"
		system("/etc/init.d/web-ir-remote restart")
	}

	if($1 == "add_nev_ircode") add_nev_ircode = unescape($2)
	if($1 == "add_nev_keyname") add_nev_keyname = unescape($2)
	if($1 == "add_nev_keysost") add_nev_keysost = unescape($2)
	if($1 == "add_nev_command") add_nev_command = unescape($2)
	if($1 == "Save_nev")
	{
		system("echo "add_nev_ircode" "add_nev_keyname" >> "rckeymaps)
		system("echo -e "add_nev_keyname"'\t'"add_nev_keysost"'\t'"add_nev_command" >> "triggerhappy)
		system("/etc/init.d/web-ir-remote restart")
	}
	if($1 == "staron") system("/etc/init.d/web-ir-remote enable")
	if($1 == "staroff") system("/etc/init.d/web-ir-remote disable")

	if($1 == "exampleconf") exampleconf = unescape($2)
	if($1 == "text")
	{
		sost = 1
		system("echo '"exampleconf"' | sed 's/\r//;/^$/d' > /etc/triggerhappy/triggers.d/example.conf")
		#print "<b> Save example.conf OK</b><br>"
		system("/etc/init.d/web-ir-remote restart")
	}

	if($1 == "webirremoteinit") webirremoteinit = unescape($2)
	if($1 == "textremoteinit")
	{
		system("echo '"webirremoteinit"' | sed 's/\r//;/^$/d' > /etc/init.d/web-ir-remote")
	#	print "<b> Save web-ir-remote-init OK</b><br>"
	}
}
END
{
	if(sost == 0) system("/www/cgi-bin/modules/web-ir-remote/Setup.cgi")
	else if(sost == 1) system("/www/cgi-bin/modules/web-ir-remote/Setuptryg.cgi")
}
