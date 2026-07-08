# LCD HD44780 Driver for OpenWRT

Select language / Оберіть мову / Выберите язык:
- [English](#-english)
- [Українська](#-українська)
- [Русский](#-русский)

---

## 🇬🇧 English

Linux kernel driver for character LCDs (HD44780)
on OrangePi/RaspberryPi under OpenWRT (4-bit GPIO).

### ⚙️ GPIO Pin Configuration

The driver supports dynamic pin mapping. Configure
pins in two ways:

#### 1. Via OpenWRT UCI Config (`/etc/config/hd44780`)
The init script loads pins from UCI automatically:
```ini
config display 'pins'
	option hd_rs '0'
	option hd_rw '1'
	option hd_e '2'
	option hd_db4 '3'
	option hd_db5 '15'
	option hd_db6 '16'
	option hd_db7 '14'
```

#### 2. Manually via Console (In a Single Line)
Override pins and load the module manually:
```sh
insmod hd44780_mod.ko \
  hd_rs=0 hd_rw=1 hd_e=2 \
  hd_db4=3 hd_db5=15 hd_db6=16 hd_db7=14
```

---

### 📋 Supported Commands for `/dev/hd44780`

The first byte determines the operation mode:

1. **Text Mode (DDRAM)** — First byte **NOT** `0xff`/`0xfe`.
   Characters printed "as is". `\n` moves to line 2.
2. **Raw Command Mode** — First byte is **`0xff`**.
   Bytes are sent directly as LCD hardware instructions.
3. **CGRAM Programming Mode** — First byte is **`0xfe`**.
   Used for creating custom user icons.

---

### 💻 Terminal Usage Examples

#### 1. Clear Display
```sh
printf "\xff\x01" > /dev/hd44780
```

#### 2. Cursor Positioning (DDRAM addressing)
Use command `0x80` + address to shift text position.
* Row 1 start: `0x00` | Row 2 start: `0x40`

```sh
# Line 1, Position 1
printf "\xff\x80Text" > /dev/hd44780

# Line 2, Position 1
printf "\xff\xC0Text" > /dev/hd44780

# Line 2, Position 4 (0x40 + 0x03 = 0x43 -> 0xC3)
printf "\xff\xC3Text" > /dev/hd44780

# Line 1, Position 10 (0x80 + 0x09 = 0x89)
printf "\xff\x89Text" > /dev/hd44780
```

#### 3. Printing Multi-line Text (using `\n`)
```sh
printf "First line\nSecond line" > /dev/hd44780
```

#### 4. Writing to All 8 CGRAM Cells
```sh
#!/bin/sh
LCD="/dev/hd44780"
if [ ! -c "\$LCD" ]; then
    echo "Error: Driver \$LCD not found!"
    exit 1
fi
echo "Uploading 8 custom characters..."
# 0: Empty Box Frame
printf "\xfe\x00\x1f\x11\x11\x11\x11\x11\x11\x1f" > "\$LCD"
# 1: Smiley Face
printf "\xfe\x01\x00\x0a\x00\x00\x11\x0e\x00\x00" > "\$LCD"
# 2: Heart Icon
printf "\xfe\x02\x00\x0a\x1f\x1f\x0e\x04\x00\x00" > "\$LCD"
# 3: Arrow Right
printf "\xfe\x03\x04\x02\x01\x1f\x01\x02\x04\x00" > "\$LCD"
# 4: Full Battery
printf "\xfe\x04\x0e\x1b\x1b\x1b\x1b\x1b\x1b\x1f" > "\$LCD"
# 5: Hourglass
printf "\xfe\x05\x1f\x11\x0a\x04\x0a\x11\x1f\x00" > "\$LCD"
# 6: Bell
printf "\xfe\x06\x04\x0e\x0e\x0e\x1f\x00\x04\x00" > "\$LCD"
# 7: Fully Solid Box
printf "\xfe\x07\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f" > "\$LCD"

printf "\xff\x01" > "\$LCD"; sleep 0.05
printf "Icons: \x00 \x01 \x02 \x03 \x04 \x05 \x06 \x07" > "\$LCD"
```

---

## 🇺🇦 Українська

Драйвер ядра Linux для підключення текстових LCD (HD44780)
до OrangePi/RaspberryPi через 4-бітний GPIO на OpenWRT.

### ⚙️ Налаштування пінів GPIO

Драйвер підтримує динамічне призначення пінів:

#### 1. Через конфіг OpenWRT UCI (`/etc/config/hd44780`)
Скрипт ініціалізації зчитує піни з файлу автоматично:
```ini
config display 'pins'
	option hd_rs '0'
	option hd_rw '1'
	option hd_e '2'
	option hd_db4 '3'
	option hd_db5 '15'
	option hd_db6 '16'
	option hd_db7 '14'
```

#### 2. Вручну через консоль (Одним рядком)
Перевизначення дефолтних пінів без файлів конфігу:
```sh
insmod hd44780_mod.ko \
  hd_rs=0 hd_rw=1 hd_e=2 \
  hd_db4=3 hd_db5=15 hd_db6=16 hd_db7=14
```

---

### 📋 Підтримувані команди пристрою `/dev/hd44780`

Перший байт визначає режим роботи пристрою:

1. **Режим тексту (DDRAM)** — Перший байт **НЕ** `0xff`/`0xfe`.
   Виводиться як є. `\n` переносить курсор на рядок 2.
2. **Режим сирих команд** — Перший байт дорівнює **`0xff`**.
   Байти йдуть як службові команди (очищення, зсув).
3. **Режим програмування CGRAM** — Перший байт дорівнює
   **`0xfe`**. Використовується для створення власних іконок.

---

### 💻 Приклади використання в консолі OpenWRT

#### 1. Очищення екрана
```sh
printf "\xff\x01" > /dev/hd44780
```

#### 2. Позиціонування курсора (DDRAM адреси)
Для зміщення тексту використовується команда `0x80` + адреса.
* Рядок 1 старт: `0x00` | Рядок 2 старт: `0x40`

```sh
# Початок 1-го рядка
printf "\xff\x80Текст" > /dev/hd44780

# Початок 2-го рядка
printf "\xff\xC0Текст" > /dev/hd44780

# 2-й рядок, 4-а позиція (0x40 + 0x03 = 0x43 -> 0xC3)
printf "\xff\xC3Текст" > /dev/hd44780

# 1-й рядок, 10-та позиція (0x80 + 0x09 = 0x89)
printf "\xff\x89Текст" > /dev/hd44780
```

#### 3. Запис у CGRAM усіх 8 комірок та виведення
```sh
#!/bin/sh
LCD="/dev/hd44780"
if [ ! -c "\$LCD" ]; then
    echo "Помилка: Драйвер \$LCD не знайдено!"
    exit 1
fi
echo "Завантаження 8 символів у CGRAM..."
# 0: Порожня рамка | 1: Смайлик | 2: Серце | 3: Стрілка
printf "\xfe\x00\x1f\x11\x11\x11\x11\x11\x11\x1f" > "\$LCD"
printf "\xfe\x01\x00\x0a\x00\x00\x11\x0e\x00\x00" > "\$LCD"
printf "\xfe\x02\x00\x0a\x1f\x1f\x0e\x04\x00\x00" > "\$LCD"
printf "\xfe\x03\x04\x02\x01\x1f\x01\x02\x04\x00" > "\$LCD"
# 4: Батарейка | 5: Годинник | 6: Дзвіночок | 7: Кубик
printf "\xfe\x04\x0e\x1b\x1b\x1b\x1b\x1b\x1b\x1f" > "\$LCD"
printf "\xfe\x05\x1f\x11\x0a\x04\x0a\x11\x1f\x00" > "\$LCD"
printf "\xfe\x06\x04\x0e\x0e\x0e\x1f\x00\x04\x00" > "\$LCD"
printf "\xfe\x07\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f" > "\$LCD"

printf "\xff\x01" > "\$LCD"; sleep 0.05
printf "Icons: \x00 \x01 \x02 \x03 \x04 \x05 \x06 \x07" > "\$LCD"
```

---
---

## 🇷🇺 Русский

Драйвер ядра Linux для подключения текстовых LCD (HD44780)
к OrangePi/RaspberryPi через 4-битный GPIO на OpenWRT.

### ⚙️ Настройка пинов GPIO

Драйвер поддерживает динамическое назначение пинов:

#### 1. Через конфиг OpenWRT UCI (`/etc/config/hd44780`)
Скрипт инициализации считывает пины автоматически:
```ini
config display 'pins'
	option hd_rs '0'
	option hd_rw '1'
	option hd_e '2'
	option hd_db4 '3'
	option hd_db5 '15'
	option hd_db6 '16'
	option hd_db7 '14'
```

#### 2. Вручную через консоль (Одной строкой)
Переопределение дефолтных пинов без файлов конфигурации:
```sh
insmod hd44780_mod.ko \
  hd_rs=0 hd_rw=1 hd_e=2 \
  hd_db4=3 hd_db5=15 hd_db6=16 hd_db7=14
```

---

### 📋 Поддерживаемые команды устройства `/dev/hd44780`

Первый байт определяет режим работы:

1. **Режим текста (DDRAM)** — Первый байт **НЕ** `0xff`/`0xfe`.
   Выводится как есть. `\n` переносит курсор на строку 2.
2. **Режим сырых команд** — Первый байт равен **`0xff`**.
   Последующие байты идут как служебные команды дисплея.
3. **Режим программирования CGRAM** — Первый байт равен
   **`0xfe`**. Используется для генерации собственных иконок.

---

### 💻 Примеры использования в консоли OpenWRT

#### 1. Очистка экрана
```sh
printf "\xff\x01" > /dev/hd44780
```

#### 2. Позиционирование курсора (DDRAM адресация)
Для смещения текста используется команда `0x80` + адрес.
* Строка 1 старт: `0x00` | Строка 2 старт: `0x40`

```sh
# Начало 1-й строки
printf "\xff\x80Текст" > /dev/hd44780

# Начало 2-й строки
printf "\xff\xC0Текст" > /dev/hd44780

# 2-я строка, 4-я позиция (0x40 + 0x03 = 0x43 -> 0xC3)
printf "\xff\xC3Текст" > /dev/hd44780

# 1-я строка, 10-я позиция (0x80 + 0x09 = 0x89)
printf "\xff\x89Текст" > /dev/hd44780
```

#### 3. Запись в CGRAM всех 8 ячеек и вывод
```sh
#!/bin/sh
LCD="/dev/hd44780"
if [ ! -c "\$LCD" ]; then
    echo "Ошибка: Драйвер \$LCD не найден!"
    exit 1
fi
echo "Загрузка 8 кастомных символов в CGRAM..."
# 0: Рамка | 1: Смайлик | 2: Сердце | 3: Стрелка
printf "\xfe\x00\x1f\x11\x11\x11\x11\x11\x11\x1f" > "\$LCD"
printf "\xfe\x01\x00\x0a\x00\x00\x11\x0e\x00\x00" > "\$LCD"
printf "\xfe\x02\x00\x0a\x1f\x1f\x0e\x04\x00\x00" > "\$LCD"
printf "\xfe\x03\x04\x02\x01\x1f\x01\x02\x04\x00" > "\$LCD"
# 4: Батарейка | 5: Часы | 6: Колокольчик | 7: Кубик
printf "\xfe\x04\x0e\x1b\x1b\x1b\x1b\x1b\x1b\x1f" > "\$LCD"
printf "\xfe\x05\x1f\x11\x0a\x04\x0a\x11\x1f\x00" > "\$LCD"
printf "\xfe\x06\x04\x0e\x0e\x0e\x1f\x00\x04\x00" > "\$LCD"
printf "\xfe\x07\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f" > "\$LCD"

printf "\xff\x01" > "\$LCD"; sleep 0.05
printf "Icons: \x00 \x01 \x02 \x03 \x04 \x05 \x06 \x07" > "\$LCD"
```

