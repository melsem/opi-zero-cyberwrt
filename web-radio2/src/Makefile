TARGET1 = ch-buffer
TARGET2 = rc-radio
TARGET3 = v-buffer
TARGET4 = web-radio2

SRCS1 = ch-main.c ch-buffer.c
SRCS2 = rc-radio.c
SRCS3 = v-buffer.c
SRCS4 = main.c play.c opl.c error_log.c helpComand.c

OBJS1 = $(SRCS1:.c=.o)
OBJS2 = $(SRCS2:.c=.o)
OBJS3 = $(SRCS3:.c=.o)
OBJS4 = $(SRCS4:.c=.o)

# OBJS = main.o play.o opl.o error_log.o helpComand.o

CC ?= gcc
CFLAGS ?= -Wall -O2

all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4)

$(TARGET1): $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) -o $(TARGET1)

$(TARGET2): $(OBJS2)
	$(CC) $(CFLAGS) $(OBJS2) -o $(TARGET2)

$(TARGET3): $(OBJS3)
	$(CC) $(CFLAGS) $(OBJS3) -o $(TARGET3)

$(TARGET4): $(OBJS4)
	$(CC) $(CFLAGS) $(OBJS4) -o $(TARGET4)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4)

.PHONY: all clean

