# Имя финального бинарного файла (OpenWrt ищет именно ir-interceptor)
TARGET = ir-interceptor

# Автоматический поиск всех файлов .c в папке src
SRCS = main.c signals.c ir_handler.c gpio_handler.c keyboard_handler.c

# Превращение списка .c файлов в список объектных файлов .o
OBJS = $(SRCS:.c=.o)

# Эти переменные OpenWrt передаст снаружи, но зададим дефолты для локальной сборки
CC ?= gcc
CFLAGS ?= -Wall -O2

# Главное правило сборки
all: $(TARGET)

# Линковка объектных файлов в готовый исполняемый файл
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Компиляция каждого .c файла в .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка сборочной директории
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
