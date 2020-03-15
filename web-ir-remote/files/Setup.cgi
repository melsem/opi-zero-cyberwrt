#!/bin/sh
#echo "Content-type: text/html; charset=utf-8"
echo 
echo "<title>irc-remote-setup</title>"
echo "<table align=center><tr><td>"
echo `cat /www/menu.html`
echo "</td></tr></table>"

rckeymaps='/etc/rc_keymaps/keyes'
triggerhappy='/etc/triggerhappy/triggers.d/example.conf'

#===================================================================================
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
#===================================================================================

kollw_strok=$(sed -n '$=' $rckeymaps)
a=1
while [ "$a" -le "$kollw_strok" ]; do
  ircode=$(sed -n ''$a'p' $rckeymaps | awk '{print $1}')
  keyname=$(sed -n ''$a'p' $rckeymaps | awk '{print $2}')
  keysost=$(sed '/'$keyname'/!d' $triggerhappy | awk '{print $2}')
  command=$(sed -n 's/'$keyname'.'$keysost'.\(.*\)/\1/p' $triggerhappy)
  echo "<form method="POST" action="Save.cgi">
<tr>
 <td valign="vertical">
  <input name="nomerstroki" hidden value="$a">"$a."
 </td>
 <td valign="vertical">
  <input name="ircodestar" hidden value="$ircode">
  <input name="keynamestar" hidden value="$keyname">
  <input name="keysoststar" hidden value="$keysost">
  <input name="commandstar" hidden value='"
   printf $command
   echo "'>

  <input name="ircode" size="8" value="
   printf "$ircode"
   echo " required placeholder="список_пуст">

  <input name="keyname" value="
   printf "$keyname"
   echo " required placeholder="список_пуст">

  <input name="keysost" size="2" value="
   printf "$keysost"
   echo " required placeholder="список_пуст">

  <input name="command" size="30" value='"
   sed -n 's/'$keyname'.'$keysost'.\(.*\)/\1/p' $triggerhappy
   echo "' required placeholder="список_пуст">
 </td>
 <td valign="vertical">
<button type=submit name="edit">"edit"</button>
<button type=submit name="delete">"delete"</button>
 </td>
</tr></form>"
let "a += 1"
done
echo "</table>"

#===================================================================================
#===================================================================================

let "kollw_strok += 1"
echo "<table>
<form method="POST" action="Save.cgi"><table><tr><td align="left"><p>"
printf $kollw_strok.
echo "
<input size="2" hidden name="n_str" value="
printf $kollw_strok
echo " required placeholder="add_nev_ircode">
<input class="b2" size="10" name="add_nev_ircode" required placeholder="add_ircode">
<input class="b2" size="15" name="add_nev_keyname" required placeholder="add_keyname">
<input class="b2" size="5" name="add_nev_keysost" required placeholder="add_keysost">
<input class="b2" size="30" name="add_nev_command" required placeholder="add_command">
<button type=submit name="Save_nev">"Save_nev"</button>
</p></td></tr></table>
</form></table>"

echo "</td></tr>
</table>"
