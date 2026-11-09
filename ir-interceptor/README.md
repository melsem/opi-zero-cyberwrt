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
*   **Advanced Hold Execution:** Fully cancels numeric input upon a long hold
    and triggers an alternative command (via the `-l` flag), passing the
    pressed key number dynamically.
*   **Persistent File Descriptors Strategy:** Bypasses aggressive `tmpfs` RAM
    cleanup cycles on embedded Linux kernels by holding state file
    descriptors open during `usleep()`.
*   **Zero Overhead:** Consumes negligible memory, running smoothly even on
    tight hardware profiles.

#### Command Line Flags

*   `-s [1|2]` : **(Required)** Triggerhappy input state. `1` for short click, `2` for hold/repeat.
*   `-c [0-9]` : **(Required)** Digit passed by the key press.
*   `-k "<command>"` : **(Required)** Base command to execute upon a timed-out
    numeric string.
*   `-l "<command>"` : Alternative command to execute on a validated long hold.
*   `-p <ms>` : Debounce timing to prevent micro-clicks double-firing (Default: 200ms).
*   `-h <ms>` : Minimum hold threshold duration (Default: 800ms).
*   `-t <ms>` : Timeout to wait for the next sequential digit (Default: 1000ms).

#### Configuration Example (`/etc/triggerhappy/triggers.d/ir.conf`)

```text
KEY_1 1 /usr/local/bin/ch-buffer -s 1 -c 1 -k "/etc/init.d/web-radio2.init reload" -l "/usr/bin/favorites.sh"
KEY_1 2 /usr/local/bin/ch-buffer -s 2 -c 1 -k "/etc/init.d/web-radio2.init reload" -l "/usr/bin/favorites.sh"
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
*   **Обробка утримання:** Повністю скасовує вибір каналу при довгому
    затисканні та миттєво запускає альтернативну команду (через прапорець
    `-l`), підставляючи номер кнопки в кінець.
*   **Стратегія відкритих дескрипторів:** Утримує файли в пам'яті відкритими
    під час сну `usleep()`, що повністю захищає їх від агресивного видалення
    утилітами очищення `tmpfs` у `/dev/shm` на слабких ядрах.
*   **Максимальна швидкість:** Бінарник оптимізований, не створює зайвих
    форків процесів та працює на будь-якому «залізі».

#### Прапорці командного рядка

*   `-s [1|2]` : **(Обов'язковий)** Стан кнопки від Triggerhappy.
    `1` — клік, `2` — утримання/автоповтор.
*   `-c [0-9]` : **(Обов'язковий)** Цифра, яка відповідає натиснутій кнопці.
*   `-k "<команда>"` : **(Обов'язковий)** Базова команда, яка виконається з
    номером каналу після таймауту.
*   `-l "<команда>"` : Альтернативна команда для тривалого утримання.
*   `-p <мс>` : Захист від брязкоту контактів пульта (Дефолт: 200 мс).
*   `-h <мс>` : Поріг часу для детекції тривалого утримання (Дефолт: 800 мс).
*   `-t <мс>` : Таймаут очікування наступної цифри каналу (Дефолт: 1000 мс).

#### Приклад конфігурації (`/etc/triggerhappy/triggers.d/ir.conf`)

```text
KEY_1 1 /usr/local/bin/ch-buffer -s 1 -c 1 -k "/etc/init.d/web-radio2.init reload" -l "/usr/bin/favorites.sh"
KEY_1 2 /usr/local/bin/ch-buffer -s 2 -c 1 -k "/etc/init.d/web-radio2.init reload" -l "/usr/bin/favorites.sh"
```

---

## Русский

### ch-buffer

Легковесный, молниеносный накопитель цифрового ввода и дебаунсер на C.
Специально разработан для одноплатных компьютеров (Orange Pi, Raspberry Pi)
и роутеров под управлением **OpenWrt** или Linux, использующих демон
**triggerhappy (thd)**.

Программа служит умным буфером между ИК-пультом и системными скриптами:
собирает последовательно нажатые цифры, отсекает незначащие нули,
предотвращает дребезг контактов и аппаратно разделяет короткое нажатие
кнопки от её длительного удержания.

#### Ключевые особенности

*   **Умное накопление:** Объединяет цифры, введенные в рамках таймаута
    (например, нажатие `0`, `0`, `5`, `1` выполнится как канал `51`).
*   **Логика удержания:** Полностью отменяет стандартную команду при зажатии
    кнопки и мгновенно выполняет альтернативный скрипт (флаг `-l`),
    автоматически передавая номер зажатой кнопки.
*   **Иммунитет к очистке tmpfs:** Держит дескрипторы файлов состояния
    открытыми в памяти во время `usleep()`, предотвращая их уничтожение
    агрессивными системными сборщиками мусора в `/dev/shm`.
*   **Высокая энергоэффективность:** Нативный бинарный код выполняется
    моментально, не нагружая процессор и RAM слабого устройства.

#### Флаги командной строки

*   `-s [1|2]` : **(Обязательный)** Состояние кнопки от Triggerhappy.
    `1` — клик, `2` — удержание/автоповтор.
*   `-c [0-9]` : **(Обязательный)** Цифра, соответствующая нажатой клавише.
*   `-k "<команда>"` : **(Обязательный)** Базовая команда для переключения
    каналов по таймауту.
*   `-l "<команда>"` : Альтернативная команда, вызываемая при удержании
    кнопки.
*   `-p <мс>` : Защита от дребезга контактов пульта (По умолчанию: 200 мс).
*   `-h <мс>` : Минимальное время зажатия для детекции удержания
    (По умолчанию: 800 мс).
*   `-t <мс>` : Таймаут ожидания следующей цифры (По умолчанию: 1000 мс).

#### Пример настройки (`/etc/triggerhappy/triggers.d/ir.conf`)

```text
KEY_1 1 /usr/local/bin/ch-buffer -s 1 -c 1 -k "/etc/init.d/web-radio2.init reload" -l "/usr/bin/favorites.sh"
KEY_1 2 /usr/local/bin/ch-buffer -s 2 -c 1 -k "/etc/init.d/web-radio2.init reload" -l "/usr/bin/favorites.sh"
```

