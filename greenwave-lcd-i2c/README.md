# 📟 Greenwave-lcd-i2c Display Driver for OpenWrt

A modular, hardware-optimized software driver for **HD44780** character displays.
Supports standard liquid crystal screen models **1602, 1604, 2002, 2004, 4004**.
Operates via the **PCF8574** I2C remote I/O expander chip.
Designed and tailored for embedded multimedia platforms powered by **OpenWrt**.

---

## 🚀 Key Features

- **Full UCI Integration**:
  All hardware profiles and options are parsed dynamically from the system config.
- **Flicker-Free Realtime Engine**:
  Analyzes character mutations buffer-by-buffer.
  Only modified array blocks are written to the I2C bus.
- **Smart Adaptive Scrolling**:
  Long metadata strings automatically scroll from right to left.
  Features a configurable delay loop (gap padding).
  Short strings are seamlessly locked into static center alignment.
- **Advanced Unicode (UTF-16) Parser**:
  Translates 2-byte Ukrainian UTF-8 text streams into positive integer codes.
  Prevents signed-bit overflow glitches on specific alphabetic segments.
- **Maximum CGRAM Hardware Utilization**:
  Fills all 8 internal RAM slots with unique custom fonts.
  Features an analytically accurate grid matrix for the **Ф** and **Ю** glyphs.
  Saves space by using the factory hardcoded character `249` for **Ц**.
- **Dynamic Adaptable Progress Bar**:
  Graphical volume level scale with 5 selectable layouts.
  Length automatically resizes based on the configuration of columns.

---

## 📂 Project Directory Tree

The source code tree is split into separate functional modules:

- `src/display.c` — Hardware I2C communications, core LCD init steps, and UCI parser.
- `src/lcd_print_cyr.c` — Dedicated UTF-16 character translation switch block.
- `src/main.c` — Master process loop, tick scheduling, scrolling logic, and progress bar layout.
- `src/Makefile` — Internal multi-object linking script for `main.o`, `display.o`, and `lcd_print_cyr.o`.

---

## 🛠 UCI System Configuration (`/etc/config/greenwave-lcd-i2c`)

Configuration profile must be placed within an anonymous layout block:

```ini
package greenwave-lcd-i2c

config greenwave
	option i2c_dev '1'
	option lcd_adres '0x27'
	option lcd_cols '20'
	option lcd_rows '4'
	option lcd_backlight '1'
	option tmp_file '/tmp/play_name'
	option vol_file '/tmp/tda_volume'
	option language 'en_en'
	option scroll_gap '5'
	option vol_style '4'
```

### Options Specification:

- `i2c_dev` — Target Linux I2C master core bus controller index.
- `lcd_adres` — Hardware hexadecimal address of the PCF8574 chip (usually `0x27`).
- `lcd_cols` — Physical screen horizontal width (characters count per row).
- `lcd_rows` — Physical screen vertical height (available rows on glass).
- `lcd_backlight` — LED backlight strip toggle switch (`1` — on, `0` — off).
- `tmp_file` — Input text source file for track names or state modes.
- `vol_file` — Input numerical source file for the volume variable (0–100%).
- `language` — Localization selector (`en_ua` — factory matrix, `en_en` — CGRAM font).
- `scroll_gap` — Visual white-space spacing gap between scrolling string loops.
- `vol_style` — Graphical progress bar layout skin identifier (ranges from 1 to 5).

---

## 📊 Volume Indicator UI Themes (`vol_style`)

- **`1`** — Heavy blocks style: filled segments drawn via `██` (hex `0xFF`), empty spots are clear.
- **`2`** — Fine-needle matrix style: active tracks drawn via thin lines `||`, empty spots are dots `.`.
- **`3`** — Modern slider track: thin line track `-` with a single trailing head block `■`.
- **`4`** — Audio-receiver grid: thick twin rails `=` representing filled areas, empty spots are spaces.
- **`5`** — Retro arrow pointer: track line `-` with a single advancing arrow cursor `>`.

---

## 💻 Compiling inside the OpenWrt SDK

To cross-compile the codebase, drop the primary `package/greenwave-lcd-i2c/Makefile` package definitions script.

### Binary Compilation:
Execute this action inside the main root folder of your configured OpenWrt toolchain:
```bash
make package/greenwave-lcd-i2c/compile V=s
```

---

## 📈 Runtime Interprocess Operations

The binary agent functions as a background standalone daemon process.
To push text strings, have external scripts stream data directly to target runtime temp storage files:

```bash
# Update first line context (instant scrolling fallback or center-lock)
echo "Радіо П'ятниця" > /tmp/play_name

# Update second line context (recalculates layout and percentages instantly)
echo "68" > /tmp/tda_volume
```

## 📄 License
Released under the terms of the MIT open-source license.

---

# 📟 Драйвер Дисплея Greenwave-lcd-i2c для OpenWrt

Модульний, апаратно-оптимізований драйвер для символьних LCD-дисплеїв **HD44780**.
Сумісний із поширеними моделями екранів **1602, 1604, 2002, 2004, 4004**.
Працює через інтерфейсний мікросхемний розширювач I2C порту **PCF8574**.
Створений для вбудованих мультимедійних комплексів на базі ОС **OpenWrt**.

---

