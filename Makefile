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

# Test configuration
TEST_DIR = tests
TEST_BIN_DIR = $(TEST_DIR)/bin
TEST_STRING = $(TEST_DIR)/string/tests_string.c
TEST_STRING_BIN = $(TEST_BIN_DIR)/test_string

# Colors
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RESET = \033[0m
BOLD = \033[1m

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
	rm -rf $(TEST_BIN_DIR)

re: fclean all

# ═══════════════════════════════════════════════════════════════════════════
# Test Rules
# ═══════════════════════════════════════════════════════════════════════════

# Build test binary
$(TEST_STRING_BIN): $(NAME) $(TEST_STRING) $(TEST_DIR)/test_framework.h
	@mkdir -p $(TEST_BIN_DIR)
	@printf "$(BLUE)$(BOLD)Building tests...$(RESET)\n"
	@$(CC) $(WFLAGS) $(INCFLAGS) $(TEST_STRING) -L. -ltypes -o $(TEST_STRING_BIN)
	@printf "$(GREEN)$(BOLD)Tests built successfully!$(RESET)\n\n"

# Run tests with verbose output (shows each test result)
test: $(TEST_STRING_BIN)
	@printf "$(BOLD)$(BLUE)═══════════════════════════════════════════$(RESET)\n"
	@printf "$(BOLD)$(BLUE)         RUNNING STRING TESTS (VERBOSE)     $(RESET)\n"
	@printf "$(BOLD)$(BLUE)═══════════════════════════════════════════$(RESET)\n"
	@./$(TEST_STRING_BIN) -v; exit_code=$$?; \
	if [ $$exit_code -eq 0 ]; then \
		printf "$(GREEN)$(BOLD)Exit code: 0 (SUCCESS)$(RESET)\n"; \
	else \
		printf "$(RED)$(BOLD)Exit code: $$exit_code (FAILURE)$(RESET)\n"; \
	fi; \
	exit $$exit_code

# Run tests with only final result
test-quiet: $(TEST_STRING_BIN)
	@printf "$(BOLD)$(BLUE)═══════════════════════════════════════════$(RESET)\n"
	@printf "$(BOLD)$(BLUE)         RUNNING STRING TESTS (QUIET)       $(RESET)\n"
	@printf "$(BOLD)$(BLUE)═══════════════════════════════════════════$(RESET)\n"
	@./$(TEST_STRING_BIN); exit_code=$$?; \
	if [ $$exit_code -eq 0 ]; then \
		printf "$(GREEN)$(BOLD)Exit code: 0 (SUCCESS)$(RESET)\n"; \
	else \
		printf "$(RED)$(BOLD)Exit code: $$exit_code (FAILURE)$(RESET)\n"; \
	fi; \
	exit $$exit_code

# Clean and run tests
test-re: fclean test

# Alias for test
tests: test

.PHONY: all clean fclean re test test-quiet test-re tests

