PROG_S = odd_even_serial.bin
PROG_P = odd_even_parallel.bin

CC = gcc
MPI = mpicc

BIN_PATH = ./bin
OBJ_PATH = ./build
SRC_PATH = ./src
OUT_PATH = ./out

MPIFLAGS = -Wall
CFLAGS = -Wall -ansi -pedantic -std=c11

OBJS_S = $(OBJ_PATH)/odd_even_serial.o
OBJS_P = $(OBJ_PATH)/odd_even_parallel.o

RM = rm -v

all: dir $(PROG_S) $(PROG_P)

debug: CFLAGS += -g -O0 -pg
debug: MPIFLAGS += -g -O0 -pg
debug: dir $(PROG_S) $(PROG_P)

$(PROG_S) : $(OBJS_S)
	$(CC) -o $(BIN_PATH)/$(PROG_S) $(OBJS_S)

$(PROG_P) : $(OBJS_P)
	$(MPI) -o $(BIN_PATH)/$(PROG_P) $(OBJS_P)

$(OBJS_S): $(SRC_PATH)/odd_even_serial.c
	$(CC) -c $(CFLAGS) -o $@  $<

$(OBJS_P): $(SRC_PATH)/odd_even_parallel.c
	$(MPI) -c $(MPIFLAGS) -o $@ $<


dir:
	mkdir -p bin build out

clean: dir
	$(RM) $(BIN_PATH)/*.bin
	$(RM) $(OBJ_PATH)/*.o

run_all: run_serial run_parallel

run_serial:
	time ./bin/odd_even_serial.bin $(shell cat ${args}.txt) > $(OUT_PATH)/$(args)_serial.out
	sort -C $(OUT_PATH)/$(args)_serial.out || echo -n "not " ; echo "sorted";

run_parallel:
	time mpiexec -n 4 ./bin/odd_even_parallel.bin $(shell cat ${args}.txt) > $(OUT_PATH)/$(args)_parallel.out
	sort -C $(OUT_PATH)/$(args)_parallel.out || echo -n "not " ; echo "sorted";

