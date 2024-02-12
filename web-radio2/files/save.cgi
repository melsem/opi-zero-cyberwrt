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
delchan=""
add_nev_url=""
add_nev_name=""
}
{
	playlist="/etc/config/web-radio2"

	if($1 == "delchan") {
		delchan=$1
		delstroki=dannie($2)
#		print "delstroki - "delstroki
		system("/www/cgi-bin/modules/web-radio2/sed.cgi delet "delstroki"")
	}
	
#	========= play-NAME-URL-(play-list) ========
	if($1 == "nomerstroki") nomerstroki=dannie($2)
	if($1 == "nameURL") nameUrlStroki=dannie($2)
	if($1 == "URL2")
	{
		system("amixer -c 0 -q set 'DAC' 100%-")
		system("sleep 1")
		system("killall -9 web-radio2 2> /dev/null")
		system("web-radio2 -u "dannie($2)" -n "nameUrlStroki" -p "nomerstroki" &")
		system("amixer -c 0 -q set 'DAC' 100%")
	}
#	===================================
	if($1 == "next") {
		system("amixer -c 0 -q set 'DAC' 100%-")
		system("sleep 1")
		system("killall -9 madplay 2> /dev/null")
		system("killall -9 curl 2> /dev/null")
		system("amixer -c 0 -q set 'DAC' 100%")
	}
	if($1 == "zapusk") system("/etc/init.d/web-radio2.init start")
	if($1 == "ostanov") system("/etc/init.d/web-radio2.init stop")
	if($1 == "autostarton") system("/etc/init.d/web-radio2.init enable")
	if($1 == "autostartoff") system("/etc/init.d/web-radio2.init disable")

#	========= play-new-URL ============
	if($1 == "URL")
	{
		system("amixer -c 0 -q set 'DAC' 100%-")
		system("sleep 1")
		system("killall -9 web-radio2 2> /dev/null")
		system("web-radio2 -u "dannie($2)" &")
		system("amixer -c 0 -q set 'DAC' 100%")
	}

#	========= play-file ============
	if($1 == "file")
	{
		system("amixer -c 0 -q set 'DAC' 100%-")
		system("sleep 1")
		system("killall -9 web-radio2 2> /dev/null")
		system("web-radio2 -f "dannie($2)" &")
		system("amixer -c 0 -q set 'DAC' 100%")
	}

#	========= play-folder ==========
	if($1 == "folder")
	{
		system("amixer -c 0 -q set 'DAC' 100%-")
		system("sleep 1")
		system("killall -9 web-radio2 2> /dev/null")
		system("web-radio2 -l "dannie($2)" &")
		system("amixer -c 0 -q set 'DAC' 100%")
	}

#	========= edit play-list =========
 	if($1 == "text") print dannie($2) | "sed 's/\r//;/^$/d' > "playlist

#	========= add_nev_name_url ==========
	if($1 == "n_str") n=dannie($2)
	if($1 == "add_nev_name")
	{
		add_nev_name=S1
		nb=dannie($2)
#		print "#EXTINF:-"n","dannie($2) | "sed 's/\r//' | sed '/^$/d' >> "playlist
		system("/www/cgi-bin/modules/web-radio2/sed.cgi addnev "n" "nb"")
	}
	if($1 == "add_nev_url") {
		add_nev_url="add_nev_url"
		print dannie($2) | "sed 's/\r//' | sed '/^$/d' >> "playlist
	}
}
END
{

	if($1 == "text")
	{
		print "<b>Play list saved. OK</b><br>"
		system("/www/cgi-bin/modules/web-radio2/setup.cgi text")
	}
	else if(add_nev_url == "add_nev_url")
	{
		print "<b>NEW URL saved. OK</b><br>"
		system("/www/cgi-bin/modules/web-radio2/setup.cgi add_nev_url")
	}
	else if(delchan == "delchan")

	{
		print "<b>Delete. OK</b><br>"
		system("/www/cgi-bin/modules/web-radio2/setup.cgi delchan")
	}
	else if($1 == "next") { system("/www/cgi-bin/modules/web-radio2/setup.cgi next") }
	else system("/www/cgi-bin/modules/web-radio2/setup.cgi text")
}

