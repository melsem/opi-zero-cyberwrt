# Визначаємо ім'я фінального модуля
obj-m += hd44780.o

# Зшиваємо головний файл та файл операцій разом
hd44780-y := hd44780-gpio-custom.o hd44780-lcd-ops.o

# Шлях до приватних структур gpiolib.h в ядрі
ccflags-y += -I$(LINUX_DIR)/drivers/gpio

