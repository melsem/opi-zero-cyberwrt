Select language / Оберіть мову / Выберите язык:
- [English](#english)
- [Українська](#українська)
- [Русский](#русский)

---

## English

### ch-buffer

A lightweight, lightning-fast input accumulator and debouncer written in C.
Specifically engineered for single-board computers (like Orange Pi,
Raspberry Pi) and routers running **OpenWrt** or Linux with **triggerhappy (thd)**.

It acts as an intermediary bridge between raw remote control signals and your
OS scripts. It queues numeric button presses within a timeout, strips leading
zeros, prevents key bounce, and effortlessly separates a standard press from
a long hold command.

#### Key Features

*   **Smart Buffering:** Collects numeric inputs within a user-defined window
    (e.g., typing `0`, `0`, `5`, `1` naturally executes as channel `51`).
*   **Modular Audio & Scan Support:** Features a dedicated, highly responsive
    routing engine for sound adjustments (`vol`, `bass`, `treble`) and
    frequency scanning.
*   **Throttled Hold Repeat:** Limits spam from `triggerhappy` by executing
    hold events (`state 2`) on audio/scan keys smoothly at set intervals
    (e.g., 500ms) via dedicated internal timers, preserving CPU cycles.
*   **Advanced Hold Execution:** Fully cancels numeric input upon a long hold
    and triggers an alternative command (via the `-l` flag), passing the
    pressed key dynamically.
*   **Isolated Profiles:** Uses independent state files in `/dev/shm/` for
    digits, audio, and scanning to entirely prevent cross-button thread conflicts.
*   **Silent by Default:** Keeps system logs clean. Output log messaging is
    strictly hidden unless explicitly requested via debug flags.

#### Command Line Flags

*   `-s [1|2]` : **(Required)** Triggerhappy input state. `1` for short click,
    `2` for hold/repeat.
*   `-c <string>` : **(Required)** Target button name passed by the key press.
    Supported values:
    *   Digits: `0` to `9`
    *   Audio: `vol+`, `vol-`, `subtrebl+`, `subtrebl-`, `subbas+`, `subbas-`,
        `bass+`, `bass-`, `treble+`, `treble-`
    *   Scan: `scan+`, `scan-`
*   `-k "<command>"` : **(Required)** Base command to execute upon a timed-out
    numeric string or an immediate short click.
*   `-l "<command>"` : Alternative command to execute on a validated long
    hold / smooth autorepeat loop.
*   `-p <ms>` : Debounce timing to prevent micro-clicks double-firing (Default: 200ms).
*   `-h <ms>` : Minimum hold threshold duration for digits (Default: 800ms).
*   `-t <ms>` : Timeout to wait for the next sequential digit (Default: 1000ms).
*   `-r` or `-?` : Verbose debug flags. Fully unmutes the terminal and prints
    detailed internal `[LOG]` diagnostics.

#### Project Structure
*   `main.c` — Entry point, CLI flag parsing, and routing.
*   `ch-buffer.c` / `.h` — Accumulation buffer logic for channels.
*   `v-buffer.c` / `.h` — Throttling engine for audio and scan keys.

#### Configuration Example (`/etc/triggerhappy/triggers.d/ir.conf`)

```text
# Digit buffering setup (with debugging logs enabled via -r)
KEY_1 1 /usr/local/bin/ch-buffer -s 1 -c 1 -k "/etc/init.d/web-radio2.init reload" -l "/usr/bin/favorites.sh" -r
KEY_1 2 /usr/local/bin/ch-buffer -s 2 -c 1 -k "/etc/init.d/web-radio2.init reload" -l "/usr/bin/favorites.sh" -r

# Volume configuration (Silent mode - default)
KEY_VOLUMEUP 1 /usr/local/bin/ch-buffer -s 1 -c "vol+" -k "amixer set Master 5%+"
KEY_VOLUMEUP 2 /usr/local/bin/ch-buffer -s 2 -c "vol+" -l "amixer set Master 5%+"
```

---

## Українська

### ch-buffer

Легковажний, блискавичний накопичувач цифрового введення та дебаунсер на C.
Створений спеціально для одноплатних комп'ютерів (Orange Pi, Raspberry Pi)
та роутерів під керуванням **OpenWrt** або Linux, які використовують демон
**triggerhappy (thd)**.

Він виступає містком між сигналами пульта та скриптами ОС: збирає цифри за
певний час, прибирає нулі попереду, захищає від брязкоту контактів і
залізобетонно розділяє короткий клік від тривалого утримання кнопки.

#### Основні можливості

*   **Розумне накопичення:** Збирає цифри, натиснуті протягом таймауту
    (наприклад, введення `0`, `0`, `5`, `1` перетвориться на канал `51`).
*   **Модульна підтримка Audio & Scan:** Має окремий швидкий рушій обробки
    для кнопок звуку (`vol`, `bass`, `treble`) та сканування частот.
*   **Керований автоповтор (Дроселювання):** Обмежує спам від `triggerhappy`.
    При затисканні аудіо/скан кнопок події виконуються плавно з фіксованою
    паузою (наприклад, 500 мс) через власні таймери, не навантажуючи процесор.
*   **Обробка утримання:** Повністю скасовує вибір каналу при довгому затисканні
    цифри та запускає альтернативну команду (через прапорець `-l`), підставляючи
    номер кнопки.
*   **Ізоляція потоків:** Використовує три незалежні файли часу в `/dev/shm/`
    для цифр, звуку та сканування, що повністю виключає будь-які конфлікти.
*   **Тихий режим за замовчуванням:** Не засмічує системні логи пристрою.
    Виведення повідомлень повністю вимкнено, поки воно не буде активовано.

#### Прапорці командного рядка

*   `-s [1|2]` : **(Обов'язковий)** Стан кнопки від Triggerhappy. `1` — короткий
    клік, `2` — утримання/автоповтор.
*   `-c <рядок>` : **(Обов'язковий)** Ім'я кнопки, що обробляється:
    *   Цифри: від `0` до `9`
    *   Звук: `vol+`, `vol-`, `subtrebl+`, `subtrebl-`, `subbas+`, `subbas-`,
        `bass+`, `bass-`, `treble+`, `treble-`
    *   Сканування: `scan+`, `scan-`
*   `-k "<команда>"` : **(Обов'язковий)** Базова команда, яка виконається після
    таймауту або миттєво для швидких кнопок.
*   `-l "<команда>"` : Альтернативна команда для тривалого утримання або крок
    плавного циклу автоповтору.
*   `-p <мс>` : Захист від брязкоту контактів (Дефолт: 200 мс).
*   `-h <мс>` : Поріг часу для детекції тривалого утримання цифрових кнопок
    (Дефолт: 800 мс).
*   `-t <мс>` : Таймаут очікування наступної цифри каналу (Дефолт: 1000 мс).
*   `-r` або `-?` : Прапорці налагодження. Вмикають виведення детальних
    діагностичних повідомлень `[LOG]` у термінал.

#### Структура проекту
*   `main.c` — Точка входу, парсинг аргументів та маршрутизація.
*   `ch-buffer.c` / `.h` — Накопичувальний буфер цифрових каналів.
*   `v-buffer.c` / `.h` — Рушій обробки та затримок для Audio та Scan.

#### Приклад конфігурації (`/etc/triggerhappy/triggers.d/ir.conf`)

```text
# Набір цифр (Режим налагодження увімкнено через -r)
KEY_1 1 /usr/local/bin/ch-buffer -s 1 -c 1 -k "/etc/init.d/web-radio2.init reload" -l "/usr/bin/favorites.sh" -r
KEY_1 2 /usr/local/bin/ch-buffer -s 2 -c 1 -k "/etc/init.d/web-radio2.init reload" -l "/usr/bin/favorites.sh" -r

# Регулювання гучності (Тихий режим за замовчуванням)
KEY_VOLUMEUP 1 /usr/local/bin/ch-buffer -s 1 -c "vol+" -k "amixer set Master 5%+"
KEY_VOLUMEUP 2 /usr/local/bin/ch-buffer -s 2 -c "vol+" -l "amixer set Master 5%+"
```

---

## Русский

### ch-buffer

Легковесный, молниеносный накопитель цифрового ввода и дебаунсер на C.
Специально разработан для одноплатных компьютеров (Orange Pi, Raspberry Pi)
и роутеров под управлением **OpenWrt** or Linux, использующих демон
**triggerhappy (thd)**.

Программа служит умным буфером между ИК-пультом и системными скриптами:
собирает последовательно нажатые цифры, отсекает незначащие нули,
предотвращает дребезг контактов и аппаратно разделяет короткое нажатие
кнопки от её длительного удержания.

#### Ключевые особенности

*   **Умное накопление:** Объединяет цифры, введенные в рамках таймаута
    (например, нажатие `0`, `0`, `5`, `1` выполнится как канал `51`).
*   **Модульная поддержка Audio & Scan:** Содержит выделенный быстрый движок
    обработки для аудио-параметров (`vol`, `bass`, `treble`) и сканирования частот.
*   **Дросселирование автоповтора:** Ограничивает лавинообразный спам от
    `triggerhappy`. При зажатии аудио/скан кнопок команды выполняются плавно с
    фиксированной паузой (например, 500 мс) через независимые таймеры.
*   **Логика удержания:** Полностью отменяет стандартную команду при зажатии
    цифры и мгновенно выполняет альтернативный скрипт (флаг `-l`), автоматически
    передавая номер кнопки.
*   **Раздельные профили:** Использует независимые файлы времени в памяти
    `/dev/shm/` для цифр, звука и сканирования, полностью исключая конфликты.
*   **Тихий режим по умолчанию:** Не засоряет системные логи. Вывод
    диагностических сообщений строго скрыт, пока не активирован принудительно.

#### Флаги командной строки

*   `-s [1|2]` : **(Обязательный)** Состояние кнопки от Triggerhappy. `1` — клик,
    `2` — удержание/автоповтор.
*   `-c <строка>` : **(Обязательный)** Имя обрабатываемой кнопки:
    *   Цифры: от `0` до `9`
    *   Звук: `vol+`, `vol-`, `subtrebl+`, `subtrebl-`, `subbas+`, `subbas-`,
        `bass+`, `bass-`, `treble+`, `treble-`
    *   Сканирование: `scan+`, `scan-`
*   `-k "<команда>"` : **(Обязательный)** Базовая команда для переключения
    каналов по таймауту или мгновенного клика.
*   `-l "<команда>"` : Альтернативная команда, вызываемая при удержании
    цифры или шаг плавного автоповтора.
*   `-p <мс>` : Защита от дребезга контактов (По умолчанию: 200 мс).
*   `-h <мс>` : Время зажатия для детекции удержания цифровых кнопок (По умолчанию: 800 мс).
*   `-t <мс>` : Таймаут ожидания следующей цифры (По умолчанию: 1000 мс).
*   `-r` или `-?` : Флаги отладки. Полностью включают вывод подробных
    диагностических сообщений `[LOG]` в консоль.

#### Структура проекта
*   `main.c` — Точка входа, парсинг аргументов и маршрутизация.
*   `ch-buffer.c` / `.h` — Накопительный буфер цифровых каналов.
*   `v-buffer.c` / `.h` — Движок обработки и задержек для Audio и Scan.

#### Пример настройки (`/etc/triggerhappy/triggers.d/ir.conf`)

```text
# Набор цифр (Режим отладки включен через флаг -r)
KEY_1 1 /usr/local/bin/ch-buffer -s 1 -c 1 -k "/etc/init.d/web-radio2.init reload" -l "/usr/bin/favorites.sh" -r
KEY_1 2 /usr/local/bin/ch-buffer -s 2 -c 1 -k "/etc/init.d/web-radio2.init reload" -l "/usr/bin/favorites.sh" -r

# Регулировка громкости (Тихий режим по умолчанию)
KEY_VOLUMEUP 1 /usr/local/bin/ch-buffer -s 1 -c "vol+" -k "amixer set Master 5%+"
KEY_VOLUMEUP 2 /usr/local/bin/ch-buffer -s 2 -c "vol+" -l "amixer set Master 5%+"
```
