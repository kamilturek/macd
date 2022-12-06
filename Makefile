CC      = cc
CFLAGS  = -Wall -Wextra -Werror
LDFLAGS = -lcJSON -framework ApplicationServices

SOURCE = $(wildcard src/*.c)
TESTSOURCE = $(filter-out src/main.c, $(SOURCE)) $(wildcard tests/*.c)

macd: $(SOURCE)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SOURCE)

test:
	$(CC) -I ./src/ -lcheck $(CFLAGS) $(LDFLAGS) $(TESTSOURCE) -o ./runtests
	./runtests

clean:
	rm -f macd

help:
	-@echo "make [macd]\t build the executable"
	-@echo "make clean\t clean the directory from executable and object files"
