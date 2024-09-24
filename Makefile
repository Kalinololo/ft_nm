CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRC = src/main.c src/parse.c
OBJ = $(SRC:.c=.o)
OBJS = $(subst src, bin, $(OBJ))
EXEC = ft_nm

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(EXEC)

re: fclean all

.PHONY: all clean fclean re