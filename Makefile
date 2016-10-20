CC=gcc
CPPFLAGS=-Wall  -Werror  -O3
#CPPFLAGS=   -g
#CPPFLAGS=-Wall -Werror -g

SRC_reg=src/utils.o src/priority_queue.o src/ai.o src/2048.o
SRC_exp=src/utils.o src/priority_queue.o src/ai.o src/2048_experiment.o
TARGET_reg=2048
TARGET_exp=2048_experiment
e_a=./2048_experiment ai avg
e_m=./2048_experiment ai max

all: $(SRC_reg)
	$(CC) -o $(TARGET_reg) $(SRC_reg) $(CPPFLAGS);

clean:
	rm -f src/*.o; rm -f output.txt;

clear:
	clear; clear; clear; clear; clear; clear; clear; clear; clear; clear; clear; clear; clear; clear; clear; clear;

git:
	git add .; git commit -m "mycommit"; git push;

compile_exp: clean $(SRC_exp)
	$(CC) -o $(TARGET_exp) $(SRC_exp) $(CPPFLAGS);

run_exp: clear run_exp_max run_exp_avg

run_exp_avg:
	$(e_a) 0; $(e_a) 1; $(e_a) 2; $(e_a) 3; $(e_a) 4; $(e_a) 5; $(e_a) 6;# $(e_m) 7; $(e_m) 8; $(e_m) 9; $(e_m) 10;

run_exp_max:
	$(e_m) 0; $(e_m) 1; $(e_m) 2; $(e_m) 3; $(e_m) 4; $(e_m) 5; $(e_m) 6;# $(e_m) 7; $(e_m) 8; $(e_m) 9; $(e_m) 10;
