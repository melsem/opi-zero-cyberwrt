#!/bin/sh
#echo "Content-type: text/html; charset=utf-8"
echo 
echo "<title>irc-remote-setup</title>"
echo "<table align=center><tr><td>"
echo `cat /www/menu.html`
echo "</td></tr></table>"

rckeymaps='/etc/rc_keymaps/keyes'
triggerhappy='/etc/triggerhappy/triggers.d/example.conf'
kollw_strok=$(sed -n '$=' $triggerhappy)
#===================================================================================
#===================================================================================
echo "<body bgcolor="#f0f0f0">
<style>
  .b0 {
cursor: pointer;
}
  .b1 {
height: 10px;
text-align: left;
padding: 1px;
border: 2px solid #0000cc;
border-radius: 10px;
}
</style>"

echo "
<table align=center><br>
<caption><b>Setup triggerhappy</b></caption>
<tr><td>
<table align=center>"
#===================================================================================
#===================================================================================
  echo "<form method="POST" action="Save.cgi">
<tr>
 <td class="b1" align="vertical">
 <textarea name="exampleconf" rows="$kollw_strok" cols="60" wrap="off">"
  cat $triggerhappy
  echo "</textarea>
 </td></tr>
 <tr>
 <td align=center>
<button class="b0" type=submit name="text">"text save"</button>
 </td>
</tr></form>
</table>"

#===================================================================================
#===================================================================================

echo "</td></tr>
</table>"
