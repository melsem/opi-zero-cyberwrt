#!/bin/sh
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru

echo "Content-type: text/html; charset=utf-8"
echo ""

if [ -f /www/menu.html ]; then
    cat /www/menu.html
fi

MUSIC_ROOT="/home"
playlist="/etc/config/web-radio2"

echo "<script type='text/javascript'>
function startTime(){ 
    var tm=new Date(); var h=tm.getHours(); var m=tm.getMinutes(); var s=tm.getSeconds(); 
    m=checkTime(m); s=checkTime(s);
    var txtEl = document.getElementById('radio_clock');
    if (txtEl) { 
        txtEl.innerHTML=h+':'+m+':'+s; 
        setTimeout(startTime, 500); 
    } else {
        setTimeout(startTime, 100);
    }
}
function checkTime(i){ if (i<10) { i='0' + i;} return i; }

function sendAction(params, callback) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', 'action.cgi?' + params, true);
    xhr.onload = function() {
        if (xhr.status === 200 && callback) {
            callback(xhr.responseText.trim());
        }
    };
    xhr.send();
}

function updateStatus() {
    sendAction('cmd=get_status', function(response) {
        var statusEl = document.getElementById('player_status');
        if (statusEl) { statusEl.innerHTML = response; }
    });
}

function changeVolume(action) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', 'volume.cgi?act=' + action, true);
    xhr.onload = function() {
        if (xhr.status === 200) {
            var res = xhr.responseText.trim();
            if (!res) return;
            var data = res.split('|');
            var vol = data[0];     
            var status = data[1];  
            
            var volVal = document.getElementById('vol_val');
            if (volVal) { volVal.innerHTML = vol; }
            
            var muteBtn = document.getElementById('mute_btn');
            if (muteBtn) {
                if (status === 'off') {
                    muteBtn.innerHTML = 'Unmute';
                    muteBtn.className = 'btn-control btn-mute muted';
                } else {
                    muteBtn.innerHTML = 'Mute';
                    muteBtn.className = 'btn-control btn-mute';
                }
            }
        }
    };
    xhr.send();
}

function startAutoRefresh() {
    setInterval(function() {
        updateStatus();
        changeVolume('get'); 
    }, 3000);
}

function playURL() {
    var url = encodeURIComponent(document.getElementById('input_url').value);
    sendAction('URL=' + url, updateStatus);
}
function playFile() {
    var file = encodeURIComponent(document.getElementById('input_file').value);
    sendAction('file=' + file, updateStatus);
}
function playFolder() {
    var folder = encodeURIComponent(document.getElementById('select_folder').value);
    sendAction('folder=' + folder, updateStatus);
}

function togglePower(mode) {
    sendAction(mode + '=1', function(response) {
        if (mode === 'zapusk') {
            var statusEl = document.getElementById('player_status');
            if (statusEl) { statusEl.innerHTML = '<b>Включение radio...</b>'; }
            setTimeout(function() {
                location.reload();
            }, 2000);
        } else {
            location.reload();
        }
    });
}
function nextChannel() { sendAction('next=1', updateStatus); }
function playChannel(idx, name, url) { 
    sendAction('nomerstroki=' + idx + '&nameURL=' + encodeURIComponent(name) + '&URL=' + encodeURIComponent(url), function(response) {
        var statusEl = document.getElementById('player_status');
        if (statusEl) { statusEl.innerHTML = '<b>Включение канала ' + idx + '...</b>'; }
        setTimeout(function() {
            location.reload();
        }, 1500);
    }); 
}
function deleteChannel(idx) { if (confirm('Are you sure... Delete? ')) { sendAction('delchan=' + idx, function() { location.reload(); }); } }
function addChannel() {
    var name = encodeURIComponent(document.getElementById('add_name').value);
    var url = encodeURIComponent(document.getElementById('add_url').value);
    var n_str = document.getElementById('add_n_str').value;
    sendAction('add_nev_name=' + name + '&add_nev_url=' + url + '&n_str=' + n_str, function() { location.reload(); });
}

