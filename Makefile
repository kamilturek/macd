CC     = cc
CFLAGS = -Wall -Wextra -Werror

SOURCE = $(wildcard *.c)
OBJS   = $(SOURCE:.c=.o)

macd: main.c
	$(CC) -framework ApplicationServices -lcJSON -o macd $(SOURCE)

clean:
	rm -f macd $(OBJS)

help:
	-@echo "make [macd]\t build the executable"
	-@echo "make clean\t clean the directory from executable and object files"
