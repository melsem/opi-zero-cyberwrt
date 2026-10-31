# OpenWrt kmod-cw1200-xr819 (Xradio XR819 Support)

Select language / Оберіть мову / Выберите язык:
- [English](#english)
- [Українська](#українська)
- [Русский](#русский)

---

## English

### Description
An out-of-tree (external) OpenWrt package that builds the **ST-Ericsson cw1200-xr819** wireless driver framework tailored for the **Xradio XR819** SDIO Wi-Fi chip. This chip is prominently used on Allwinner H2+/H3 boards, such as the **Orange Pi Zero**.

By using a modified `cw1200_wlan_sdio` module integrated with the modern `mac80211-backport` framework, this package bypasses legacy Allwinner kernels and delivers a stable Wi-Fi implementation on mainline Linux kernels (tested on **Kernel 5.15 / OpenWrt 23.05**).

### Features
- **Automated Firmware Delivery:** Automatically downloads, extracts, and places the original `xr819_fw.bin`, `sdd_xr819.bin`, and `boot_xr819.bin` blobs into `/lib/firmware/xr819/`.
- **Mainline Alignment:** Delegates hardware power management entirely to the kernel's automatic sequencer (`mmc-pwrseq-simple`), eliminating system deadlocks.
- **Clean Architecture:** Isolated package structure that copies clean, native in-kernel staging files and dynamically injects localized compatibility patches without polluting global target directories.

### Installation & Compilation
1. **DTS Configuration:** Ensure your Device Tree (`.dts`) patch in `target/linux/sunxi/patches-5.15/` contains the dual-compatibility string: `compatible = "xradio,xr819";`.
2. **Configure Indexes:** Clear the temporary configuration cache and parse the recipe:
   ```bash
   rm -rf tmp/
   make defconfig
   make menuconfig
   ```
3. **Select Package:** Go to `Kernel modules` ---> `Wireless Drivers` ---> Select `[*]` **kmod-cw1200-xr819**.
4. **Compile:** Build the module independently or run the full image build:
   ```bash
   make package/kernel/cw1200-xr819/compile V=s
   # Or full build:
   make -j$(nproc) V=s
   ```

---

## Українська

### Опис
Автономний (зовнішній) пакет OpenWrt для збирання бездротового драйвера **ST-Ericsson cw1200-xr819**, 
адаптованого під Wi-Fi чип **Xradio XR819 (SDIO)**. Цей модуль масово використовується на платах Allwinner H2+/H3, 
зокрема на **Orange Pi Zero**.

Завдяки модифікації модуля `cw1200_wlan_sdio` та його інтеграції з сучасним ядровим стеком `mac80211-backport`, 
цей пакет дозволяє відмовитися від застарілих форків Allwinner та забезпечує стабільну роботу Wi-Fi на сучасних mainline-ядрах 
(протестовано на **Ядрі 5.15 / OpenWrt 23.05**).

### Особливості
- **Автоматичне завантаження прошивки:** Система сама завантажує, розпаковує та копіює оригінальні файли `xr819_fw.bin`, `sdd_xr819.bin` 
та `boot_xr819.bin` у каталог `/lib/firmware/xr819/`.
- **Сумісність із кодом ядра:** Керування живленням повністю делеговано штатному автоматичному секвенсору ядра (`mmc-pwrseq-simple`), 
що виключає апаратні зависання шини.
- **Чиста архітектура:** Модуль є ізольованим. Він бере чистий код із папки ядра, накладає локальні патчі сумісності «на льоту» та 
не засмічує глобальні каталоги архітектури Allwinner.

### Встановлення та компіляція
1. **Налаштування DTS:** Переконайтеся, що ваш патч дерева пристроїв (`.dts`) у `target/linux/sunxi/patches-5.15/` додає рядок подвійної 
сумісності: `compatible = "xradio,xr819";`.
2. **Оновлення індексів:** Очистіть кеш конфігуратора та перечитайте файли:
   ```bash
   rm -rf tmp/
   make defconfig
   make menuconfig
   ```
3. **Вибір пакета:** Перейдіть у `Kernel modules` ---> `Wireless Drivers` ---> Позначте зірочкою `[*]` пункт **kmod-cw1200-xr819**.
4. **Запуск збирання:** Скомпілюйте модуль окремо або запустіть повну збірку образу:
   ```bash
   make package/kernel/cw1200-xr819/compile V=s
   # Або повний образ:
   make -j$(nproc) V=s
   ```

---

## Русский

### Описание
Автономный (внешний) пакет OpenWrt для сборки беспроводного драйвера **ST-Ericsson cw1200-xr819**, адаптированного под Wi-Fi чип **Xradio XR819 (SDIO)**. Данный модуль массово используется на платах Allwinner H2+/H3, в частности на **Orange Pi Zero**.

Благодаря модификации модуля `cw1200_wlan_sdio` и его интеграции с современным сетевым стеком `mac80211-backport`, этот пакет позволяет отказаться от устаревших форков Allwinner и обеспечивает стабильную работу Wi-Fi на современных mainline-ядрах (протестировано на **Ядре 5.15 / OpenWrt 23.05**).

### Особенности
- **Автоматическая загрузка прошивки:** Система самостоятельно скачивает, распаковывает и копирует оригинальные файлы `xr819_fw.bin`, `sdd_xr819.bin` и `boot_xr819.bin` в каталог `/lib/firmware/xr819/`.
- **Совместимость с кодом ядра:** Управление питанием полностью делегировано штатному автоматическому секвенсору ядра (`mmc-pwrseq-simple`), что исключает аппаратные зависания шины.
- **Чистая архитектура:** Модуль полностью изолирован. Он берет чистый исходный код из папки ядра, накладывает локальные патчи совместимости «на лету» и не засоряет глобальные каталоги архитектуры Allwinner.

### Установка и компиляция
1. **Настройка DTS:** Убедитесь, что ваш патч дерева устройств (`.dts`) в `target/linux/sunxi/patches-5.15/` добавляет строку двойной совместимости: `compatible = "xradio,xr819";`.
2. **Обновление индексов:** Очистите кэш конфигуратора и перечитайте файлы:
   ```bash
   rm -rf tmp/
   make defconfig
   make menuconfig
   ```
3. **Выбор пакета:** Перейдите в `Kernel modules` ---> `Wireless Drivers` ---> Отметьте звездочкой `[*]` пункт **kmod-cw1200-xr819**.
4. **Запуск сборки:** Скомпилируйте модуль отдельно или запустите полную сборку всего образа:
   ```bash
   make package/kernel/cw1200-xr819/compile V=s
   # Или полный образ:
   make -j\$(nproc) V=s
   ```

