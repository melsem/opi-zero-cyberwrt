# OpenWrt TDA7313 Audio Processor Controller (C-Version)

<!-- LANGUAGES NAVIGATION BUTTONS -->
<p align="center">
  <a href="#english-version">🇬🇧 English</a> │ 
  <a href="#українська-версія">🇺🇦 Українська</a> │ 
  <a href="#русская-версия">🇷🇺 Русский</a>
</p>

---

<div id="english-version"></div>

## 🇬🇧 English Version

<details open>
<summary><b>Click to expand/collapse English Documentation</b></summary>

### 🎧 Audio System Architecture

The acoustics feature a highly custom hardware design.
It lacks a classic physical tone control for low frequencies.
This limitation is fully compensated for by the logic of this compiled C microcode:

*   **Subwoofer (Separate LF Channel):**
    Physically wired to the rear output channels (`LR` and `RR`).
    It works through a passive Low-Pass Filter (LPF).
    Instead of a classic Bass register, the low frequencies are controlled dynamically.
    The level (`sbass`) is changed by modifying the relative attenuation of these exact output channels.

*   **Satellites (Mid/HF Sandwich):**
    Physically wired to the front output channels (`LF` and `RF`).
    They utilize a dual control logic layout.
    They use the built-in hardware tone control register (`treble`).
    They also use relative incremental volume control of the front channels (`streble`) to emphasize mid-frequencies.

### 🛠 Key Features

1.  **Soft Fade-in:**
    Upon system boot (`init`), the volume initializes to absolute silence (63 dB attenuation).
    Then it smoothly ramps up to the target level.
    It uses precise hardware microsecond pauses via `usleep(15000)`.
    This completely prevents loud pops and clicks in speakers.

2.  **Anti-Click Switch (Hardware Input Mute):**
    During input switching (`switch / input`), the program temporarily mutes the channel.
    It applies a hardware mute bit (+32 to the switch register) for 20 ms.
    This eliminates relay click noises.

3.  **UCI Integration:**
    I2C parameters (`i2c_adres` and `i2c_dev`) are automatically read from the system config.
    The program fetches them from `/etc/config/tda7313`.
    If settings are missing, fallback defaults (`0x44` and `0`) are automatically applied.

### 🚀 Usage and Command Syntax

Format: `tda7313 <type> <value>`

*   `tda7313 init` — Complete chip initialization with Soft Fade-in.
*   `tda7313 volume +` / `tda7313 volume -` — Smooth volume step (range: 0..63).
*   `tda7313 sbass +` / `tda7313 sbass -` — Subwoofer volume adjustment via `LR/RR` attenuators.
*   `tda7313 streble +` / `tda7313 streble -` — Satellites volume adjustment via `LF/RF` attenuators.
*   `tda7313 bass +` / `tda7313 treble +` — Hardware TDA7313 chip tone boost (0..15).
*   `tda7313 switch 1` — Clickless switching to Input №1 (0..3).
*   `tda7313 mute` — Mute the audio instantly.

### 🏗 Build and Installation

Compile inside OpenWrt SDK:
```bash
make package/i2c-encoder-tda7313/compile V=s
```

Install on router:
```bash
opkg install i2c-encoder-tda7313_*.ipk
/etc/init.d/tda7313 enable
/etc/init.d/tda7313 start
```
</details>

---

<div id="українська-версія"></div>

## 🇺🇦 Українська версія

<details>
<summary><b>Натисніть, щоб розгорнути/згорнути українську документацію</b></summary>

### 🎧 Особливості архітектури аудіосистеми

Акустика має нестандартну схемотехніку звукових шляхів.
У ній повністю відсутній фізичний класичний темброблок для низьких частот.
Цей нюанс повністю компенсується на рівні логіки даного компільованого С-коду:

*   **Сабвуфер (Окремий канал НЧ):**
    Фізично підключений до задніх вихідних каналів мікросхеми (`LR` та `RR`).
    Сигнал проходить через пасивний фільтр низьких частот (ФНЧ).
    Замість класичного регістру Bass, рівень низьких частот (`sbass`) регулюється інакше.
    Він змінюється через коригування відносної гучності (атенюації) саме цих вихідних каналів.

*   **Сателіти (Сендвіч СЧ/ВЧ):**
    Підключені до передніх виходів мікросхеми (`LF` та `RF`).
    Для них використовується подвійна логіка керування частотним балансом.
    Задіюється класичний вбудований апаратний темброблок мікросхеми (`treble`).
    Також застосовується відносне інкрементальне регулювання гучності передніх каналів (`streble`) для виділення середніх частот.

### 🛠 Ключовий функціонал програми

1.  **Soft Fade-in (Автоматичний плавний старт):**
    При завантаженні служби роутера (`init`) гучність ініціалізується в повну тишу (63 дБ атенюації).
    Після цього рівень звуку плавно нарощується до цільового значення.
    Кроки виконуються за допомогою точних апаратних мікросекундних пауз `usleep(15000)`.
    Це повністю виключає появу звукових ударів та щигликів у динаміках.

2.  **Анти-клік комутація (Hardware Input Mute):**
    Під час перемикання аудіо-входів (`switch / input`) програма тимчасово затихає.
    Вона на 20 мс блокує вхідний канал спеціальним апаратним бітом мутування (+32 до регістру комутатора).
    Це допомагає приглушити клацання реле та ліній під час зміни джерела.

