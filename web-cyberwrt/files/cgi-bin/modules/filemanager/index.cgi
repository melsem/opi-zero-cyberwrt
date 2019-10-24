#!/bin/sh
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
echo "Content-type: text/html; charset=utf-8"
echo
echo "<title>Файловый менеджер</title>
<style>
   table.main tr:nth-child(2n) { background: #f0f0f0;}
   table.main tr:nth-child(3) { background: #667; color: #fff; }
   table.main tr:hover { background: silver; }
</style>"
if echo "$QUERY_STRING" | egrep -q "qwerrewq" ; then
QUERY_STRING=${QUERY_STRING/qwerrewq=}
QUERY_STRING=${QUERY_STRING//%2F//}
QUERY_STRING=$QUERY_STRING/
fi
if echo "$QUERY_STRING" | egrep -q "&&&" ; then
QUERY_STRING=${QUERY_STRING/&&&}
temp=$(echo $QUERY_STRING | tr '/' '\n' | tail -n2)
QUERY_STRING=${QUERY_STRING/$temp}
QUERY_STRING=$(echo $QUERY_STRING | tr -s '/')
fi

if echo "$QUERY_STRING" | egrep -q "&&_&&" ; then
QUERY_STRING=/$QUERY_STRING
`rm ${QUERY_STRING/&&_&&}`
QUERY_STRING=${QUERY_STRING/&&_&&}
temp=$(echo $QUERY_STRING | tr '/' '\n' | tail -n1)
QUERY_STRING=${QUERY_STRING/$temp}
QUERY_STRING=$(echo $QUERY_STRING | tr -s '/')
fi
if echo "$QUERY_STRING" | egrep -q "&&carduino&&" ; then
QUERY_STRING=/$QUERY_STRING
`rmdir ${QUERY_STRING/&&carduino&&}`
QUERY_STRING=${QUERY_STRING/&&carduino&&}
temp=$(echo $QUERY_STRING | tr '/' '\n' | tail -n2)
QUERY_STRING=${QUERY_STRING/$temp}
QUERY_STRING=$(echo $QUERY_STRING | tr -s '/')
QUERY_STRING=$(echo $QUERY_STRING | tr -s '/')
fi
if echo "$QUERY_STRING" | egrep -q "&newdircarduino&" ; then
QUERY_STRING=/$QUERY_STRING
QUERY_STRING=${QUERY_STRING/&newdircarduino&}
QUERY_STRING=$(echo $QUERY_STRING | tr -s '/')
`mkdir $QUERY_STRING/NEW_FOLDER`
fi
if echo "$QUERY_STRING" | egrep -q "&newfilcarduino&" ; then
QUERY_STRING=/$QUERY_STRING
QUERY_STRING=${QUERY_STRING/&newfilcarduino&}
QUERY_STRING=$(echo $QUERY_STRING | tr -s '/')
`touch $QUERY_STRING/NEW_FILE`
fi
echo `cat /www/menu.html`
echo "<table border=0 class=main><tr class=menu><td colspan=5><div style=\"height: 35px;\"><table width=100% style='background:ffffff'><tr style='background:ffffff'><td><a href=?$QUERY_STRING&newdircarduino& title=\"Создать папку\" onclick=\"return confirm('Создать папку \'NEW_FOLDER\'?')\"><img src=/modules/filemanager/folder.png width="32" height="32"></a>
<a href=?$QUERY_STRING&newfilcarduino& title=\"Создать файл\" onclick=\"return confirm('Создать файл \'NEW_FILE\'?')\"><img src=/modules/filemanager/file.png width="32" height="32"></a></td>
<td align=right><form method=POST enctype=multipart/form-data action=upload_file.cgi>
<input type=file name=&&&$QUERY_STRING><input type=submit value=\"Загрузить файл\">
</form></td></tr></table></div></td></tr>
<tr><td colspan=5><div style=\"height: 25px;\"><form action=index.cgi method=GET>
<input type=text name=qwerrewq style=\"width: 100%; background-color: #f1f1f1; vertical-align: top;\" value="
echo $QUERY_STRING
echo "></form></div></td></tr><tr><td width=150><b>Имя</td><td><b>Ред</td><td><b>Удал</td><td width=70><b>Размер</b></td><td width=220><b>Дата изменения</b></td></tr>"
if echo "$QUERY_STRING" | egrep -q '[a-z || 0-0]/' ; then
echo "<tr><td colspan=5><a href=?$QUERY_STRING&&&><img src=/modules/filemanager/folder.png>..</a></td></tr>"
fi
for param in `ls -F /$QUERY_STRING` ; do
if echo "$param" | egrep -q "/" ; then
echo "<tr><td><a href=?$QUERY_STRING$param"
echo "><img src=/modules/filemanager/folder.png>"
echo $(echo $param | tr -d '/')
echo "</a></td><td><a href=# onclick=window.open('editd.cgi?/$QUERY_STRING$param','Edit','width=250,height=340,toolbar=0,location=0')  title=Редактировать><img src=/modules/filemanager/edit.png></a></td><td align=center><a href=?$QUERY_STRING$param&&carduino&&"
echo "onclick=\"return confirm('Удалить папку \'$param\'?')\" title=Удалить><img src=/modules/filemanager/del.png></a></td>"
echo "<td>`du -hs /$QUERY_STRING/$param | cut -f1`</td><td>`date -r /$QUERY_STRING/$param`</td></tr>"
fi
done
for param in `ls -F /$QUERY_STRING` ; do
if echo "$param" | egrep -v -q "/" ; then
param=${param/"*"}
echo "<tr><td><a href=fil.cgi?/$QUERY_STRING/$param"
echo " title=Скачать><img src=/modules/filemanager/file.png>"
echo $param
echo "</a></td><td><a href=# onclick=window.open('edit.cgi?/$QUERY_STRING$param','Edit','width=880,height=550,toolbar=0')  title=Редактировать><img src=/modules/filemanager/edit.png></a></td><td align=center><a href=?$QUERY_STRING$param&&_&&"
echo "onclick=\"return confirm('Удалить файл \'$param\'?')\" title=Удалить><img src=/modules/filemanager/del.png></a></td>"
echo "<td>`ls -lah /$QUERY_STRING/$param  | awk '{ print $5}'`</td><td>`date -r /$QUERY_STRING/$param`</td>
</tr>"
fi
done
echo "</table>"
echo "<iframe name=hidden src=/fil.cgi style=\"width:0px; height:0px; visibility:hidden\"></iframe>"
