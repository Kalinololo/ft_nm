CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRC = src/main.c src/parse.c src/utils.c src/ft_nm64.c src/ft_nm32.c
OBJ = $(SRC:.c=.o)
OBJS = $(subst src, bin, $(OBJ))
EXEC = ft_nm

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test:
	sh test.sh


clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(EXEC)

re: fclean all

.PHONY: all clean fclean re