3.  **Повна UCI-інтеграція:**
    Параметри I2C шини (`i2c_adres` та `i2c_dev`) динамічно зчитуються з налаштувань.
    Програма бере їх із системного конфігу OpenWrt `/etc/config/tda7313`.
    Якщо параметри відсутні, спрацьовує захист і застосовуються дефолтні значення `0x44` та `0`.

### 🚀 Керування та синтаксис команд

Формат виклику: `tda7313 <тип> <значення>`

*   `tda7313 init` — Повна ініціалізація мікросхеми та запуск плавного нарощування звуку.
*   `tda7313 volume +` / `tda7313 volume -` — Плавний крок гучності звуку (межі 0..63).
*   `tda7313 sbass +` / `tda7313 sbass -` — Регулювання відносної гучності Сабвуфера через атенюатори `LR/RR`.
*   `tda7313 streble +` / `tda7313 streble -` — Регулювання гучності сателітів СЧ/ВЧ через атенюатори `LF/RF`.
*   `tda7313 bass +` / `tda7313 treble +` — Класичне апаратне підсилення темброблоку TDA7313 (0..15).
*   `tda7313 switch 1` — Безшумне перемикання на AUX/Вхід №1 (0..3).
*   `tda7313 mute` — Миттєве затишення звуку.

### 🏗 Збірка та встановлення

Для компіляції пакету в середовищі OpenWrt SDK виконайте:
```bash
make package/i2c-encoder-tda7313/compile V=s
```

Встановлення на роутер:
```bash
opkg install i2c-encoder-tda7313_*.ipk
/etc/init.d/tda7313 enable
/etc/init.d/tda7313 start
```
</details>

---

<div id="русская-version"></div>

## 🇷🇺 Русская версия

<details>
<summary><b>Нажмите, чтобы развернуть/свернуть русскую документацию</b></summary>

### 🎧 Особенности архитектуры аудиосистемы

Акустика имеет нестандартную схемотехнику звуковых трактов.
В ней полностью отсутствует физический классический темброблок низких частот.
Этот нюанс компенсируется на уровне логики данного скомпилированного С-кода:

*   **Сабвуфер (Отдельный канал НЧ):**
    Физически подключен к задним выходным каналам микросхемы (`LR` и `RR`).
    Сигнал идет через пассивный фильтр низких частот (ФНЧ).
    Вместо классического регистра Bass, уровень низких частот (`sbass`) регулируется иначе.
    Он изменяется путем корректировки относительной громкости (аттенюации) именно этих выходных каналов.

*   **Сателлиты (Сэндвич СЧ/ВЧ):**
    Подключены к передним выходам микросхемы (`LF` и `RF`).
    Для них используется двойная логика управления частотным балансом.
    Задействуется классический встроенный аппаратный темброблок микросхемы (`treble`).
    Также применяется относительное инкрементальное регулирование громкости передних каналов (`streble`) для выделения средних частот.

### 🛠 Ключевой функционал программы

1.  **Soft Fade-in (Автоматический плавный старт):**
    При загрузке службы роутера (`init`) громкость инициализируется в полную тишину (63 дБ аттенюации).
    После этого уровень звука плавно нарастает до целевого значения.
    Шаги выполняются с помощью точных аппаратных микросекундных пауз `usleep(15000)`.
    Это полностью исключает появление звуковых ударов и щелчков в динамиках.

2.  **Анти-клик коммутация (Hardware Input Mute):**
    Во время переключения аудио-входов (`switch / input`) программа временно затихает.
    Она на 20 мс блокирует входной канал специальным аппаратным битом мутирования (+32 к регистру коммутатора).
    Это помогает приглушить щелчки реле и линий во время смены источника.

3.  **Полная UCI-интеграция:**
    Параметры I2C шины (`i2c_adres` и `i2c_dev`) динамически считываются из настроек.
    Программа берет их из системного конфига OpenWrt `/etc/config/tda7313`.
    Если параметры отсутствуют, срабатывает защита и применяются дефолтные значения `0x44` и `0`.

### 🚀 Управление и синтаксис команд

Формат вызова: `tda7313 <тип> <значение>`

*   `tda7313 init` — Полная инициализация микросхемы и запуск плавного нарастания звука.
*   `tda7313 volume +` / `tda7313 volume -` — Плавный шаг громкости звука (пределы 0..63).
*   `tda7313 sbass +` / `tda7313 sbass -` — Регулировка относительной громкости Сабвуфера через аттенюаторы `LR/RR`.
*   `tda7313 streble +` / `tda7313 streble -` — Регулировка громкости сателлитов СЧ/ВЧ через аттенюаторы `LF/RF`.
*   `tda7313 bass +` / `tda7313 treble +` — Классическое аппаратное усиление темброблока TDA7313 (0..15).
*   `tda7313 switch 1` — Бесшумное переключение на AUX/Вход №1 (0..3).
*   `tda7313 mute` — Мгновенное приглушение звука.

### 🏗 Сборка и установка

Для компиляции пакета в среде OpenWrt SDK выполните:
```bash
make package/i2c-encoder-tda7313/compile V=s
```

Установка на роутер:
```bash
opkg install i2c-encoder-tda7313_*.ipk
/etc/init.d/tda7313 enable
/etc/init.d/tda7313 start
```
</details>

***

