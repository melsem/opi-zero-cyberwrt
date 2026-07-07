#!/bin/sh

# Функція декодування URL (видаляє каретки %0D, міняє + на пробіли, декодує %XX)
unescape() {
    echo "$1" | sed 's/%0[Dd]//g; s/+/ /g; s/%/\\x/g' | xargs -0 printf "%b"
}

# 1. Обов'язкові HTTP-заголовки
echo "Content-type: text/html; charset=utf-8"
echo ""

# Ініціалізація лічильників та змінних (Блок BEGIN)
cbr=0; tbr=0; abr=0; dsm=0
menu1=0; menu1nosynk=0; menu2=0

# 2. Зчитування POST або GET даних
if [ "$REQUEST_METHOD" = "POST" ] && [ -n "$CONTENT_LENGTH" ] && [ "$CONTENT_LENGTH" -gt 0 ]; then
    POST_DATA=$(dd bs=1 count=$CONTENT_LENGTH 2>/dev/null)
else
    POST_DATA="$QUERY_STRING"
fi

# 3. Парсинг потоку даних (еквівалент RS="&" та FS="=")
OLD_IFS=$IFS
IFS='&'

for pair in $POST_DATA; do
    IFS=$OLD_IFS
    key="${pair%%=*}"
    val_raw="${pair#*=}"
    
    val=$(unescape "$val_raw")

    case "$key" in
        # --- Секція ПРИЛАД ---
        prbor)
            tpbr="$val"
            uci set termostat-serial-arduino.@displ_tip[0].prbor="$val"
            kdl=${#tpbr}  # Підрахунок кількості символів
            [ "$kdl" -gt 54 ] && kdl=54
            ;;
        tprbor)
            [ "$tpbr" != "$val" ] && cbr=$((cbr + 1))
            ;;
        fPort)
            cfport="$val"
            ;;
        save_prbor)
            if [ "$cbr" -ne 0 ]; then
                uci commit termostat-serial-arduino
                echo "d+=0 $kdl $tpbr" > "/dev/tty$cfport"
            fi
            ;;

        # --- МЕНЮ 1 (Порт та налаштування екрану) ---
        speed)
            nevSpeed="$val"
            uci set port_listen.@displ_tip[0].speed="$nevSpeed"
            ;;
        tspeed)
            [ "$nevSpeed" != "$val" ] && tbr=$((tbr + 1))
            ;;
        devPort)
            nevDevPort="$val"
            uci set port_listen.@displ_tip[0].devPort="$nevDevPort"
            uci set termostat-serial-arduino.@displ_tip[0].updateRead=''
            ;;
        tdevPort)
            stariyDevPort="$val"
            [ "$nevDevPort" != "$stariyDevPort" ] && tbr=$((tbr + 1))
            ;;
        t_file)
            arfile="$val"
            uci set port_listen.@displ_tip[0].t_file="$val"
            ;;
        tt_file)
            [ "$arfile" != "$val" ] && tbr=$((tbr + 1))
            ;;
        i2c_adres)
            uci set termostat-serial-arduino.@displ_tip[0].i2c_adres="$val"
            ic="$val"
            ;;
        ti2c_adres)
            [ "$ic" != "$val" ] && abr=$((abr + 1))
            ;;
        simvol)
            uci set termostat-serial-arduino.@displ_tip[0].simvol="$val"
            sm="$val"
            ;;
        tsimvol)
            [ "$sm" != "$val" ] && abr=$((abr + 1))
            ;;
        strok)
            uci set termostat-serial-arduino.@displ_tip[0].strok="$val"
            sr="$val"
            ;;
        tstrok)
            [ "$sr" != "$val" ] && abr=$((abr + 1))
            ;;
        w1_gpio)
            uci set termostat-serial-arduino.@arduino[0].w1_gpio="$val"
            wr="$val"
            ;;
        tw1_gpio)
            [ "$wr" != "$val" ] && abr=$((abr + 1))
            ;;
        save_commit)
            menu1=$((menu1 + 1))
            ;;
        save_commit_synk)
            menu1=$((menu1 + 1))
            menu1nosynk=$((menu1nosynk + 1))
            ;;

        # --- МЕНЮ 2 (Конфігурація датчиків) ---
        poryadk)
            aa="$val"
            mc=$((val - 1))
            ;;
        dsPort)
            comport="$val"
            ;;

        # Фінальне збереження датчика (відправка в UART та UCI)
        save_commit_sensor)
            if [ "$dsm" -ne 0 ]; then
                if [ "$mc" -lt 4 ]; then
                    echo "b+=$mc $dl $name $mac $tip $nGp $invers $allar $hist/" > "/dev/tty$comport"
                else
                    echo "b+=$mc $dl $name $mac/" > "/dev/tty$comport"
                fi
                menu2=1
                sleep 2
                uci commit termostat-serial-arduino
            fi
            ;;

        # Обробка динамічних полів датчиків (закінчуються на значення $aa)
        *)
            # Перевіряємо, чи збігається кінець імені поля з поточним номером датчика $aa
            if [ -n "$aa" ]; then
                case "$key" in
                    name_snr"$aa")
                        name="$val"
                        dl=${#name}
                        [ "$dl" -gt 13 ] && dl=13
                        uci set termostat-serial-arduino.@arduino[0].name_snr"${aa}"="$name"
                        ;;
                    tname_snr"$aa")
                        [ "$name" != "$val" ] && dsm=$((dsm + 1))
                        ;;
                    MAC"$aa")
                        mac="$val"
                        uci set termostat-serial-arduino.@arduino[0].MAC"${aa}"="$mac"
                        ;;
                    tMAC"$aa")
                        [ "$mac" != "$val" ] && dsm=$((dsm + 1))
                        ;;
                    tip_ds"$aa")
                        tip="$val"
                        uci set termostat-serial-arduino.@arduino[0].tip_ds"${aa}"="$tip"
                        ;;
                    ttip_ds"$aa")
                        [ "$tip" != "$val" ] && dsm=$((dsm + 1))
                        ;;
                    nGp"${aa}"_out)
                        nGp="$val"
                        uci set termostat-serial-arduino.@arduino[0].nGp"${aa}"_out="$nGp"
                        ;;
                    tnGp"${aa}"_out)
                        [ "$nGp" != "$val" ] && dsm=$((dsm + 1))
                        ;;
                    invers"$aa")
                        invers="$val"
                        uci set termostat-serial-arduino.@arduino[0].invers"${aa}"="$invers"
                        ;;
                    tinvers"$aa")
                        [ "$invers" != "$val" ] && dsm=$((dsm + 1))
                        ;;
                    alarm"$aa")
                        # Арифметика Busybox sh (множення на 10)
                        allar=$((val * 10))
                        uci set termostat-serial-arduino.@arduino[0].alarm"${aa}"="$allar"
                        ;;
                    talarm"$aa")
                        [ "$allar" != "$val" ] && dsm=$((dsm + 1))
                        ;;
                    histe"$aa")
                        hist=$((val * 10))
                        uci set termostat-serial-arduino.@arduino[0].histe"${aa}"="$hist"
                        ;;
                    thiste"$aa")
                        [ "$hist" != "$val" ] && dsm=$((dsm + 1))
                        ;;
                esac
            fi
            ;;
    esac
    IFS='&'
done
IFS=$OLD_IFS

# 4. Блок END (Фінальні системні дії та редиректи)
if [ "$menu1" -ne 0 ]; then
    if [ "$abr" -ne 0 ] || [ "$tbr" -ne 0 ]; then
        echo "a+=0 $nevSpeed $sm $sr $wr $ic" > "/dev/tty$stariyDevPort"
    fi
    [ "$abr" -ne 0 ] && uci commit termostat-serial-arduino
    if [ "$tbr" -ne 0 ]; then
        uci commit port_listen
        sleep 1
        /etc/init.d/port_listen.init restart
        sleep 2
    fi
    
    if [ "$menu1nosynk" -ne 0 ]; then
        /www/cgi-bin/modules/termostat-serial-arduino/sync.cgi 1
    else
        /www/cgi-bin/modules/termostat-serial-arduino/setupMenu.cgi
    fi
elif [ "$menu2" -ne 0 ]; then
    /www/cgi-bin/modules/termostat-serial-arduino/sync.cgi 0
else
    /www/cgi-bin/modules/termostat-serial-arduino/setupMenu.cgi
fi

