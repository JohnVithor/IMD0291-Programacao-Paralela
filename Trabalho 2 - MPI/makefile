NAME_S = odd_even_serial
NAME_P1 = odd_even_parallel
NAME_P2 = odd_even_parallel_otimized

PROG_S = $(NAME_S).bin
PROG_P1 = $(NAME_P1).bin
PROG_P2 = $(NAME_P2).bin

CC = gcc
MPI = mpicc

BIN_PATH = ./bin
OBJ_PATH = ./build
SRC_PATH = ./src

MPIFLAGS = -Wall
CFLAGS = -Wall -ansi -pedantic -std=c11

OBJS_S = $(OBJ_PATH)/$(NAME_S).o
OBJS_P1 = $(OBJ_PATH)/$(NAME_P1).o
OBJS_P2 = $(OBJ_PATH)/$(NAME_P2).o

RM = rm -v

all: dir $(PROG_S) $(PROG_P1) $(PROG_P2)

debug: CFLAGS += -g -O0 -pg
debug: MPIFLAGS += -g -O0 -pg
debug: dir $(PROG_S) $(PROG_P1) $(PROG_P2)

$(PROG_S) : $(OBJS_S)
	$(CC) -o $(BIN_PATH)/$(PROG_S) $(OBJS_S) -lm

$(OBJS_S): $(SRC_PATH)/$(NAME_S).c
	$(CC) -c $(CFLAGS) -o $@  $<  -lm

$(PROG_P1) : $(OBJS_P1)
	$(MPI) -o $(BIN_PATH)/$(PROG_P1) $(OBJS_P1)  -lm

$(OBJS_P1): $(SRC_PATH)/$(NAME_P1).c  -lm
	$(MPI) -c $(MPIFLAGS) -o $@ $<

$(PROG_P2) : $(OBJS_P2)
	$(MPI) -o $(BIN_PATH)/$(PROG_P2) $(OBJS_P2)  -lm

$(OBJS_P2): $(SRC_PATH)/$(NAME_P2).c  -lm
	$(MPI) -c $(MPIFLAGS) -o $@ $<

dir:
	mkdir -p bin build

clean: dir
	$(RM) $(BIN_PATH)/*.bin
	$(RM) $(OBJ_PATH)/*.o
