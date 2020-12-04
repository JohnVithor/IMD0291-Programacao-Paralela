NAME_S = serial
NAME_P = parallel

BIN_PATH = ./bin
OBJ_PATH = ./build
SRC_PATH = ./src

CC = gcc
MPIFLAGS = -Wall
CFLAGS = -Wall -ansi -pedantic -std=c11

PROG_S = $(addsuffix .bin, $(NAME_S))
PROG_P = $(addsuffix .bin, $(NAME_P))
OBJS_S_aux = $(addprefix $(OBJ_PATH)/, $(NAME_S))
OBJS_S = $(addsuffix .o, $(OBJS_S_aux))
OBJS_P_aux = $(addprefix $(OBJ_PATH)/, $(NAME_P))
OBJS_P = $(addsuffix .o, $(OBJS_P_aux))

RM = rm -v

all: dir $(PROG_S) $(PROG_P)

debug: CFLAGS += -g -O0 -pg
debug: dir $(PROG_S) $(PROG_P)

%.bin : $(OBJ_PATH)/%.o
	if "$@" | grep -q "parallel"; then $(CC) -o $(BIN_PATH)/$@ $< -lm; else $(CC) -o $(BIN_PATH)/$@ $< -lm -fopenmp; fi;

.SECONDARY:
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	$(CC) -c $(CFLAGS) -o $@ $< -fopenmp

dir:
	mkdir -p bin build

clean: dir
	$(RM) $(BIN_PATH)/*
	$(RM) $(OBJ_PATH)/*.o
