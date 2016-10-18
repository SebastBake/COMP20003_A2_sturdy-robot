CC=gcc
#CPPFLAGS=-Wall  -Werror  -O3
#CPPFLAGS=   -g
CPPFLAGS=-Wall -Werror -g

SRC=src/utils.o src/priority_queue.o src/ai.o src/2048.o 
TARGET=2048

all: clean $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CPPFLAGS)

clean:
	rm -f $(TARGET) src/*.o

clear:
	clear; clear; clear; clear; clear; clear; clear;

git:
	git add .; git commit -m "mycommit"; git push;
