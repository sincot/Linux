BIN=probar
CC=gcc
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

$(BIN):$(OBJ)
	@$(CC) -o $@ $^
%.o:%.c
	@$(CC) -c $<

.PHONY:clean
clean:
	@rm -rf $(BIN) $(OBJ)



