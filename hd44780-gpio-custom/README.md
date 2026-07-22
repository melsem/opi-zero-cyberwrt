# Custom GPIO Dynamic Drivers for OpenWrt / Linux

This repository contains universal Linux kernel drivers 
(`w1-gpio-custom` and `hd44780-gpio-custom`) to dynamically 
initialize **One-Wire (W1)** and **HD44780** devices via GPIO.

Designed for **Linux kernel 5.4.0+** using modern **gpiod API**.

---

## EN (English)

### Key Features
* **No Device Tree:** Registers devices on the fly without DTS.
* **Modern gpiod API:** Uses dynamic `gpiod_lookup_table`.
* **Auto Chip Detection:** Finds `chip_label` automatically.
* **Multi-Bus:** Supports up to 4 concurrent buses.

### Usage and Syntax

#### 1. One-Wire (`w1-gpio-custom`)
Format: `id,pin,od` (`od` flag: `1` = active, `0` = disabled).
```bash
insmod w1-gpio-custom.ko bus0=0,4,1
```

#### 2. LCD Display (`hd44780-gpio-custom`)
Format: `id,rs,rw,e,db4,db5,db6,db7` (4-bit mode).
```bash
insmod hd44780-gpio-custom.ko bus0=1,12,13,14,16,17,18,19
```

### Unloading
Removes all allocated structures cleanly:
```bash
rmmod w1-gpio-custom
rmmod hd44780-gpio-custom
```

---

## UA (Українська)

### Основні переваги
* **Без Device Tree:** Реєструє пристрої на льоту без DTS.
* **Сучасний gpiod API:** Працює через `gpiod_lookup_table`.
* **Автовизначення чіпа:** Сама знаходить назву `chip_label`.
* **Мульти-шина:** Підтримує до 4 шин одночасно.

### Використання та Синтаксис

#### 1. Шнур One-Wire (`w1-gpio-custom`)
Формат: `id,pin,od` (прапорець `od`: `1` — увімк, `0` — вимк).
```bash
insmod w1-gpio-custom.ko bus0=0,4,1
```

#### 2. Дисплей LCD (`hd44780-gpio-custom`)
Формат: `id,rs,rw,e,db4,db5,db6,db7` (4-бітний режим).
```bash
insmod hd44780-gpio-custom.ko bus0=1,12,13,14,16,17,18,19
```

### Вивантаження
Повністю очищає виділені структури з пам'яті:
```bash
rmmod w1-gpio-custom
rmmod hd44780-gpio-custom
```

---

## 🛠 OpenWrt Makefile Example
```makefile
include \$(TOPDIR)/rules.mk
include \$(INCLUDE_DIR)/kernel.mk

PKG_NAME:=gpio-custom-drivers
PKG_RELEASE:=1

include \$(INCLUDE_DIR)/package.mk

define KernelPackage/w1-gpio-custom
  SUBMENU:=1-Wire support
  TITLE:=Custom GPIO 1-Wire bus driver
  FILES:=\$(PKG_BUILD_DIR)/w1-gpio-custom.ko
endef

define KernelPackage/hd44780-gpio-custom
  SUBMENU:=Display support
  TITLE:=Custom GPIO HD44780 display driver
  FILES:=\$(PKG_BUILD_DIR)/hd44780-gpio-custom.ko
endef

EXTRA_CFLAGS += -I\$(LINUX_DIR)/drivers/gpio

define Build/Compile
	\((KERNEL_MAKE) M="\)(PKG_BUILD_DIR)" modules
endef

(eval (call KernelPackage,w1-gpio-custom))
(eval (call KernelPackage,hd44780-gpio-custom))
```

## 📝 License
Distributed under the **GPL v2** license.

