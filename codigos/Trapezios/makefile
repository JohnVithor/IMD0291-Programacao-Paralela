PROG_S = trap_serial.bin
PROG_P = trap_parallel.bin
PROG_PR = trap_parallel_reduce.bin

CC = gcc
MPI = mpicc

BIN_PATH = ./bin
OBJ_PATH = ./build
SRC_PATH = ./src

MPIFLAGS = -Wall
CFLAGS = -Wall -ansi -pedantic -std=c11

OBJS_S = $(OBJ_PATH)/trap_serial.o
OBJS_P = $(OBJ_PATH)/trap_parallel.o
OBJS_PR = $(OBJ_PATH)/trap_parallel_reduce.o

RM = rm -v

all: dir $(PROG_S) $(PROG_P) $(PROG_PR)

debug: CFLAGS += -g -O0 -pg
debug: MPIFLAGS += -g -O0 -pg
debug: dir $(PROG_S) $(PROG_P)

$(PROG_S) : $(OBJS_S)
	$(CC) -o $(BIN_PATH)/$(PROG_S) $(OBJS_S)

$(PROG_P) : $(OBJS_P)
	$(MPI) -o $(BIN_PATH)/$(PROG_P) $(OBJS_P)

$(PROG_PR) : $(OBJS_PR)
	$(MPI) -o $(BIN_PATH)/$(PROG_PR) $(OBJS_PR)

$(OBJS_S): $(SRC_PATH)/trap_serial.c
	$(CC) -c $(CFLAGS) -o $@  $<

$(OBJS_P): $(SRC_PATH)/trap_parallel.c
	$(MPI) -c $(MPIFLAGS) -o $@ $<

$(OBJS_PR): $(SRC_PATH)/trap_parallel_reduce.c
	$(MPI) -c $(MPIFLAGS) -o $@ $<

dir:
	mkdir -p bin build

clean: dir
	$(RM) $(BIN_PATH)/*.bin
	$(RM) $(OBJ_PATH)/*.o

run_serial:
	time ./bin/trap_serial.bin $(args)

run_parallel:
	time mpiexec -n 6 ./bin/trap_parallel.bin $(args)
	time mpiexec -n 6 ./bin/trap_parallel_reduce.bin $(args)

