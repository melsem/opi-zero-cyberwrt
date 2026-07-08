# 📟 Greenwave-lcd-i2c Display Driver for OpenWrt

Select language / Оберіть мову:
- [English](#english)
- [Українська](#українська)

---

## English

Modular software driver for **HD44780** character displays.
Supports standard liquid crystal screens: **1602, 1604, 2002, 2004, 4004**.
Operates via the **PCF8574** I2C expander chip.
Designed for embedded media platforms on **OpenWrt**.

---

## 🚀 Key Features

- **Full Libuci Integration**:
  Hardware options are parsed via native **C-API (`libuci`)**.
  Elimines shell forks (`popen`).
  Drastically reduces CPU overhead.
- **Flicker-Free Engine**:
  Analyzes character mutations buffer-by-buffer.
  Static fields block subsequent bus updates.
  Ensures smooth execution without flickering.
- **Adaptive Scrolling**:
  Long metadata strings scroll right to left.
  Configurable via `scroll_gap` padding.
  Short strings lock into static center alignment.
- **Smart Caps Lock**:
  Live song names automatically become **UPPERCASE** (`A-Z`).
  Ensures extreme uniformity on alphanumeric grids.
  System errors preserve native lower-case letters.
- **RAM Font Optimization**:
  Fills CGRAM slots dynamically based on localization.
  Offloads standard letters to native hardware equivalents.
  Unlocks dedicated high-definition glyphs for **Л**, **Ф**, **Ю**.
  Maps **П** onto the hardware `π` glyph (`0xF7`).
- **Flash Memory Guard**:
  Enforces strict `/tmp/` RAM-storage safety checks.
  Blocks `fopen` routines on persistent ROM.
  Prevents flash storage wear and hardware degradation.
- **Isolated Error Banners**:
  Features completely autonomous row monitors.
  Broken paths switch into independent scrolling warnings.
  Healthy display rows continue to function normally.
- **Modern Procd Daemon**:
  Managed fully by OpenWrt core init controller.
  Includes automatic hot-crash respawn routines.
  Performs atomic state cleanups upon shutdown.
- **Dynamic Progress Bar**:
  Graphical volume scale with 5 selectable layouts.
  Length automatically resizes based on screen columns.

---

## 📂 Project Directory Tree

Source code tree functional modules:

- `src/display.c` — Libuci reader, I2C logic, CGRAM allocator.
- `src/lcd_print_cyr.c` — Multi-mode Cyrillic Unicode translator.
- `src/main.c` — Master loop, `stat()` triggers, row scrollers.
- `src/Makefile` — Linking script with `-luci` linker flag.

---

## 🛠 UCI System Configuration (`/etc/config/greenwave-lcd-i2c`)

Profile must be placed within an anonymous layout:

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

- `i2c_dev` — Target Linux I2C master bus index.
- `lcd_adres` — Hex address of PCF8574 chip (usually `0x27`).
- `lcd_cols` — Physical screen character count per row.
- `lcd_rows` — Available rows count on glass.
- `lcd_backlight` — LED backlight switch (`1` — on, `0` — off).
- `tmp_file` — Input text file path. **Must be in `/tmp/`**.
- `vol_file` — Input numerical file path (0–100%). **Must be in `/tmp/`**.
- `language` — Localization and ROM matrix configuration selector:
  - **`en_en`** — Standard Western ROMs. Allocates 8 slots for **Б, Г/Ґ, Д, Ж, И, Л, Ф, Ю**.
  - **`en_ua`** / **`en_ru`** — For Cyrillic ROM tables (HD44780 A02). Disables CGRAM to save I2C bandwidth. Directly maps text into hardware blocks. Enforces upper-case conversion.
- `scroll_gap` — Space spacing gap between scrolling string loops.
- `vol_style` — Graphical progress bar layout identifier (1 to 5).

---

## 📊 Volume Indicator UI Themes (`vol_style`)

- **`1`** — Heavy blocks style: filled segments drawn via `██`.
- **`2`** — Fine-needle matrix style: active tracks are `||`, empty spots are `.`.
- **`3`** — Modern slider track: thin line `-` with head block `■`.
- **`4`** — Audio-receiver grid: thick twin rails `=` representing filled areas.
- **`5`** — Retro arrow pointer: track line `-` with advancing arrow `>`.

---

## 💻 Compiling inside the OpenWrt SDK

Drop definitions script inside `package/greenwave-lcd-i2c/Makefile`.

### Binary Compilation:
Execute this action inside the main root folder:
```bash
make package/greenwave-lcd-i2c/compile V=s
```

---

## ⚙️ Service Control Daemon (Procd)

Process is managed by native initialization subsystem (`init.d`).

```bash
# Start the driver instance
/etc/init.d/greenwave-lcd-i2c start

# Stop the daemon (blanks the matrix)
/etc/init.d/greenwave-lcd-i2c stop

# Restart the service context
/etc/init.d/greenwave-lcd-i2c restart
```

---

## 📈 Interprocess Operations

Daemon is optimized with quick file `stat()` triggers.
File reads execute *only* upon actual buffer modification.
External scripts stream data directly to target temp files:

```bash
# Update first line context (instant scrolling)
echo "Радіо П'ятниця" > /tmp/play_name

# Update second line context (recalculates layout)
echo "68" > /tmp/tda_volume
```

## 📄 License
Released under the terms of the MIT open-source license.

---

## Українська

Модульний програмний драйвер для символьних дисплеїв **HD44780**.
Підтримує стандартні моделі рідкокристалічних екранів: **1602, 1604, 2002, 2004, 4004**.
Працює через мікросхему розширювача портів I2C **PCF8574**.
Розроблено для вбудованих медіаплатформ на базі **OpenWrt**.

---

## 🚀 Ключові особливості

- **Повна інтеграція з Libuci**:
  Параметри заліза зчитуються через рідне **C-API (`libuci`)**.
  Повністю виключає важкі системні виклики (`popen`).
  Радикально знижує навантаження на процесор роутера.
- **Двигун без блимання (Flicker-Free)**:
  Аналізує зміни символів буфер за буфером.
  Статичні поля блокують повторні записи в шину I2C.
  Забезпечує плавну роботу екрана без мерехтіння.
- **Адаптивна прокрутка**:
  Довгі рядки метаданих автоматично прокручуються справа наліво.
  Налаштовується через відступ пробілів `scroll_gap`.
  Короткі рядки чітко фіксуються по центру екрана.
- **Розумний Caps Lock**:
  Назви треків автоматично переводяться у **ВЕЛИКІ ЛІТЕРИ** (`A-Z`).
  Гарантує ідеальну візуальну уніфікованість на сітці дисплея.
  Системні помилки зберігають свій оригінальний малий регістр.
- **Оптимізація пам'яті шрифтів (CGRAM)**:
  Заповнює слоти пам'яті динамічно залежно від локалізації.
  Замінює стандартні літери вбудованими залізничними аналогами.
  Звільняє місце під якісні кастомні гліфи для **Л**, **Ф**, **Ю**.
  Перенаправляє літеру **П** на вбудований символ `反` (`0xF7`).
- **Захист флеш-пам'яті від зносу**:
  Запроваджує жорстку перевірку безпеки шляхів через RAM (`/tmp/`).
  Блокує виклики `fopen` на внутрішній флеш-пам'яті (ROM).
  Запобігає деградації заліза та продовжує вік роутера.
- **Ізольовані банери помилок**:
  Кожен рядок дисплея має автономний моніторинг.
  Зламані шляхи в UCI вмикають незалежний бігучий рядок помилки.
  При цьому інші справні рядки продовжують працювати в штатному режимі.
- **Сучасний демон Procd**:
  Керується безпосередньо головним менеджером ініціалізації OpenWrt.
  Включає автоматичне відновлення роботи (respawn) при падінні.
  Виконує чисте закриття пристрою та очищення екрана при зупинці.
- **Динамічна шкала прогресу**:
  Графічний індикатор гучності з 5 кастомними стилями.
  Довжина шкали автоматично підлаштовується під кількість колонок екрана.

---

## 📂 Структура каталогів проєкту

Функціональні модулі вихідного коду в папці `src`:

- `src/display.c` — зчитувач Libuci, логіка I2C, розподільник CGRAM.
- `src/lcd_print_cyr.c` — багаторежимний транслятор кирилиці з UTF-8.
- `src/main.c` — головний цикл, тригери `stat()`, контролери скролінгу.
- `src/Makefile` — скрипт лінкування з прапорцем лінкера `-luci`.

---

## 🛠 Системна конфігурація UCI (`/etc/config/greenwave-lcd-i2c`)

Профіль налаштувань має міститися в анонімному блоці:

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

### Специфікація параметрів:

- `i2c_dev` — індекс головного контролера шини I2C в Linux.
- `lcd_adres` — шістнадцяткова адреса чіпа PCF8574 (зазвичай `0x27`).
- `lcd_cols` — фізична ширина екрана (кількість символів у рядку).
- `lcd_rows` — кількість доступних рядків на склі дисплея.
- `lcd_backlight` — вимикач LED підсвічування (`1` — увімкнено, `0` — вимкнено).
- `tmp_file` — шлях до файлу з текстом треку. **Має бути строго в `/tmp/`**.
- `vol_file` — шлях до файлу зі значенням гучності (0–100%). **Має бути строго в `/tmp/`**.
- `language` — селектор локалізації та апаратної матриці ROM дисплея:
  - **`en_en`** — Стандартні західні прошивки. Виділяє 8 кастомних слотів під **Б, Г/Ґ, Д, Ж, И, Л, Ф, Ю**.
  - **`en_ua`** / **`en_ru`** — Для дисплеїв з вбудованою кирилицею в ROM (HD44780 A02). Вимикає завантаження CGRAM для економії шини I2C. Напряму мапить Unicode на залізні коди кирилиці. Завжди примусово переводить кирилицю у великий регістр.
- `scroll_gap` — розмір відступу пробілами між циклами прокрутки бігучого рядка.
- `vol_style` — ідентифікатор графічного стилю шкали гучності (від 1 до 5).

---

## 📊 Візуальні теми індикатора гучності (`vol_style`)

- **`1`** — Стиль важких блоків: заповнені сегменти малюються через `██`.
- **`2`** — Стиль тонкої матриці: активні блоки — це лінії `||`, порожні — крапки `.`.
- **`3`** — Сучасний повзунок плеєра: тонка лінія треку `-` з одинарним блоком-головкою `■`.
- **`4`** — Сітка аудіоресивера: товсті подвійні рейки `=`, що показують рівень.
- **`5`** — Ретро стрілка: лінія треку `-` з одинарним вказівником-стрілкою `>`.

---

## 💻 Компіляція всередині OpenWrt SDK

Помістіть файл визначення пакета в каталог `package/greenwave-lcd-i2c/Makefile`.

### Компіляція бінарника:
Виконайте цю команду всередині головної кореневої папки вашого OpenWrt SDK:
```bash
make package/greenwave-lcd-i2c/compile V=s
```

---

## ⚙️ Керування службою через Демон (Procd)

Процес повністю контролюється рідною підсистемою ініціалізації (`init.d`).

```bash
# Запустити драйвер дисплея
/etc/init.d/greenwave-lcd-i2c start

# Зупинити демон (очищає матрицю екрана та вимикає текст)
/etc/init.d/greenwave-lcd-i2c stop

# Перезапустити сервіс
/etc/init.d/greenwave-lcd-i2c restart
```

---

## 📈 Міжпроцесна взаємодія в реальному часі

Демон оптимізовано за допомогою швидких системних тригерів `stat()`.
Читання файлів виконується *виключно* тоді, коли плеєр реально оновив буфер даних.
Зовнішні скрипти або плеєри стрімлять дані прямо в тимчасові файли:

```bash
# Оновити перший рядок (миттєвий старт прокрутки або фіксація по центру)
echo "Радіо П'ятниця" > /tmp/play_name

# Оновити другий рядок (миттєвий перерахунок довжини та відсотків шкали)
echo "68" > /tmp/tda_volume
```

## 📄 Ліцензія
Розповсюджується на умовах відкритої ліцензії MIT.

