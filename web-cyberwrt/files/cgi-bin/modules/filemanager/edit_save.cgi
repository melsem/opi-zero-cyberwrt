#!/usr/bin/awk -f
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
function unescape(s)
{
gsub(/\+/," ",s)
gsub("%0D","",s)

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
if($1 == "B3" ){
path=unescape($2)
#path=$(path//%2F//)
}
if($1 == "S1" ){
print unescape($2) > path
}
}
END
{
print "Сохранено.<br>"
print "<a href=\"javascript:window.open(window.location, \'_self\').close();\">Закрыть окно</a>"
#print "202"
}
