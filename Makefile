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
	gcc resources/sample.c -o resources/sample_64
	gcc resources/sample.c -g -o resources/sample_64_debug
	gcc -m32 resources/sample.c -o resources/sample_32
	gcc -m32 resources/sample.c -g -o resources/sample_32_debug


clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(EXEC)

re: fclean all

.PHONY: all clean fclean re