function saveTextarea() {
    var txt = encodeURIComponent(document.getElementById('text_list').value);
    var xhr = new XMLHttpRequest();
    xhr.open('POST', 'action.cgi', true);
    xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    xhr.onload = function() {
        if (xhr.status === 200) {
            alert('Плейлист збережено!');
            location.reload();
        } else {
            alert('Помилка: ' + xhr.status);
        }
    };
    xhr.send('text=' + txt);
}
function toggleAutoStart(mode) { sendAction(mode + '=1', function() { location.reload(); }); }

startTime();
startAutoRefresh();
</script>"

echo "<title>WEB-RADIO-2</title>"

if [ -f /etc/init.d/web-radio2.init ] && /etc/init.d/web-radio2.init enabled 2>/dev/null; then
    star_tup=0 ; clor="2196F3" ; onoff="autostartoff" ; cto="ON"
else
    onoff="autostarton" ; cto="OFF" ; clor="F44336" ; star_tup=1
fi

echo "<style>
  body { font-family: 'Segoe UI', Arial, sans-serif; background-color: #f4f6f9; color: #333; margin: 0; padding: 20px; }
  .radio-container { max-width: 900px; margin: 0 auto; background: #fff; padding: 25px; border-radius: 12px; box-shadow: 0 4px 15px rgba(0,0,0,0.08); }
  .player-widget { background: #2c3e50; color: #fff; padding: 20px; border-radius: 10px; margin-bottom: 25px; box-shadow: inset 0 0 10px rgba(0,0,0,0.3); }
  .player-widget .time { font-size: 14pt; color: #bdc3c7; margin-bottom: 5px; font-family: monospace; }
  .player-widget .status { font-size: 18pt; font-weight: bold; color: #2ecc71; }
  
  .control-panel { display: flex; flex-wrap: wrap; gap: 10px; align-items: center; background: #f8f9fa; padding: 15px; border-radius: 8px; margin-bottom: 25px; border: 1px solid #e9ecef; }
  .btn-control { font-size: 12pt; font-weight: bold; padding: 10px 20px; border: none; border-radius: 6px; cursor: pointer; transition: all 0.15s ease; text-transform: uppercase; user-select: none; }
  .input-group button { padding: 10px 20px; background: #34495e; color: #fff; border: none; border-radius: 6px; cursor: pointer; font-weight: bold; min-width: 120px; transition: all 0.15s ease; user-select: none; }
  .btn-play { background: #2ecc71; color: #fff; border: none; padding: 6px 15px; border-radius: 4px; cursor: pointer; font-weight: bold; transition: all 0.15s ease; user-select: none; }
  .btn-save-list { font-size: 13pt; background-color: #2ecc71; color: white; padding: 10px 30px; cursor: pointer; border: none; border-radius: 6px; font-weight: bold; margin-top: 10px; display: block; width: 100%; text-align: center; transition: all 0.15s ease; user-select: none; }
  /* ЭФФЕКТЫ НАВЕДЕНИЯ (HOVER) */
  .btn-start { background: #2ecc71; color: #fff; }
  .btn-start:hover { background: #27ae60; }
  .btn-stop { background: #e74c3c; color: #fff; }
  .btn-stop:hover { background: #c0392b; }
  .btn-next { background: #3498db; color: #fff; }
  .btn-next:hover { background: #2980b9; }
  .btn-vol { background: #95a5a6; color: #fff; font-size: 14pt; padding: 8px 15px; }
  .btn-vol:hover { background: #7f8c8d; }
  .btn-mute { background: #f39c12; color: #fff; }
  .btn-mute:hover { background: #d35400; }
  .btn-mute.muted { background: #d35400; }
  .btn-mute.muted:hover { background: #b33900; }
  .input-group button:hover { background: #2c3e50; }
  .btn-play:hover { background: #27ae60; }
  .btn-save-list:hover { background-color: #27ae60; }
  .btn-del { background: none; border: none; color: #e74c3c; font-size: 16pt; cursor: pointer; padding: 0 10px; transition: all 0.15s ease; }
  .btn-del:hover { color: #c0392b; transform: scale(1.15); }

  /* ЭФФЕКТЫ НАЖАТИЯ И УДЕРЖАНИЯ (ACTIVE) */
  .btn-control:active, .input-group button:active, .btn-play:active, .btn-save-list:active { transform: translateY(2px) scale(0.98); filter: brightness(0.85); box-shadow: inset 0 2px 4px rgba(0,0,0,0.2); }
  .btn-del:active { transform: scale(0.9); filter: brightness(0.8); }

  .vol-display { font-size: 16pt; font-weight: bold; min-width: 60px; text-align: center; color: #2c3e50; }
  .input-group { display: flex; width: 100%; margin-bottom: 10px; gap: 8px; }
  .input-group input, .input-group select { flex: 1; padding: 10px; border: 1px solid #ced4da; border-radius: 6px; font-size: 11pt; }
  .stations-table { width: 100%; border-collapse: collapse; margin-top: 20px; background: #fff; }
  .stations-table th { background: #34495e; color: #fff; padding: 12px; text-align: left; font-size: 11pt; }
  .stations-table td { padding: 10px; border-bottom: 1px solid #dee2e6; vertical-align: middle; }
  .stations-table tr:hover { background-color: #f1f3f5; }
  .station-name-input { width: 100%; padding: 8px; border: 1px solid #e9ecef; background: #fff; border-radius: 4px; font-size: 11pt; color: #495057; font-weight: 500; }
  .meta-panel { margin-top: 30px; padding: 20px; background: #f8f9fa; border-radius: 8px; border: 1px solid #e9ecef; }
  textarea { width: 100%; box-sizing: border-box; border: 1px solid #ced4da; border-radius: 6px; font-family: monospace; font-size: 10.5pt; padding: 10px; background: #fff; resize: vertical; }
  .autostart-text { font-size: 12pt; font-weight: bold; }
</style>"

echo "<div class='radio-container'>"

echo "<div class='player-widget'>"
echo "  <div class='time' id='radio_clock'>00:00:00</div>"
echo "  <div>Сейчас играет: <span class='status' id='player_status'>Loading...</span></div>"
echo "</div>"

echo "<div class='input-group'>"
echo "  <input type='text' id='input_url' placeholder='Введите URL радиостанции (например, http://hostingradio.ru)'>"
echo "  <button onclick='playURL()'>Play URL</button>"
echo "</div>"

echo "<div class='input-group'>"
echo "  <input type='text' id='input_file' placeholder='Путь к mp3 файлу (например, /home/music/track.mp3)'>"
echo "  <button onclick='playFile()'>Play File</button>"
echo "</div>"

echo "<div class='input-group'>"
echo "  <select id='select_folder'>"
find "$MUSIC_ROOT" -type f -name "*.mp3" -exec dirname {} \; 2>/dev/null | sort -u | while read -r dir; do echo "<option value=\"$dir\">$dir</option>"; done
echo "  </select>"
echo "  <button onclick='playFolder()'>Play Folder</button>"
echo "</div><br>"

echo "<div class='control-panel'>"
if ! pidof madplay &>/dev/null && ! pidof curl &>/dev/null; then
    echo "  <button class='btn-control btn-start' onclick='togglePower(\"zapusk\")'>START RADIO</button>"
else
    echo "  <button class='btn-control btn-stop' onclick='togglePower(\"ostanov\")'>STOP RADIO</button>"
fi
echo "  <button class='btn-control btn-next' onclick='nextChannel()'>Next channel</button>"

echo "  <div style='margin-left: auto; display: flex; align-items: center; gap: 10px;'>"
echo "    <button class='btn-control btn-vol' onclick='changeVolume(\"down\")'>─</button>"
echo "    <span class='vol-display' id='vol_val'>0%</span>"
echo "    <button class='btn-control btn-vol' onclick='changeVolume(\"up\")'>┼</button>"
echo "    <button id='mute_btn' class='btn-control btn-mute' onclick='changeVolume(\"mute\")'>Mute</button>"
echo "  </div>"
echo "</div>"

echo "<h3>Список сохраненных радиостанций</h3>"
echo "<table class='stations-table'>"
echo "<thead><tr><th width='40'>Уд.</th><th width='40'>№</th><th>Название станции</th><th width='70'>Эфир</th></tr></thead>"
echo "<tbody>"

b=$(grep -c "^#EXTINF" "$playlist" 2>/dev/null) ; : "${b:=0}"

if [ -f "$playlist" ]; then
    while read -r line1 || [ -n "$line1" ]; do
        if echo "$line1" | grep -q "^#EXTINF"; then
            real_id=$(echo "$line1" | grep -o 'EXTINF:-[0-9]\+' | cut -d- -f2)
            name=$(echo "$line1" | cut -d, -f2- | tr -d '"' | tr -d "'")
            read -r url
            
            [ -z "$real_id" ] && continue
            [ -z "$name" ] && name="Radio $real_id"
            [ -z "$url" ] && continue

#            js_name=$(echo "$name" | sed "s/\"/\\\\\"/g")
#            js_url=$(echo "$url" | sed "s/\"/\\\\\"/g")
#    Замінюємо подвійні лапки на HTML-сутність &quot;
            js_name=$(echo "$name" | sed 's/"/\&quot;/g')
            js_url=$(echo "$url" | sed 's/"/\&quot;/g' | sed "s/'//g")
#    Про всяк випадок прибираємо одинарні лапки, щоб вони не конфліктували
#            js_name=$(echo "$js_name" | sed "s/'//g")


            echo "<tr>"
            echo "  <td align='center'><button class='btn-del' onclick='deleteChannel($real_id)'>&times;</button></td>"
            echo "  <td style='color: #7f8c8d; font-weight: bold;'>$real_id</td>"
            echo "  <td><input class='station-name-input' readonly value=\"$name\" title=\"$url\"></td>"
            echo "  <td><button class='btn-play' onclick='playChannel($real_id, \"$js_name\", \"$js_url\")'>play</button></td>"
            echo "</tr>"
        fi
    done < "$playlist"
fi
echo "</tbody></table><br>"

next_b=$((b + 1))
echo "<h3>Быстрое добавление</h3>"
echo "<div class='input-group'>"
echo "  <input type='hidden' id='add_n_str' value='$next_b'>"
echo "  <input type='text' id='add_name' placeholder='Название станции (например, Lux FM)' style='flex: 0.4;'>"
echo "  <input type='text' id='add_url' placeholder='Поток URL (http://...)'>"
echo "  <button onclick='addChannel()' style='background: #2ecc71;'>Сохранить</button>"
echo "</div>"

echo "<div class='meta-panel'>"
echo "  <div style='display: flex; align-items: center; justify-content: space-between; margin-bottom: 15px;'>"
echo "    <span class='autostart-text'>Автозапуск при старте роутера: <span style='color: #$clor;'>$cto</span></span>"
if [ "$star_tup" -eq 1 ]; then 
    echo "    <button class='btn-control btn-start' style='padding: 6px 15px; font-size: 10pt;' onclick='toggleAutoStart(\"autostarton\")'>Включить</button>"
else 
    echo "    <button class='btn-control btn-stop' style='padding: 6px 15px; font-size: 10pt;' onclick='toggleAutoStart(\"autostartoff\")'>Выключить</button>"
fi
echo "  </div>"

kollw_str=$(sed -n '$=' "$playlist" 2>/dev/null) ; : "${kollw_str:=5}"
echo "  <textarea rows='$kollw_str' id='text_list'>"
cat "$playlist" 2>/dev/null
echo "  </textarea>"
echo "  <button class='btn-save-list' onclick='saveTextarea()'>Синхронизировать и сохранить плейлист</button>"
echo "</div>"

echo "</div>"

if [ ! -f /www/menu.html ]; then
    echo "</body></html>"
fi

