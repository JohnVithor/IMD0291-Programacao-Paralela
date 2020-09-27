NAME_S = pi_serial
NAME_P = pi_parallel
NAME_PR = pi_parallel_reduce

PROG_S = $(NAME_S).bin
PROG_P = $(NAME_P).bin
PROG_PR = $(NAME_PR).bin

CC = gcc
MPI = mpicc

BIN_PATH = ./bin
OBJ_PATH = ./build
SRC_PATH = ./src

MPIFLAGS = -Wall
CFLAGS = -Wall -ansi -pedantic -std=c11

OBJS_S = $(OBJ_PATH)/$(NAME_S).o
OBJS_P = $(OBJ_PATH)/$(NAME_P).o
OBJS_PR = $(OBJ_PATH)/$(NAME_PR).o

RM = rm -v

all: dir $(PROG_S) $(PROG_P) $(PROG_PR)

debug: CFLAGS += -g -O0 -pg
debug: MPIFLAGS += -g -O0 -pg
debug: dir $(PROG_S) $(PROG_P)

$(PROG_S) : $(OBJS_S)
	$(CC) -o $(BIN_PATH)/$(PROG_S) $(OBJS_S) -lm

$(PROG_P) : $(OBJS_P)
	$(MPI) -o $(BIN_PATH)/$(PROG_P) $(OBJS_P)  -lm

$(PROG_PR) : $(OBJS_PR)
	$(MPI) -o $(BIN_PATH)/$(PROG_PR) $(OBJS_PR)  -lm

$(OBJS_S): $(SRC_PATH)/$(NAME_S).c
	$(CC) -c $(CFLAGS) -o $@  $<  -lm

$(OBJS_P): $(SRC_PATH)/$(NAME_P).c  -lm
	$(MPI) -c $(MPIFLAGS) -o $@ $<

$(OBJS_PR): $(SRC_PATH)/$(NAME_PR).c
	$(MPI) -c $(MPIFLAGS) -o $@ $<  -lm

dir:
	mkdir -p bin build

clean: dir
	$(RM) $(BIN_PATH)/*.bin
	$(RM) $(OBJ_PATH)/*.o

run_all: run_serial run_parallel

run_serial:
	./bin/$(NAME_S).bin $(args)

run_parallel:
	mpiexec -n 6 ./bin/$(NAME_P).bin $(args)
	mpiexec -n 6 ./bin/$(NAME_PR).bin $(args)