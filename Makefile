CC = cc
WFLAGS = -Wall -Wextra -Werror
INCFLAGS = -I ./inc

NAME = libtypes.a

SRC_DIR = src
OBJ_DIR = obj
STR_DIR = string
UTILS_DIR = utils

S_FILES = $(SRC_DIR)/$(STR_DIR)/string.c $(SRC_DIR)/$(UTILS_DIR)/utils.c
O_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(S_FILES))

all: $(NAME)

$(NAME): $(O_FILES)
	ar rcs $(NAME) $(O_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(WFLAGS) $(INCFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

