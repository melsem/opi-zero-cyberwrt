#!/bin/sh
#Copyright (C) 2015 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
if echo "$QUERY_STRING" | egrep -q "mtd" ; then
	#echo "Content-type: text/html; charset=utf-8"
	#echo
	s=${QUERY_STRING//R/_}
	s=$s"`date +"%d_%m_%Y_%H-%M-%S"`.bin"
	echo "Content-Type: application/octet-stream"
	echo "Content-Disposition: attachment; filename=$s"
	echo
	string=${QUERY_STRING//R/ }
	string=${string//mtd/mtdblock}
	for i in $string ; do
		cat /dev/$i
	done
	exit 0
fi
echo "Content-type: text/html; charset=utf-8"
echo
echo "<title>Бэкап ROM</title>"
echo `cat /www/menu.html`
echo "
<style>
	.h, #f{display:none;}
	.checked{background-color:#CCFFCC; cursor:default;}
	.nums{border: solid 1px black; width:20px; height:20px; text-align:center}
	table {width:500px; cursor:pointer;}
   	table tr:nth-child(1) { background: #667; color: #fff;  cursor:default;}
   	table tr:hover { background: silver;}
</style>
<body><h2>Бэкап</h2><table border=0>
<tr><td></td><td>Dev</td><td>Size</td><td>Name</td></tr>
<textarea id=source class=h>`cat /proc/mtd | sed -n '2,$p'`</textarea><iframe id=f></iframe>
<script>
cArr=document.getElementsByTagName('input');
function clr(){
	var strn=document.getElementById('span');
	trArr=document.getElementsByTagName('tr');
	for(i=1;i<trArr.length-1;i++){
		trArr[i].className='';
		trArr[i].firstChild.firstChild.innerHTML='';
	}
	string='';
	sort=0;
}
var f=document.getElementById('f');
function mtdArr(){
	var source = document.getElementById('source').value;
	mtd=[];
	var mtd=source.split(/\n/);
	for(i=0;i<mtd.length; i++){
		mtd[i]= mtd[i].split(' ');
	}
	return mtd;
}
var sort=0;
var string='';
function str(mtd){
	var tr=document.getElementById(mtd);
	if(tr.className !== 'checked'){
		sort=sort+1;
		tr.className='checked';
		string=string + mtd + 'R';
		tr.firstChild.firstChild.innerHTML=sort;	
	}
}
function sbmt(){
	if(string.length === 0){
		alert('Ничего не выбрано!');
	}else{
		f.contentWindow.location.href='index.cgi?'+string;
	}
}
	
mtdArray=mtdArr();
for(i=0;i<mtdArray.length;i++){
	document.write('<tr class=\"\" id='+mtdArray[i][0].slice(0,-1)+' onclick=\"str(\''+mtdArray[i][0].slice(0,-1)+'\')\"><td align=center><div class=nums></div></td><td>'+mtdArray[i][0]+'</td><td>'+mtdArray[i][1]+'</td><td>'+mtdArray[i][3]+'</td></tr>');
	var l=i;
}
document.write('<tr><td colspan=4><button onclick=\"sbmt()\">Бэкап</button><button onclick=\"clr()\">Очистить</button></tr>');
</script>
</form>
</table>
</body>"