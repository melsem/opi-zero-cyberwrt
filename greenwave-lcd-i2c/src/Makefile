BINS = greenwave-lcd-i2c
OBJS = main.o display.o lcd_print_cyr.o
LIBS = -luci

all: $(BINS)

$(BINS): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $(BINS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

display.o: display.c
	$(CC) $(CFLAGS) -c display.c -o display.o

lcd_print_cyr.o: lcd_print_cyr.c
	$(CC) $(CFLAGS) -c lcd_print_cyr.c -o lcd_print_cyr.o

clean:
	rm -f $(BINS) $(OBJS)

.PHONY: all clean

