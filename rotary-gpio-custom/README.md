# Custom GPIO bitbanging drivers for OpenWrt (v2.2 / v2.3 API)

## # Спосіб застосування драйверів `*-gpio-custom` (Українська)

Ці модулі ядра є універсальними та повністю апаратно-незалежними. Вони автоматично визначають текстове ім'я поточного GPIO-контролера SoC за допомогою внутрішнього дескриптора ядра (`desc->gdev->chip->label`). Параметри передаються у вигляді масивів за допомогою утиліти `insmod` або через конфігураційні файли.

Кожен рядок аргументу `busX` (або `deviceX`) приймає параметри у форматі:  
`busX=<id>,<pin1>,<pin2>,...`

---

### 1. 1-Wire шина (`w1-gpio-custom`)
Приймає 3 параметри: `<id>,<pin>,[od]`
* **id** — унікальний системний номер пристрою (для формування платформи `w1-gpio.ID`).
* **pin** — номер GPIO піна для лінії даних 1-Wire.
* **od** — режим роботи піна (`0` = Active High, `1` = Open Drain).

**Приклади команд:**
```bash
insmod w1-gpio-custom bus0=1,10,1
```

---

### 2. Ротаційний енкодер (`rotary-gpio-custom`)
Приймає 5 параметрів: `<id>,<gpio_a>,<gpio_b>,[steps],[axis]`
* **id** — унікальний системний номер пристрою (для формування `rotary-encoder.ID`).
* **gpio_a** — номер GPIO піна для Фази А (Data A).
* **gpio_b** — номер GPIO піна для Фази B (Data B).
* **steps** — кількість кроків на один повний оборот (за замовчуванням: `24`).
* **axis** — код осі введення Linux Input Event (за замовчуванням: `0`, що відповідає `REL_X`).

**Приклади команд:**
```bash
# Підключення одного енкодера з ID 0 на піни GPIO 10 та GPIO 11
insmod rotary-gpio-custom bus0=0,10,11

# Підключення двох енкодерів (ID 0 на GPIO 10,11 та ID 1 на GPIO 20,21 з 12 кроками)
insmod rotary-gpio-custom bus0=0,10,11 bus1=1,20,21,12
```

---

### 3. Апаратна шина I2C (`i2c-gpio-custom`)
Приймає 3 параметри: `<id>,<sda>,<scl>`
* **id** — бажаний номер шини I2C в системі (створить пристрій `/dev/i2c-ID`).
* **sda** — номер GPIO піна для лінії даних (SDA).
* **scl** — номер GPIO піна для лінії тактування (SCL).
* *Примітка: Піни автоматично конфігуруються ядром у режим Open-Drain.*

**Приклади команд:**
```bash
# Створення шини /dev/i2c-4 на пінах GPIO 2 (SDA) та GPIO 3 (SCL)
insmod i2c-gpio-custom bus0=4,2,3

# Створення двох незалежних шин I2C (номер 4 та номер 5)
insmod i2c-gpio-custom bus0=4,2,3 bus1=5,18,19
```

---

### 4. Апаратна шина SPI (`spi-gpio-custom`)
Приймає 5 параметрів: `<id>,<sck>,<mosi>,<miso>,<cs>`
* **id** — системний номер шини SPI (створить пристрої `/dev/spidevID.X`).
* **sck** — номер GPIO піна для тактування (Serial Clock).
* **mosi** — номер GPIO піна для виходу даних (Master Out Slave In).
* **miso** — номер GPIO піна для входу даних (Master In Slave Out).
* **cs** — номер GPIO піна для вибору мікросхеми (Chip Select / Slave Select).

**Приклади команд:**
```bash
# Створення шини SPI з ID 1 (піни: SCK=4, MOSI=5, MISO=6, CS=7)
insmod spi-gpio-custom bus0=1,4,5,6,7

# Створення двох різних шин SPI (ID 1 та ID 2)
insmod spi-gpio-custom bus0=1,4,5,6,7 bus1=2,12,13,14,15
```

---
---

## # Usage Guide for `*-gpio-custom` Drivers (English)

These kernel modules are universal and hardware-agnostic. They automatically resolve the dynamic GPIO controller string name (`desc->gdev->chip->label`) of the active SoC. Parameters are passed as arrays using the `insmod` command line or via configuration files.

Each `busX` (or `deviceX`) line accepts arguments formatted as:  
`busX=<id>,<pin1>,<pin2>,...`

---

### 1. 1-Wire Bus (`w1-gpio-custom`)
Accepts 3 parameters: `<id>,<pin>,[od]`
* **id** — Unique system device platform ID (creates `w1-gpio.ID`).
* **pin** — GPIO pin number for the 1-Wire data line.
* **od** — Pin operation mode (`0` = Active High, `1` = Open Drain).

**Command Examples:**
```bash
insmod w1-gpio-custom bus0=1,10,1
```

---

### 2. Rotary Encoder (`rotary-gpio-custom`)
Accepts 5 parameters: `<id>,<gpio_a>,<gpio_b>,[steps],[axis]`
* **id** — Unique system platform device ID (creates `rotary-encoder.ID`).
* **gpio_a** — GPIO pin number for Phase A (Data A).
* **gpio_b** — GPIO pin number for Phase B (Data B).
* **steps** — Number of steps per full rotation (default: `24`).
* **axis** — Linux Input Event axis code (default: `0`, which maps to `REL_X`).

**Command Examples:**
```bash
# Single encoder configuration with ID 0 on GPIO 10 and GPIO 11
insmod rotary-gpio-custom bus0=0,10,11

# Two separate encoders (ID 0 on GPIO 10,11 and ID 1 on GPIO 20,21 with 12 steps)
insmod rotary-gpio-custom bus0=0,10,11 bus1=1,20,21,12
```

---

### 3. I2C Bus (`i2c-gpio-custom`)
Accepts 3 parameters: `<id>,<sda>,<scl>`
* **id** — Desired I2C adapter number (creates system `/dev/i2c-ID` node).
* **sda** — GPIO pin number used for the Serial Data line.
* **scl** — GPIO pin number used for the Serial Clock line.
* *Note: Pins are automatically handled as Open-Drain by the kernel.*

**Command Examples:**
```bash
# Create an I2C adapter /dev/i2c-4 using GPIO 2 (SDA) and GPIO 3 (SCL)
insmod i2c-gpio-custom bus0=4,2,3

# Create two independent I2C buses simultaneously (bus 4 and bus 5)
insmod i2c-gpio-custom bus0=4,2,3 bus1=5,18,19
```

---

### 4. SPI Bus (`spi-gpio-custom`)
Accepts 5 parameters: `<id>,<sck>,<mosi>,<miso>,<cs>`
* **id** — SPI master bus number (creates `/dev/spidevID.X` interface devices).
* **sck** — GPIO pin number used for Serial Clock.
* **mosi** — GPIO pin number used for Master Out Slave In line.
* **miso** — GPIO pin number used for Master In Slave Out line.
* **cs** — GPIO pin number used for Chip Select / Slave Select line.

**Command Examples:**
```bash
# Create an SPI master bus with ID 1 (SCK=4, MOSI=5, MISO=6, CS=7)
insmod spi-gpio-custom bus0=1,4,5,6,7

# Create two separate SPI master buses concurrently (bus 1 and bus 2)
insmod spi-gpio-custom bus0=1,4,5,6,7 bus1=2,12,13,14,15
```

