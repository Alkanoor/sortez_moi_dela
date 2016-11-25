EXEC = fast_sort
OBJ = $(EXEC).o

CC = gcc
CFLAGS = -Wall -Werror -O1

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $< -o $@ $(CFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean :
	@rm -rf $(EXEC)
	@rm -rf $(OBJ)
