#!/bin/sh
#echo "Content-type: text/html; charset=utf-8"
echo 
echo "<title>irc-remote-setup</title>"
echo "<table align=center><tr><td>"
echo `cat /www/menu.html`
echo "</td></tr></table>"

rckeymaps='/etc/rc_keymaps/keyes'
triggerhappy='/etc/triggerhappy/triggers.d/example.conf'
cat /etc/rc.d/S99web-ir-remote &> /dev/null
if [ $? = 1 ]; then
ontxtoff2='Желаете включить авто-запуск?'
ontxtoff='Желаете отключить авто-запуск?'
onoff=staron
cto=OFF
clor="ff0000"
dsd=""
dsd2=hidden
else
dsd=hidden
dsd2=""
clor="0000ff"
ontxtoff2='Желаете отключить авто-запуск?'
ontxtoff='Желаете включить авто-запуск?'
onoff=staroff
cto=ON
fi

#_______________ time-javascript _____________________________

if [ "$1" == "setup_irc" ]; then
echo "<table align="center" border=0 width=><tr><td>
<b><span hidden id="time"></span></b>
<script type="text/javascript">var i = 0; function time(){ document.getElementById(\"time\").innerHTML = i; i--; if (i < 0) location.href = \"/cgi-bin/modules/web-ir-remote/Setup.html\"; }
time(); setInterval(time, 500); </script>
</td></tr></table>"
fi
#____________________________________________________________

#===================================================================================
echo "<body bgcolor="#f0f0f0">
<style>
  .b0 {
cursor: pointer;
}
  .b1 {
font-size: 15pt;
text-align: center;
padding: 20px 40px;
margin-left: 1px;
cursor: pointer;
border-radius:inherit;
}
</style>"

echo "
<table align=center><br>
<caption><b>Setup rc-keymaps and triggerhappy</b></caption>
<tr><td>
<table align=center>"
#===================================================================================
# kollw_strok=$(sed -n '$=' $rckeymaps)
kollw_strok=$(sed -n '$=' $triggerhappy)	# example.conf
a=1
while [ "$a" -le "$kollw_strok" ]; do
  keyname=$(sed -n ''$a'p' $triggerhappy | awk '{print $1}')
  keysost=$(sed -n ''$a'p' $triggerhappy | sed '/'$keyname'/!d' | awk '{print $2}')
  command=$(sed -n ''$a'p' $triggerhappy | sed -n 's/'$keyname'.'$keysost'.\(.*\)/\1/p')
  ircode=$(sed '/'$keyname'$/!d' $rckeymaps | awk '{print $1}')
  echo "<form method="POST" action="Save.cgi">
<tr>
 <td align="vertical">
  <input name="nomerstroki" hidden value="$a">"$a."
 </td>
 <td align="vertical">
  <input name="ircode_star" hidden value="$ircode">
  <input name="keyname_star" hidden value="$keyname">
  <input name="keysost_star" hidden value="$keysost">
  <input name="command_star" hidden value='"
   printf $command
   echo "'>

  <input name="ircode" size="8" value="
  if [ -n "$ircode" ]; then
   printf "$ircode"
  else
   printf "keyboard"
  fi
   echo " required placeholder="список_пуст">

  <input readonly name="keyname" value="
   printf "$keyname"
   echo " required placeholder="список_пуст">

  <input name="keysost" size="2" value="
   printf "$keysost"
   echo " required placeholder="список_пуст">

  <input name="command" size="30" value='"
   sed -n 's/\b'$keyname'\b.'$keysost'\b.\(.*\)/\1/p' $triggerhappy
   echo "' required placeholder="список_пуст">
 </td>
 <td align="vertical">
<button class="b0" type=submit name="edit">"edit"</button>
<button class="b0" type=submit name="delete">"delete"</button>
 </td>
</tr></form>"
let "a += 1"
done
echo "</table>"
#======  Save_nev  =======
echo "<table>
<form method="POST" action="Save.cgi"><table><tr><td align="left"><p>"
let "kollw_strok += 1"
printf $kollw_strok.
echo "<input size="10" name="add_nev_ircode" required value="keyboard">"
 kollw_comand=$(thd --listevents | sed -n '$=') # 581 - command
 ac=1
echo "<select class="button" name="add_nev_keyname">"
 while [ "$ac" -le "$kollw_comand" ]; do
  echo "<option>"
  thd --listevents | sed -n ''$ac'p'
  echo "</option>"
  let "ac += 1"
 done
echo "</select>"
echo "
  <input size="5" name="add_nev_keysost" required placeholder="add_keysost">
  <input size="30" name="add_nev_command" required placeholder="add_command">
<button class="b0" type=submit name="Save_nev">"Save_nev"</button>
</p></td></tr></table>
</form></table>"
#====== End Save_nev  =======

echo "</td></tr>
</table>"
#####################################################

echo "<form method="POST" action="Save.cgi">
<table align=center border=0>
<tr><td>
<tr><td align=center><p style="color:#$clor">Auto Startup - <b>$cto</b></p></td></tr>

<tr><td align=center>
<button $dsd name="$onoff" type=submit title='"$ontxtoff2"' class="b1">
<img src=/modules/web-ir-remote/b.jpg alt= style="vertical-align: middle">"Enable"</button>

<button $dsd2 name="$onoff" type=submit title='"$ontxtoff2"' class="b1">
<img src=/modules/web-ir-remote/r.jpg alt= style="vertical-align: middle">"Disable"</button>
</td></tr>

</td></tr></table></form>"
###*************************************************
init='/etc/init.d/web-ir-remote'
n_strok=$(sed -n '$=' $init)
d_strok=$(wc -L $init)
  echo "<table align=center>
<form method="POST" action="Save.cgi">
<tr>
 <td class="b1" align="vertical">
 <textarea name="webirremoteinit" rows="$n_strok" cols="$d_strok" wrap="off">"
  cat $init
  echo "</textarea>
 </td></tr>
 <tr>
 <td align=center>
<button class="b0" type=submit name="textremoteinit">"save"</button>
 </td>
</tr></form>
</table>"
