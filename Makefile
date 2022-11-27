CC      = cc
CFLAGS  = -Wall -Wextra -Werror
LDFLAGS = -lcJSON -framework ApplicationServices

SOURCE = $(wildcard *.c)
OBJS   = $(SOURCE:.c=.o)

macd: $(SOURCE)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SOURCE)

clean:
	rm -f macd $(OBJS)

help:
	-@echo "make [macd]\t build the executable"
	-@echo "make clean\t clean the directory from executable and object files"
