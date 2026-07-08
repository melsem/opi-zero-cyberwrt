obj-m := hd44780_mod.o
hd44780_mod-y := hd44780.o cmd_write_init.o

all:
	$(MAKE) -C $(LINUX_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(LINUX_DIR) M=$(PWD) clean

