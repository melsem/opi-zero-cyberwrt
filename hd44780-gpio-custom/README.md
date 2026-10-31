# Universal HD44780 LCD & Thermostat Driver for OpenWrt

---

## 🌐 Language Selection / Вибір мови / Выбор языка

*   [English Documentation](#-english-documentation)
*   [Українська документація](#-українська-документація)
*   [Русская документация](#-русская-документация)

---

## 🇬🇧 English Documentation

### 📝 Project Description
A high-performance, monolithic Linux Kernel Module (`hd44780.ko`) and Web-CGI management ecosystem designed for OpenWrt devices (optimized for Orange Pi Zero). The driver acts as a "black box" for User-Space applications, allowing them to output text and control custom characters via a unified interface `/dev/hd44780.X` using standard file operations or `ioctl`.

The driver completely isolates the hardware layer and supports three modes of operation on the fly:
1.  **High-Speed Native GPIO:** Direct parallel 4-bit interface connection using 7 discrete GPIO pins.
2.  **Built-in Hardware I2C:** Connection via PCF8574 expander using the SoC's hardware I2C controllers (`i2c-0`, `i2c-1`, etc.).
3.  **Virtual Software I2C (`i2c-gpio`):** Dynamic compilation and registration of a software I2C bus inside the kernel on any free GPIO pins, without requiring third-party drivers.

### 🧱 Architecture & Project Structure
*   `hd44780-gpio-custom.c` — Main kernel module entry point. Handles `insmod` parameters, parses arguments, and routes initialization logic.
*   `hd44780-lcd-ops.c` / `.h` — Pure parallel 4-bit Native GPIO driver implementation, hardware timing loops (`udelay`), and `miscdevice` file operations.
*   `hd44780-lcd-i2c.c` / `.h` — I2C layer implementation. Handles dynamic `platform_device` allocation for virtual sub-buses and registers `pcf8574` clients.
*   `i2c_HD44780.init` — OpenWrt init script managing daemon initialization and 4-channel independent DS18B20 thermostat automation.
*   `Setup.cgi` / `Save.cgi` — Secure Web-GUI configuration subsystem that prevents Quirks Mode and layout breakages.

### ⚙️ UCI Configuration (`/etc/config/hd44780-gpio-custom`)
```config
config display 'pins'
	option lcd_mode_dev 'LCD_MODE_I2C'   # 'LCD_MODE_I2C' or 'LCD_MODE_GPIO'
	option i2c_bus_type 'builtin'        # 'builtin' (hardware) or 'virtual' (software bitbang)
	option dev '0'                       # Device index/bus ID (/dev/hd44780.0)
	option address '0x27'                # PCF8574 Hex I2C Address (0x27 or 0x3F)
	option sda_gpio '20'                 # Custom SDA Pin (Only for 'virtual' mode)
	option scl_gpio '21'                 # Custom SCL Pin (Only for 'virtual' mode)
	option hd_rs '0'                     # Native GPIO Pin Mapping (Only for 'LCD_MODE_GPIO')
	option hd_rw '1'
	option hd_e '2'
	option hd_db4 '3'
	option hd_db5 '15'
	option hd_db6 '16'
	option hd_db7 '14'
```

### 🛠 Compiling within OpenWrt Buildroot
Place the source directory into `package/kernel/hd44780-gpio-custom/src/`. Use the following standard multi-object `Makefile`:
```makefile
obj-m += hd44780.o
hd44780-y := hd44780-gpio-custom.o hd44780-lcd-ops.o hd44780-lcd-i2c.o
ccflags-y += -I\$(LINUX_DIR)/drivers/gpio
```
Run compilation from your Buildroot directory:
```bash
make package/kernel/hd44780-gpio-custom/compile V=s
```

### 🚀 Execution Examples
**Hardware Built-in I2C Mode:**
```bash
insmod hd44780 dev_mode="LCD_MODE_I2C" i2c_bus_type="builtin" dev=0 address=0x27
```
**Virtual Software Bitbang I2C Mode:**
```bash
insmod hd44780 dev_mode="LCD_MODE_I2C" i2c_bus_type="virtual" dev=3 address=0x27 sda_gpio=15 scl_gpio=16
```
**High-Speed Native GPIO Mode:**
```bash
insmod hd44780 dev_mode="LCD_MODE_GPIO" bus0=0,0,1,2,3,15,16,14
```


#### 🧭 User-Space API Guide
*   The application communicates with the hardware abstraction layer via a unified device node `/dev/hd44780.X`.
*   The program opens the file node using a standard `open()` call with read/write access.
*   Once the descriptor is obtained, the process transmits raw commands, text, or custom graphics via `ioctl`.
*   The architecture guarantees complete hardware-independent execution for the user-space binary.

#### 📡 Communication Macros & Functions
*   **IOCTL Magic Protocol:** 4 core commands shared between kernel headers and the binary code.

```c
/* Standard HD44780 commands. IOCTL configuration for kernel driver communication */
#define LCD_IOC_MAGIC 'k'
#define LCD_IOC_WRITE_CMD   _IOW(LCD_IOC_MAGIC, 1, unsigned char)
#define LCD_IOC_WRITE_DATA  _IOW(LCD_IOC_MAGIC, 2, unsigned char)
#define LCD_IOC_SET_CGRAM   _IOW(LCD_IOC_MAGIC, 3, unsigned char[8])
#define LCD_IOC_READ_DATA   _IOR(LCD_IOC_MAGIC, 4, unsigned char)
```

*   **Low-Level API Routines:** Functions invoked by the daemon to feed directives and characters to the LCD.

```c
void write_CMD(char c) {
	unsigned char cmd = (unsigned char)c;
	
	// Invoke ioctl with WRITE_CMD code
	if (ioctl(file, LCD_IOC_WRITE_CMD, &cmd) < 0) {
		fprintf(stderr, "IOCTL CMD Error: %s\n", strerror(errno));
		errFl = 1;
		sprintf(temp_gpOut, "IOCTL CMD Error: %s", strerror(errno));
		error_log(temp_gpOut);
	}
}

// Sending raw character data for the display (text strings)
void write_dat(char c) {
	unsigned char data = (unsigned char)c;
	
	// Invoke ioctl with WRITE_DATA code
	if (ioctl(file, LCD_IOC_WRITE_DATA, &data) < 0) {
		fprintf(stderr, "IOCTL DATA Error: %s\n", strerror(errno));
		errFl = 1;
		sprintf(temp_gpOut, "IOCTL DATA Error: %s", strerror(errno));
		error_log(temp_gpOut);
	}
}
```

#### 🛠 Target Routing inside `main()`
*   The binary dynamically shapes the destination file node path inside the core loop.
*   This mechanism enables independent execution for both built-in buses, software sub-buses, and parallel lines.
*   The setup is strictly secured against any memory overflows via boundary tracking.

```c
int main(int argc, char** argv) {
	// ... code ...

	// Buffer overflow protection for setlcd_patch (maximum 20 bytes)
	snprintf(setlcd_patch, sizeof(setlcd_patch), "%s", setlcd);

	// Dynamically assemble the target LCD_DEV file node path
	if (arg_dev_bit == 1) sprintf(dev_path, "%s%s", LCD_DEV, arg_dev_num);
	else sprintf(dev_path, "%s0", LCD_DEV);

	if ((file = open(dev_path, O_RDWR)) < 0) {
		errFl = 1;
		fprintf(stderr,"Open GPIO_Dev Error: %s (%d)\n", strerror(errno), errno);
		sprintf(temp_gpOut,"Open GPIO_Dev Error: %s (%d)", strerror(errno), errno);
		error_log(temp_gpOut);
	}

	// ... code ...
}
```

### Advanced Hardware Scalability

> [!NOTE]
> By integrating the `i2c-gpio` layer via native `gpiod` kernel APIs, \
> the generated virtual bus acts as a fully compliant, standard \
> **System I2C Controller**.

#### 🎛️ Shared Resource Mapping
*   **GPIO Lockdown:** Selected SDA/SCL pins are exclusively reserved \
    by the kernel. Direct bit-banging via `/sys/class/gpio/` is blocked \
    to prevent signals collisions.
*   **Bus Multitasking:** The file node `/dev/i2c-X` remains globally open. \
    Other user-space daemons or kernel drivers can read/write data concurrently.
*   **Topology:** Multiple target chips can share the exact same physical lines, \
    provided they utilize unique slave addresses.

#### 📈 Expansion Matrix (Future-Proofing)

| Target Hardware | Communication Protocol | System Role | Dynamic Behavior |
| :--- | :--- | :--- | :--- |
| **DS3231 / DS1307** | I2C (Shared lines) | Real-Time Clock | Retains precise uptime during hard \
reboots without internet |

| **Extra PCF8574** | I2C (Shared lines) | Relay IO Expander | Scales automation array from 4 to 8, \
12, or 16 independent channels |

| **BME280 / Si7021** | I2C (Shared lines) | Climate Telemetry | Captures ambient humidity, pressure, \
and thermal metrics |

```text
[SoC Hardware] ──> [Virtual i2c-gpio Bus Driver]
                         │
                         ├──> Address 0x27 ──> [HD44780 LCD (Via hd44780.ko)]
                         ├──> Address 0x68 ──> [DS3231 RTC Clock]
                         └──> Address 0x20 ──> [Secondary Relay Array]
```

[Go to Top ⬆️](#-language-selection--вибір-мови--выбор-языка)

## 🇺🇦 Українська документація

### 📝 Опис проєкту
Високопродуктивний монолітний модуль ядра Linux (`hd44780.ko`) та екосистема вебкарт керування CGI, розроблена 
для пристроїв OpenWrt (оптимізована під Orange Pi Zero). Драйвер виступає в ролі «чорної скриньки» для програм 
користувача (User-Space), дозволяючи їм виводити текст та завантажувати власні символи через 
уніфікований інтерфейс `/dev/hd44780.X` за допомогою стандартних файлових операцій або `ioctl`.

Драйвер повністю ізолює апаратний рівень і підтримує на льоту три режими роботи:
1.  **High-Speed Native GPIO:** Пряме паралельне 4-бітне підключення інтерфейсу з використанням 7 дискретних пінів GPIO.
2.  **Built-in Hardware I2C:** Підключення через розширювач PCF8574 з використанням апаратних I2C контролерів процесора (`i2c-0`, `i2c-1` тощо).
3.  **Virtual Software I2C (`i2c-gpio`):** Динамічна компіляція та реєстрація програмної шини I2C всередині ядра на будь-яких вільних GPIO ніжках, без необхідності залучення сторонніх драйверів.

### 🧱 Архітектура та структура проєкту
*   `hd44780-gpio-custom.c` — Головна точка входу модуля ядра. Обробляє параметри `insmod`, парсить аргументи та розводить логіку ініціалізації.
*   `hd44780-lcd-ops.c` / `.h` — Реалізація чистого паралельного 4-бітного драйвера Native GPIO, апаратні цикли затримок (`udelay`) та операції з файлами `miscdevice`.
*   `hd44780-lcd-i2c.c` / `.h` — Реалізація I2C рівня драйвера. Виконує динамічне виділення `platform_device` для віртуальних субшин та реєструє клієнтів `pcf8574`.
*   `i2c_HD44780.init` — Системний скрипт ініціалізації OpenWrt, що керує запуском демонів автоматизації та 4-канальним незалежним терморегулятором на базі DS18B20.
*   `Setup.cgi` / `Save.cgi` — Безпечна підсистема веб-інтерфейсу налаштування конфігурації, захищена від Quirks Mode та збоїв розмітки сторінки.

### ⚙️ Конфігурація UCI (`/etc/config/hd44780-gpio-custom`)
```config
config display 'pins'
	option lcd_mode_dev 'LCD_MODE_I2C'   # 'LCD_MODE_I2C' або 'LCD_MODE_GPIO'
	option i2c_bus_type 'builtin'        # 'builtin' (апаратний) або 'virtual' (віртуальний i2c-gpio)
	option dev '0'                       # Номер пристрою (/dev/hd44780.0) або шини i2c-0
	option address '0x27'                # Шістнадцяткова адреса PCF8574 (0x27 або 0x3F)
	option sda_gpio '20'                 # Пін SDA (Тільки для режиму 'virtual')
	option scl_gpio '21'                 # Пін SCL (Тільки для режиму 'virtual')
	option hd_rs '0'                     # Розпиновка для Native GPIO (Тільки для 'LCD_MODE_GPIO')
	option hd_rw '1'
	option hd_e '2'
	option hd_db4 '3'
	option hd_db5 '15'
	option hd_db6 '16'
	option hd_db7 '14'
```

### 🛠 Компіляція всередині OpenWrt Buildroot
Помістіть папку з вихідним кодом у директорію `package/kernel/hd44780-gpio-custom/src/`. Використовуйте наступний стандартний мультиоб'єктний `Makefile`:
```makefile
obj-m += hd44780.o
hd44780-y := hd44780-gpio-custom.o hd44780-lcd-ops.o hd44780-lcd-i2c.o
ccflags-y += -I\$(LINUX_DIR)/drivers/gpio
```
Запустіть компіляцію з колінь вашого Buildroot:
```bash
make package/kernel/hd44780-gpio-custom/compile V=s
```

### 🚀 Приклади запуску
**Режим апаратного вбудованого I2C:**
```bash
insmod hd44780 dev_mode="LCD_MODE_I2C" i2c_bus_type="builtin" dev=0 address=0x27
```
**Режим програмного віртуального I2C (Bitbang):**
```bash
insmod hd44780 dev_mode="LCD_MODE_I2C" i2c_bus_type="virtual" dev=3 address=0x27 sda_gpio=15 scl_gpio=16
```
**Режим швидкісного Native GPIO:**
```bash
insmod hd44780 dev_mode="LCD_MODE_GPIO" bus0=0,0,1,2,3,15,16,14
```


#### 🧭 Принцип роботи
*   Користувацький бінарник взаємодіє з апаратним рівнем драйвера через файл символьного пристрою `/dev/hd44780.X`.
*   Програма відкриває цей файл за допомогою стандартного системного виклику `open()` із правами на читання та запис.
*   Після отримання дескриптора файлу процес передає прямі інструкції, текст або завантажує графіку через `ioctl`.
*   Архітектура гарантує повну незалежність виконання користувацького коду від фізичного типу підключення екрана.

#### 📡 Команди зв'язку та функції API
*   **Протокол IOCTL команд:** 4 основні команди, які синхронізовані між заголовками ядра та кодом програми.

```c
/* Стандартні команди HD44780. Налаштування IOCTL команд для зв'язку з драйвером ядра */
#define LCD_IOC_MAGIC 'k'
#define LCD_IOC_WRITE_CMD   _IOW(LCD_IOC_MAGIC, 1, unsigned char)
#define LCD_IOC_WRITE_DATA  _IOW(LCD_IOC_MAGIC, 2, unsigned char)
#define LCD_IOC_SET_CGRAM   _IOW(LCD_IOC_MAGIC, 3, unsigned char[8])
#define LCD_IOC_READ_DATA   _IOR(LCD_IOC_MAGIC, 4, unsigned char)
```

*   **Функції відправки даних:** Стандартні функції процесу для надсилання конфігураційних інструкцій та текстових символів.

```c
void write_CMD(char c) {
	unsigned char cmd = (unsigned char)c;
	
	// Викликаємо ioctl з кодом WRITE_CMD
	if (ioctl(file, LCD_IOC_WRITE_CMD, &cmd) < 0) {
		fprintf(stderr, "IOCTL CMD Error: %s\n", strerror(errno));
		errFl = 1;
		sprintf(temp_gpOut, "IOCTL CMD Error: %s", strerror(errno));
		error_log(temp_gpOut);
	}
}

// Надсилання чистих даних для дисплея (символи тексту)
void write_dat(char c) {
	unsigned char data = (unsigned char)c;
	
	// Викликаємо ioctl з кодом WRITE_DATA
	if (ioctl(file, LCD_IOC_WRITE_DATA, &data) < 0) {
		fprintf(stderr, "IOCTL DATA Error: %s\n", strerror(errno));
		errFl = 1;
		sprintf(temp_gpOut, "IOCTL DATA Error: %s", strerror(errno));
		error_log(temp_gpOut);
	}
}
```

#### 🛠 Маршрутизація пристроїв у функції `main()`
*   Усередині основного життєвого циклу програми бінарник динамічно формує шлях до потрібного файлу пристрою.
*   Цей механізм дозволяє незалежно керувати як апаратними екранами, так і віртуальними шинами або паралельними лініями.
*   Код надійно захищає операційну систему від переповнення буфера завдяки суворому відстеженню меж пам'яті.

```c
int main(int argc, char** argv) {
	// ...... код ......

	// Захист від переповнення буфера setlcd_patch (максимум 20 байт)
	snprintf(setlcd_patch, sizeof(setlcd_patch), "%s", setlcd);

	// Динамічне формування шляху до файлу пристрою LCD_DEV
	if (arg_dev_bit == 1) sprintf(dev_path, "%s%s", LCD_DEV, arg_dev_num);
	else sprintf(dev_path, "%s0", LCD_DEV);

	if ((file = open(dev_path, O_RDWR)) < 0) {
		errFl = 1;
		fprintf(stderr,"Open GPIO_Dev Error: %s (%d)\n", strerror(errno), errno);
		sprintf(temp_gpOut,"Open GPIO_Dev Error: %s (%d)", strerror(errno), errno);
		error_log(temp_gpOut);
	}

	// ....... код .........
}
```

### Масштабованість та розширення заліза

> [!NOTE]
> Завдяки інтеграції рівня `i2c-gpio` через офіційні апаратні `gpiod` \
> API ядра, створена віртуальна шина працює як повноцінний, \
> стандартний **Системний I2C Контролер**.

#### 🎛️ Розподіл системних ресурсів
*   **Блокування GPIO:** Вибрані піни SDA/SCL повністю резервуються ядром. \
    Пряме керування через `/sys/class/gpio/` блокується для уникнення \
    апаратних конфліктів.
*   **Багатозадачність шини:** Вузол пристрою `/dev/i2c-X` залишається відкритим \
    для всієї системи. Інші програми користувача або драйвери можуть \
    паралельно зчитувати дані.
*   **Топологія шини:** Кілька різних чіпів можуть фізично сидіти на одних \
    і тих самих дротах, якщо вони мають унікальні шістнадцяткові адреси.

#### 📈 Матриця розширення периферії

| Цільове залізо | Протокол зв'язку | Роль у системі | Поведінка комплексу |
| :--- | :--- | :--- | :--- |
| **DS3231 / DS1307** | I2C (Спільні лінії) | Годинник RTC | Зберігає точний хід часу при повних \
ребутах без доступу до мережі |

| **Додатковий PCF8574** | I2C (Спільні лінії) | Розширювач портів | Збільшує сітку автоматизації з 4 до 8, \
12 або 16 незалежних реле |

| **BME280 / Si7021** | I2C (Спільні лінії) | Кліматичний сенсор | Зчитує вологість, атмосферний тиск \
та температуру середовища |

```text
[Процесор SoC] ──> [Драйвер віртуальної шини i2c-gpio]
                         │
                         ├──> Адреса 0x27 ──> [HD44780 LCD (Через hd44780.ko)]
                         ├──> Адреса 0x68 ──> [Годинник RTC DS3231]
                         └──> Адреса 0x20 ──> [Додатковий блок реле]
```

[Go to Top ⬆️](#-language-selection--вибір-мови--выбор-языка)

## 🇷🇺 Русская документация

### 📝 Описание проекта
Высокопроизводительный монолитный модуль ядра Linux (`hd44780.ko`) и веб-CGI экосистема управления, разработанная для устройств OpenWrt (оптимизирована под Orange Pi Zero). Драйвер выступает в роли «черного ящика» для пользовательских программ (User-Space), позволяя им выводить текст и загружать кастомные символы через унифицированный интерфейс `/dev/hd44780.X` с помощью стандартных файловых операций или `ioctl`.

Драйвер полностью изолирует аппаратный уровень и поддерживает на лету три режима работы:
1.  **High-Speed Native GPIO:** Прямое параллельное 4-битное подключение интерфейса с использованием 7 дискретных пинов GPIO.
2.  **Built-in Hardware I2C:** Подключение через расширитель PCF8574 с использованием аппаратных I2C контроллеров процессора (`i2c-0`, `i2c-1` и т.д.).
3.  **Virtual Software I2C (`i2c-gpio`):** Динамическая компиляция и регистрация программной шины I2C внутри ядра на любых свободных GPIO ножках, без необходимости задействования сторонних драйверов.

### 🧱 Архитектура и структура проекта
*   `hd44780-gpio-custom.c` — Главная точка входа модуля ядра. Обрабатывает параметры `insmod`, парсит аргументы и разводит логику инициализации.
*   `hd44780-lcd-ops.c` / `.h` — Реализация чистого параллельного 4-битного драйвера Native GPIO, аппаратные циклы задержек (`udelay`) и операции с файлами `miscdevice`.
*   `hd44780-lcd-i2c.c` / `.h` — Реализация I2C уровня драйвера. Выполняет динамическое выделение `platform_device` для виртуальных субшин и регистрирует клиентов `pcf8574`.
*   `i2c_HD44780.init` — Системный скрипт инициализации OpenWrt, управляющий запуском демонов автоматизации и 4-канальным независимым терморегулятором на базе DS18B20.
*   `Setup.cgi` / `Save.cgi` — Безопасная подсистема веб-интерфейса настройки конфигурации, защищенная от Quirks Mode и сбоев разметки страницы.

### ⚙️ Конфигурация UCI (`/etc/config/hd44780-gpio-custom`)
```config
config display 'pins'
	option lcd_mode_dev 'LCD_MODE_I2C'   # 'LCD_MODE_I2C' или 'LCD_MODE_GPIO'
	option i2c_bus_type 'builtin'        # 'builtin' (аппаратный) или 'virtual' (виртуальный i2c-gpio)
	option dev '0'                       # Номер устройства (/dev/hd44780.0) или шины i2c-0
	option address '0x27'                # Шестнадцатеричный адрес PCF8574 (0x27 или 0x3F)
	option sda_gpio '20'                 # Пин SDA (Только для режима 'virtual')
	option scl_gpio '21'                 # Пин SCL (Только для режима 'virtual')
	option hd_rs '0'                     # Распиновка для Native GPIO (Только для 'LCD_MODE_GPIO')
	option hd_rw '1'
	option hd_e '2'
	option hd_db4 '3'
	option hd_db5 '15'
	option hd_db6 '16'
	option hd_db7 '14'
```

### 🛠 Компиляция внутри OpenWrt Buildroot
Поместите папку с исходным кодом в директорию `package/kernel/hd44780-gpio-custom/src/`. Используйте следующий стандартный мультиобъектный `Makefile`:
```makefile
obj-m += hd44780.o
hd44780-y := hd44780-gpio-custom.o hd44780-lcd-ops.o hd44780-lcd-i2c.o
ccflags-y += -I\$(LINUX_DIR)/drivers/gpio
```
Запустите компиляцию из корня вашего Buildroot:
```bash
make package/kernel/hd44780-gpio-custom/compile V=s
```

### 🚀 Примеры запуска
**Режим аппаратного встроенного I2C:**
```bash
insmod hd44780 dev_mode="LCD_MODE_I2C" i2c_bus_type="builtin" dev=0 address=0x27
```
**Режим программного виртуального I2C (Bitbang):**
```bash
insmod hd44780 dev_mode="LCD_MODE_I2C" i2c_bus_type="virtual" dev=3 address=0x27 sda_gpio=15 scl_gpio=16
```
**Режим скоростного Native GPIO:**
```bash
insmod hd44780 dev_mode="LCD_MODE_GPIO" bus0=0,0,1,2,3,15,16,14
```


#### 🧭 Принцип работы
*   Пользовательский бинарник взаимодействует с аппаратным уровнем драйвера через файл символьного устройства `/dev/hd44780.X`.
*   Программа открывает этот файл с помощью стандартного системного вызова `open()` с правами на чтение и запись.
*   После получения дескриптора файла процесс передает прямые инструкции, текст или загружает графику через `ioctl`.
*   Архитектура гарантирует полную независимость выполнения пользовательского кода от физического типа подключения экрана.

#### 📡 Команды связи и функции API
*   **Протокол IOCTL команд:** 4 основные команды, которые синхронизированы между заголовками ядра и кодом программы.

```c
/* Стандартные команды HD44780. Настройка IOCTL команд для связи с драйвером ядра */
#define LCD_IOC_MAGIC 'k'
#define LCD_IOC_WRITE_CMD   _IOW(LCD_IOC_MAGIC, 1, unsigned char)
#define LCD_IOC_WRITE_DATA  _IOW(LCD_IOC_MAGIC, 2, unsigned char)
#define LCD_IOC_SET_CGRAM   _IOW(LCD_IOC_MAGIC, 3, unsigned char[8])
#define LCD_IOC_READ_DATA   _IOR(LCD_IOC_MAGIC, 4, unsigned char)
```

*   **Функции отправки данных:** Стандартные функции процесса для отправки конфигурационных инструкций и текстовых символов.

```c
void write_CMD(char c) {
	unsigned char cmd = (unsigned char)c;
	
	// Вызываем ioctl с кодом WRITE_CMD
	if (ioctl(file, LCD_IOC_WRITE_CMD, &cmd) < 0) {
		fprintf(stderr, "IOCTL CMD Error: %s\n", strerror(errno));
		errFl = 1;
		sprintf(temp_gpOut, "IOCTL CMD Error: %s", strerror(errno));
		error_log(temp_gpOut);
	}
}

// Отправка чистых данных для дисплея (символы текста)
void write_dat(char c) {
	unsigned char data = (unsigned char)c;
	
	// Вызываем ioctl с кодом WRITE_DATA
	if (ioctl(file, LCD_IOC_WRITE_DATA, &data) < 0) {
		fprintf(stderr, "IOCTL DATA Error: %s\n", strerror(errno));
		errFl = 1;
		sprintf(temp_gpOut, "IOCTL DATA Error: %s", strerror(errno));
		error_log(temp_gpOut);
	}
}
```

#### 🛠 Маршрутизация устройств в функции `main()`
*   Внутри основного жизненного цикла программы бинарник динамически формирует путь к требуемому файлу устройства.
*   Этот механизм позволяет независимо управлять как аппаратными экранами, так и виртуальными шинами или параллельными линиями.
*   Код надежно защищает операционную систему от переполнения буфера благодаря строгому отслеживанию границ памяти.

```c
int main(int argc, char** argv) {
	// ...... код ......

	// Защита от переполнения буфера setlcd_patch (максимум 20 bytes)
	snprintf(setlcd_patch, sizeof(setlcd_patch), "%s", setlcd);

	// Динамическое формирование пути к файлу устройства LCD_DEV
	if (arg_dev_bit == 1) sprintf(dev_path, "%s%s", LCD_DEV, arg_dev_num);
	else sprintf(dev_path, "%s0", LCD_DEV);

	if ((file = open(dev_path, O_RDWR)) < 0) {
		errFl = 1;
		fprintf(stderr,"Open GPIO_Dev Error: %s (%d)\n", strerror(errno), errno);
		sprintf(temp_gpOut,"Open GPIO_Dev Error: %s (%d)", strerror(errno), errno);
		error_log(temp_gpOut);
	}

	// ....... код .........
}
```

### Масштабируемость и расширение железа

> [!NOTE]
> Благодаря интеграции уровня `i2c-gpio` через официальные аппаратные \
> `gpiod` API ядра, созданная виртуальная шина работает как полноценный, \
> стандартный **Системный I2C Контроллер**.

#### 🎛️ Распределение системных ресурсов
*   **Блокировка GPIO:** Выбранные пины SDA/SCL полностью резервируются ядром. \
    Прямое управление через `/sys/class/gpio/` блокируется во избежание \
    аппаратных конфликтов.
*   **Многозадачность шины:** Узел устройства `/dev/i2c-X` остается открытым \
    для всей системы. Другие программы пользователя или драйверы могут \
    параллельно считывать данные.
*   **Топология шины:** Несколько разных чипов могут физически сидеть на одних \
    и тех же проводах, если они имеют уникальные шестнадцатеричные адреса.

#### 📈 Матрица расширения периферии

| Целевое железо | Протокол связи | Роль в системе | Поведение комплекса |
| :--- | :--- | :--- | :--- |
| **DS3231 / DS1307** | I2C (Общие линии) | Часы RTC | Сохраняют точный ход времени при полных \
ребутах без доступа к сети |

| **Дополнительный PCF8574** | I2C (Общие линии) | Расширитель портов | Увеличивает сетку автоматизации с 4 до 8, \
12 или 16 независимых реле |

| **BME280 / Si7021** | I2C (Общие линии) | Климатический сенсор | Считывает влажность, атмосферное давление \
и температуру среды |

```text
[Процессор SoC] ──> [Драйвер виртуальной шины i2c-gpio]
                         │
                         ├──> Адрес 0x27 ──> [HD44780 LCD (Через hd44780.ko)]
                         ├──> Адрес 0x68 ──> [Часы RTC DS3231]
                         └──> Адрес 0x20 ──> [Дополнительный блок реле]
```

[Go to Top ⬆️](#-language-selection--вибір-мови--выбор-языка)