## 🚀 Ключові можливості

- **Повна інтеграція з UCI**:
  Усі апаратні конфігурації автоматично зчитуються із системних файлів ОС роутера.
- **Робота без мерехтіння (Realtime Engine)**:
  Посимвольний аналітик поточних текстових буферів.
  На шину I2C надсилаються команди лише для тих знаків, що фактично змінилися.
- **Розумний адаптивний скролінг**:
  Довгі текстові рядки плавно прокручуються з права на ліво.
  Зазор між кінцем і початком тексту (пауза) регулюється через конфіг.
  Короткі тексти автоматично центруються на склі без навантаження на шину.
- **Перекодувальник Unicode (UTF-16)**:
  Безпечний переклад двохбайтних UTF-8 символів українського алфавіту.
  Повністю усуває баг із від'ємними знаковими зміщеннями для літер **р, с, т**.
- **100% утилізація пам'яті CGRAM**:
  Заповнення всіх 8 внутрішніх комірок унікальними графічними гліфами.
  Впроваджено ідеальну точкову геометрію для літер **Ф** та **Ю**.
  Заощаджено пам'ять завдяки фабричному вбудованому коду `249` для літери **Ц**.
- **Адаптивна шкала гучності**:
  Графічний індикатор з 5 різними візуальними темами відображення на вибір.
  Ширина шкали автоматично розраховується під будь-який формат колонок екрана.

---

## 📂 Програмна структура каталогу

Вихідний код розділено на окремі ізольовані модулі:

- `src/display.c` — Низькорівневий обмін I2C, ініціалізація чипа та UCI парсер.
- `src/lcd_print_cyr.c` — Таблиця викликів та перекодування літер кирилиці UTF-16.
- `src/main.c` — Головний суперцикл, диспетчер кроків анімації, скролінг та Progress Bar.
- `src/Makefile` — Внутрішній Makefile для роздільної компіляції об'єктів `main.o`, `display.o` та `lcd_print_cyr.o`.

---

## 🛠 Налаштування UCI (`/etc/config/greenwave-lcd-i2c`)

Параметри вносяться в прошивку у вигляді анонімної секції:

```ini
package greenwave-lcd-i2c

config greenwave
	option i2c_dev '1'
	option lcd_adres '0x27'
	option lcd_cols '20'
	option lcd_rows '4'
	option lcd_backlight '1'
	option tmp_file '/tmp/play_name'
	option vol_file '/tmp/tda_volume'
	option language 'en_en'
	option scroll_gap '5'
	option vol_style '4'
```

### Специфікація налаштувань:

- `i2c_dev` — Індексний номер активної апаратної шини I2C у роутері.
- `lcd_adres` — Шістнадцяткова адреса дисплея на шині (зазвичай `0x27`).
- `lcd_cols` — Фізична ширина екрана (максимальна кількість символів у рядку).
- `lcd_rows` — Фізична висота екрана (кількість ліній на склі).
- `lcd_backlight` — Керування світлодіодною підсвіткою (`1` — увімкнено, `0` — вимкнено).
- `tmp_file` — Файл-джерело текстової інформації для відображення на першому рядку.
- `vol_file` — Файл-джерело поточної цифри гучності підсилювача (значення від 0 до 100%).
- `language` — Вибір знакогенератора (`en_ua` — заводський ROM, `en_en` — генерація у CGRAM).
- `scroll_gap` — Кількість порожніх пробілів-розділювачів у циклі бігучого рядка.
- `vol_style` — Номер графічної теми оформлення шкали гучності (від 1 до 5).

---

## 📊 Графічні стилі індикатора гучності (`vol_style`)

- **`1`** — Щільні класичні блоки: заповнені зони малюються кубиками `██` (код `0xFF`), пусті — пробілами.
- **`2`** — Тонкі ділення еквалайзера: заповнені зони відображаються паличками `||`, пусті — крапками `.`.
- **`3`** — Повзунок плеєра з точкою: шкала малюється лінією `-` із одним головним кубиком `■` наприкінці.
- **`4`** — Доріжки аудіо-ресивера: заповнені зони малюються подвійною лінією `=`, пусті — пробілами.
- **`5`** — Повзунок плеєра зі стрілкою: трек малюється лінією `-` із рухомим покажчиком гучності `>`.

---

## 💻 Крос-компіляція в OpenWrt SDK

Для збірки пакету використовується стандартний зовнішній скрипт `package/greenwave-lcd-i2c/Makefile`.

### Запуск збірки:
Виконайте команду в головному робочому каталозі вашого налаштованого OpenWrt SDK:
```bash
make package/greenwave-lcd-i2c/compile V=s
```

---

## 📈 Керування та взаємодія в системі

Програма працює автономно у фоновому режимі як системна служба (демон).
Для виведення даних інші процеси або bash-скрипти мають просто перезаписати файли в `/tmp`:

```bash
# Оновити статус першого рядка (текст відразу зацентрується або почне скролінг)
echo "Радіо П'ятниця" > /tmp/play_name

# Оновити рівень гучності (миттєвий перерахунок довжини прогрес-бару та цифри відсотків)
echo "68" > /tmp/tda_volume
```

## 📄 Ліцензія
Проект поширюється під вільною відкритою ліцензією MIT.
