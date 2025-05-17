##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## .
##

NAME = ray_cast

SRC = src/main.c \
      src/wolf3d.c \
      src/start_ray.c \

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lcsfml-graphics -lcsfml-system -lcsfml-audio -lcsfml-window -lm -lncurses

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)

debug: CFLAGS += -g
debug: LDFLAGS += -g
debug: fclean $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all