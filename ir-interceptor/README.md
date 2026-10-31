Select language / Оберіть мову / Выберите язык:
- [English](#english)
- [Українська](#українська)
- [Русский](#русский)

---

## English

# Input Interceptor for OpenWrt

A lightweight, high-performance input event daemon for OpenWrt optimized for
embedded systems. It intercepts low-level Linux `evdev` events from infrared
(IR) remotes, rotary encoders, keyboards, and GPIO keys, matching them against
a configuration file and executing corresponding system commands asynchronously.

### Features
* **Zero Flash Wear:** Configuration and all mapping rules are parsed into
  RAM once at startup or on-demand via a `SIGHUP` signal.
* **Procd Integration:** Full compatibility with the modern OpenWrt process
  manager (`procd`), providing fail-safe automatic respawning and non-blocking
  runtime reloads.
* **Smart IR Channels:** Features a dynamic buffer accumulation system for
  multidigit channel numbers (e.g., typing `1`, `1`, `1` executes channel `111`
  command after a custom timeout).
* **Multi-Remote Support:** Handles identical markers assigned to different
  physical hardware codes (e.g., combining multiple IR controllers).
* **Advanced Debouncing:** Separated press and hold debounce intervals to
  mitigate button contact chatter and encoder noise.

### Directory Structure
```text
ir-interceptor/
├── Makefile                        # OpenWrt Package Build Recipe
├── files/
│   ├── ir-interceptor.conf         # Global Configuration File
│   └── ir-interceptor.init         # Procd Init Script
└── src/
    ├── Makefile                    # Internal Source Compilation Recipe
    ├── main.c                      # Core Dispatcher & Event Loop
    ├── signals.c / signals.h       # Linux Signal Management Module
    ├── ir_handler.c / .h           # Multi-digit IR Channel Processor
    ├── gpio_handler.c / .h         # GPIO Keys & Rotary Encoder Processor
    └── keyboard_handler.c / .h     # Standard Input & Key Processor
```

### Installation & Usage

1. Place the package directory inside your OpenWrt SDK `package/` folder.
2. Compile using OpenWrt build system:
   ```bash
   make package/ir-interceptor/compile V=s
   ```
3. Transfer and install the generated `.ipk` binary to your device via `opkg`.
4. Manage the background service using standard init system commands:
   ```bash
   /etc/init.d/ir-interceptor enable
   /etc/init.d/ir-interceptor start
   ```

### Debugging & Signals

* **Live logs view:**
  ```bash
  logread -f | grep ir-interceptor
  ```
* **Hot-reload configuration (RAM refresh):**
  ```bash
  /etc/init.d/ir-interceptor reload
  ```

---

## Українська

# Перехоплювач пристроїв введення для OpenWrt

Легковагий, високопродуктивний демон перехоплення подій підсистеми введення
Linux для OpenWrt, оптимізований для вбудованих систем. Він перехоплює
низькорівневі події `evdev` від інфрачервоних (ІК) пультів, ротарі-енкодерів,
клавіатур та GPIO кнопок, зіставляє їх із конфігураційним файлом і асинхронно
виконує відповідні системні команди.

### Особливості
* **Нульовий знос Flash-пам'яті:** Конфігураційний файл та всі правила маппінгу
  парсяться в оперативну пам'ять (ОЗУ) один раз при старті або на вимогу
  через системний сигнал `SIGHUP`.
* **Інтеграція з Procd:** Повна сумісність із сучасним менеджером процесів
  OpenWrt (`procd`), що забезпечує автоматичний перезапуск у разі падіння
  та «гаряче» оновлення без зупинки процесу.
* **Розумні ІК-канали:** Система накопичення цифрових комбінацій для
  багатозначних каналів (наприклад, послідовний набір `1`, `1`, `1` виконає
  команду для каналу `111` після завершення таймауту).
* **Мультипультовий режим:** Можливість призначення однакових маркерів на
  різні фізичні коди (наприклад, для одночасної підтримки кількох різних
  ІК-пультів).
* **Просунутий Дебаунс:** Розділений захист від брязкіту контактів для
  одиночного натискання та утримання, а також швидкий дебаунс для енкодерів.

### Інсталяція та Запуск

1. Скопіюйте директорію пакета до папки `package/` вашого OpenWrt SDK.
2. Запустіть компіляцію:
   ```bash
   make package/ir-interceptor/compile V=s
   ```
3. Перенесіть зібраний `.ipk` пакет на роутер та встановіть його через `opkg`.
4. Керуйте службою за допомогою стандартних команд:
   ```bash
   /etc/init.d/ir-interceptor enable
   /etc/init.d/ir-interceptor start
   ```

### Налагодження та Сигнали

* **Перегляд логів у реальному часі:**
  ```bash
  logread -f | grep ir-interceptor
  ```
* **Гаряче перезавантаження конфігурації в ОЗУ:**
  ```bash
  /etc/init.d/ir-interceptor reload
  ```

---

## Русский

# Перехватчик устройств ввода для OpenWrt

Легковесный, высокопроизводительный демон перехвата событий подсистемы ввода
Linux для OpenWrt, оптимизированный для встраиваемых систем. Он перехватывает
низкоуровневые события `evdev` от инфракрасных (ИК) пультов, ротационных
энкодеров, клавиатур и GPIO кнопок, сопоставляет их с конфигурационным
файлом и асинхронно выполняет соответствующие системные команды.

### Особенности
* **Нулевой износ Flash-памяти:** Конфигурационный файл и все правила маппинга
  парсятся в оперативную память (ОЗУ) один раз при старте или по требованию
  через системный сигнал `SIGHUP`.
* **Интеграция с Procd:** Полная совместимость с современным менеджером
  процессов OpenWrt (`procd`), обеспечивающая автоматический перезапуск в
  случае падения и «горячее» обновление без остановки процесса.
* **Умные ИК-каналы:** Система динамического накопления цифровых комбинаций
  для многозначных каналов (например, последовательный набор цифр `1`, `1`, `1`
  выполнит команду для канала `111` по истечении таймаута).
* **Мультипультовый режим:** Возможность назначения одинаковых маркеров на
  разные физические коды (например, для одновременной поддержки нескольких
  разных ИК-пультов).
* **Продвинутый Дебаунс:** Раздельная защита от дребезга контактов для
  одиночного нажатия и удержания, а также быстрый дебаунс для относительных
  осей энкодеров.

### Инсталляция и Запуск

1. Скопируйте директорию пакета в папку `package/` вашего OpenWrt SDK.
2. Запустите компиляцию:
   ```bash
   make package/ir-interceptor/compile V=s
   ```
3. Перенесите собранный `.ipk` пакет на роутер и установите его через `opkg`.
4. Управляйте службой с помощью стандартных команд инициализации:
   ```bash
   /etc/init.d/ir-interceptor enable
   /etc/init.d/ir-interceptor start
   ```

### Отладка и Сигналы

* **Просмотр логов в реальном времени:**
  ```bash
  logread -f | grep ir-interceptor
  ```
* **Горячая перезагрузка конфигурации в ОЗУ:**
  ```bash
  /etc/init.d/ir-interceptor reload
  ```
