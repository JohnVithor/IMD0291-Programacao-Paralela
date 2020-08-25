PROG_S = trap_serial.bin
PROG_P = trap_parallel.bin

CC = gcc
MPI = mpicc

BIN_PATH = ./bin
OBJ_PATH = ./build
SRC_PATH = ./src

MPIFLAGS = -Wall
CFLAGS = -Wall -ansi -pedantic -std=c11

OBJS_S = $(OBJ_PATH)/trap_serial.o
OBJS_P = $(OBJ_PATH)/trap_parallel.o

RM = rm -v

all: dir $(PROG_S) $(PROG_P)

debug: CFLAGS += -g -O0 -pg
debug: MPIFLAGS += -g -O0 -pg
debug: dir $(PROG_S) $(PROG_P)

$(PROG_S) : $(OBJS_S)
	$(CC) -o $(BIN_PATH)/$(PROG_S) $(OBJS_S)

$(PROG_P) : $(OBJS_P)
	$(MPI) -o $(BIN_PATH)/$(PROG_P) $(OBJS_P)

$(OBJ_PATH)/trap_serial.o: $(SRC_PATH)/trap_serial.c
	$(CC) -c $(CFLAGS) -o $@  $<

$(OBJ_PATH)/trap_parallel.o: $(SRC_PATH)/trap_parallel.c
	$(MPI) -c $(MPIFLAGS) -o $@ $<

dir:
	mkdir -p bin build doc

clean: dir
	$(RM) $(BIN_PATH)/*.bin
	$(RM) $(OBJ_PATH)/*.o

	
doxy:
	$(RM) $(DOC_DIR)/*
	doxygen Doxyfile
