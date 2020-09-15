PROG_S1 = pi_serial
PROG_P = pi_parallel

CC = gcc
MPI = mpicc

BIN_PATH = ./bin
OBJ_PATH = ./build
SRC_PATH = ./src

MPIFLAGS = -Wall
CFLAGS = -Wall -ansi -pedantic -std=c11

OBJS_S1 = $(OBJ_PATH)/$(PROG_S1).o
OBJS_S2 = $(OBJ_PATH)/$(PROG_S2).o
OBJS_P = $(OBJ_PATH)/$(PROG_P).o

RM = rm -v

all: dir $(PROG_S1).bin $(PROG_P).bin  

$(PROG_S1).bin : $(OBJS_S1)
	$(CC) -o $(BIN_PATH)/$(PROG_S1).bin $(OBJS_S1) -lm

$(PROG_P).bin : $(OBJS_P)
	$(MPI) -o $(BIN_PATH)/$(PROG_P).bin $(OBJS_P) -lm

$(OBJS_S1): $(SRC_PATH)/$(PROG_S1).c
	$(CC) -c $(CFLAGS) -o $@  $< -lm

$(OBJS_P): $(SRC_PATH)/$(PROG_P).c
	$(MPI) -c $(MPIFLAGS) -o $@ $< -lm

dir:
	mkdir -p bin build

clean: dir
	$(RM) $(BIN_PATH)/*.bin
	$(RM) $(OBJ_PATH)/*.o

run_all: run_serial run_parallel

run_serial:
	time ./bin/$(PROG_S1).bin $(args)
	
run_parallel:
	time mpiexec -n 6 ./bin/$(PROG_P).bin $(args)

