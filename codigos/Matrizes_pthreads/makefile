NAME_S = serial
NAME_P = parallel

PROG_S = $(NAME_S).bin
PROG_P = $(NAME_P).bin

CC = gcc

BIN_PATH = ./bin
OBJ_PATH = ./build
SRC_PATH = ./src

MPIFLAGS = -Wall
CFLAGS = -Wall -ansi -pedantic -std=c11

OBJS_S = $(OBJ_PATH)/$(NAME_S).o
OBJS_P = $(OBJ_PATH)/$(NAME_P).o

RM = rm -v

all: dir $(PROG_S) $(PROG_P)

debug: CFLAGS += -g -O0 -pg
debug: MPIFLAGS += -g -O0 -pg
debug: dir $(PROG_S) $(PROG_P)

$(PROG_S) : $(OBJS_S)
	$(CC) -o $(BIN_PATH)/$(PROG_S) $(OBJS_S) -lm

$(OBJS_S): $(SRC_PATH)/$(NAME_S).c
	$(CC) -c $(CFLAGS) -o $@  $<  -lm

$(PROG_P) : $(OBJS_P)
	$(CC) -o $(BIN_PATH)/$(PROG_P) $(OBJS_P) -lm -lpthread -lrt

$(OBJS_P): $(SRC_PATH)/$(NAME_P).c  -lm -lpthread -lrt
	$(CC) -c $(CFLAGS) -o $@ $<

dir:
	mkdir -p bin build

clean: dir
	$(RM) $(BIN_PATH)/*.bin
	$(RM) $(OBJ_PATH)/*.o
