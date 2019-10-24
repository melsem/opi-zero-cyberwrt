#!/bin/sh
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
echo "Content-type: text/html; charset=utf-8"
echo
echo "<title>Редактировать папку</title>"

if echo "$QUERY_STRING" | egrep -q "carduinoruqwer" ; then
QUERY_STRING=${QUERY_STRING//carduinoruqwer}
QUERY_STRING=${QUERY_STRING//+/_}
QUERY_STRING=${QUERY_STRING//%2F//}
QUERY_STRING=${QUERY_STRING//=/}
QUERY_STRING=$(echo $QUERY_STRING | tr -s '/')
oldnam=$(echo $QUERY_STRING | tr '&' '\n' | tail -n1)
newnam=${QUERY_STRING/&$oldnam}
pat=$(echo $newnam | tr '/' '\n' | tail -n1)
pat=${newnam/$pat}
oldnam=$pat$oldnam
`mv $oldnam $newnam`
QUERY_STRING=$newnam/
echo "<script>window.opener.location.href='index.cgi?$pat'; window.close(); return false
</script>"
fi
if echo "$QUERY_STRING" | egrep -q "chmodcarduino" ; then
QUERY_STRING=${QUERY_STRING//%2F//}
pat=$(echo $QUERY_STRING | tr '=' '\n' | tail -n1)
chmodnew=${QUERY_STRING:8:3}
`chmod $chmodnew $pat`
QUERY_STRING=$pat
dpat=$(echo $QUERY_STRING | tr '/' '\n' | tail -n2)
pat=${pat/$dpat}
echo "<script>window.opener.location.href='index.cgi?$pat'; window.close(); return false</script>"
fi


#val="<pre>`cat $QUERY_STRING`"
temp=$(echo $QUERY_STRING | tr '/' '\n' | tail -n2)
path=${QUERY_STRING/$temp}
QUERY_STRING=$(echo $QUERY_STRING | tr -s '/')
cmd=`ls -ld $QUERY_STRING`
cmd=$(echo $cmd | awk '{k=0;for(i=0;i<=8;i++)k+=((substr($1,i+2,1)~/[rwx]/) \
             *2^(8-i));if(k)printf("%0o ",k);}')

#echo $QUERY_STRING

echo "<script type=\"text/javascript\" src=\"/modules/filemanager/chmod.js\"></script>
<table width=100% border=0><tr><td valign=top><b>Переименовать папку</b><br>
<form actiom=edit.cgi method=GET><input type=text name=carduinoruqwer$path value=$temp>
<input type=hidden name=carduinoruqwer value=$temp><br>"
echo "<input type=submit value=Переименовать></form>
</td></tr>
<tr><td>
<b>Изменить права у папки</b>
<form name=\"chmod\" action=\"editd.cgi\" method=\"GET\">
<TABLE BORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"0\" style=\"font:normal 12px Verdana\";>
<TR ALIGN=\"LEFT\" VALIGN=\"MIDDLE\">
<TD>Текущие права: </TD>
<TD>
<input type=\"text\" name=\"t_total\" value=$cmd size=\"4\" onKeyUp=\"octalchange()\"> </TD>
<TD><input type=\"text\" name=\"sym_total\" value=\"\" size=\"12\" READONLY=\"1\" STYLE='border: 0px none; font-family: \"Courier New\", Courier, mono;'>
</TD>
</TR>
</TABLE>
<BR>
<table cellpadding=\"2\" cellspacing=\"0\" border=\"0\" style=\"font:normal 12px Verdana\">
<tr bgcolor=\"#333333\">
<td WIDTH=\"60\" align=\"left\"> </td>
<td WIDTH=\"55\" align=\"center\" style=\"color:white\"><b>owner
</b></td>
<td WIDTH=\"55\" align=\"center\" style=\"color:white\"><b>group
</b></td>
<td WIDTH=\"55\" align=\"center\" style=\"color:white\"><b>other
<b></td>
</tr>
<tr bgcolor=\"#dddddd\">
<td WIDTH=\"60\" align=\"left\" nowrap BGCOLOR=\"#FFFFFF\">read</td>
<td WIDTH=\"55\" align=\"center\" bgcolor=\"#EEEEEE\">
<input type=\"checkbox\" name=\"owner4\" value=\"4\" onclick=\"calc_chmod()\">
</td>
<td WIDTH=\"55\" align=\"center\" bgcolor=\"#ffffff\"><input type=\"checkbox\" name=\"group4\" value=\"4\" onclick=\"calc_chmod()\">
</td>
<td WIDTH=\"55\" align=\"center\" bgcolor=\"#EEEEEE\">
<input type=\"checkbox\" name=\"other4\" value=\"4\" onclick=\"calc_chmod()\">
</td>
</tr>
<tr bgcolor=\"#dddddd\">
<td WIDTH=\"60\" align=\"left\" nowrap BGCOLOR=\"#FFFFFF\">write</td>
<td WIDTH=\"55\" align=\"center\" bgcolor=\"#EEEEEE\">
<input type=\"checkbox\" name=\"owner2\" value=\"2\" onclick=\"calc_chmod()\"></td>
<td WIDTH=\"55\" align=\"center\" bgcolor=\"#ffffff\"><input type=\"checkbox\" name=\"group2\" value=\"2\" onclick=\"calc_chmod()\">
</td>
<td WIDTH=\"55\" align=\"center\" bgcolor=\"#EEEEEE\">
<input type=\"checkbox\" name=\"other2\" value=\"2\" onclick=\"calc_chmod()\">
</td>
</tr>
<tr bgcolor=\"#dddddd\">
<td WIDTH=\"60\" align=\"left\" nowrap BGCOLOR=\"#FFFFFF\">execute</td>
<td WIDTH=\"55\" align=\"center\" bgcolor=\"#EEEEEE\">
<input type=\"checkbox\" name=\"owner1\" value=\"1\" onclick=\"calc_chmod()\">
</td>
<td WIDTH=\"55\" align=\"center\" bgcolor=\"#ffffff\"><input type=\"checkbox\" name=\"group1\" value=\"1\" onclick=\"calc_chmod()\">
</td>
<td WIDTH=\"55\" align=\"center\" bgcolor=\"#EEEEEE\">
<input type=\"checkbox\" name=\"other1\" value=\"1\" onclick=\"calc_chmod()\">
</td>
</tr>
</table>
<input type=\"hidden\" name=chmodcarduino value=$QUERY_STRING>
<input type=submit value=\"Изменить права\">
</form>
</td></tr>
</table>"
#echo $val
#echo $QUERY_STRING