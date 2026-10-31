# Визначаємо ім'я фінального модуля
obj-m += hd44780.o

# Зшиваємо головний файл, еталонні операції GPIO та нові операції I2C разом
hd44780-y := hd44780-gpio-custom.o hd44780-lcd-ops.o hd44780-lcd-i2c.o

# Шлях до приватних структур gpiolib.h в ядрі
ccflags-y += -I$(LINUX_DIR)/drivers/gpio
