NAME = minishell

SRC_DIR = src
INC_DIR = include
LIBFT_DIR = libft
TOKENINZATION_DIR = $(SRC_DIR)/tokenization
PARSER_DIR = $(SRC_DIR)/parser
EXPANSION_DIR = $(SRC_DIR)/expansion
EXECUTION_DIR = $(SRC_DIR)/execution
BUILTINS_DIR = $(SRC_DIR)/builtins
INITS_DIR = $(SRC_DIR)/inits

# SRC = $(wildcard $(SRC_DIR)/*.c) \
# 	  $(wildcard $(TOKENINZATION_DIR)/*.c) \
# 	  $(wildcard $(PARSER_DIR)/*.c) \
# 	  $(wildcard $(EXPANSION_DIR)/*.c) \
# 	  $(wildcard $(EXPANSION_DIR)/braces/*.c) \
# 	  $(wildcard $(EXPANSION_DIR)/tilde/*.c) \
# 	  $(wildcard $(EXPANSION_DIR)/sign/*.c) \
# 	  $(wildcard $(EXPANSION_DIR)/glob/*.c) \
# 	  $(wildcard $(EXPANSION_DIR)/quotes/*.c) \
# 	  $(wildcard $(EXECUTION_DIR)/*.c) \
# 	  $(wildcard $(BUILTINS_DIR)/*.c) \
# 	  $(wildcard $(INITS_DIR)/*.c) \

SRC = \
	src/builtins/bi_ft_cd.c \
	src/builtins/bi_ft_echo.c \
	src/builtins/bi_ft_env.c \
	src/builtins/bi_ft_exit.c \
	src/builtins/bi_ft_export.c \
	src/builtins/bi_ft_pwd.c \
	src/builtins/bi_ft_unset.c \
	src/execution/execute_access.c \
	src/execution/execute_builtins.c \
	src/execution/execute_logical.c \
	src/execution/execute_pid_utils.c \
	src/execution/execute_pipe.c \
	src/execution/execute_redir2.c \
	src/execution/execute_redir.c \
	src/execution/execute_subshell_seq.c \
	src/execution/execute_words2.c \
	src/execution/execute_words.c \
	src/execution/execution.c \
	src/expansion/braces/braces_commas.c \
	src/expansion/braces/braces_expansion.c \
	src/expansion/braces/braces_extract.c \
	src/expansion/braces/braces_sequence.c \
	src/expansion/braces/braces_sequence_utils.c \
	src/expansion/expansion.c \
	src/expansion/expansion_inquotes.c \
	src/expansion/expansion_utils.c \
	src/expansion/glob/glob_expansion.c \
	src/expansion/glob/glob_init.c \
	src/expansion/glob/glob_utils.c \
	src/expansion/quotes/quotes_expansion.c \
	src/expansion/quotes/quotes_utils.c \
	src/expansion/sign/sign_expansion.c \
	src/expansion/sign/sign_utils2.c \
	src/expansion/sign/sign_utils.c \
	src/expansion/tilde/tilde_expansion.c \
	src/expansion/tilde/tilde_user2.c \
	src/expansion/tilde/tilde_user.c \
	src/inits/cleanup.c \
	src/inits/envp_utils.c \
	src/inits/init_envp.c \
	src/inits/init_prompt.c \
	src/inits/init_signal.c \
	src/inits/input_check.c \
	src/inits/input_nl.c \
	src/minishell.c \
	src/parser/parser_ast_utils.c \
	src/parser/parser.c \
	src/parser/parser_groups.c \
	src/parser/parser_operator.c \
	src/parser/parser_print.c \
	src/parser/parser_print_utils.c \
	src/parser/parser_prompt_rl.c \
	src/parser/parser_redir.c \
	src/parser/parser_redir_doc_expansion.c \
	src/parser/parser_redir_doc_prompt.c \
	src/parser/parser_redir_utils.c \
	src/parser/parser_seq_bg.c \
	src/parser/parser_simple_cmd.c \
	src/parser/parser_subshell.c \
	src/parser/parser_utils.c \
	src/parser/parser_word.c \
	src/tokenization/create_token.c \
	src/tokenization/create_token_utils.c \
	src/tokenization/normalize_grouping.c \
	src/tokenization/normalize_logical.c \
	src/tokenization/normalize_redir_token.c \
	src/tokenization/normalize_redir_token_utils.c \
	src/tokenization/normalize_subshell.c \
	src/tokenization/normalize_word_token.c \
	src/tokenization/tokenization.c \
	src/tokenization/tokenization_utils2.c \
	src/tokenization/tokenization_utils.c

OBJ = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
IFLAGS = -I$(INC_DIR) -I$(LIBFT_DIR)/$(INC_DIR)
LFLAGS = -l$(LIBFT_DIR)/libft.a
LIBFT = $(LIBFT_DIR)/libft.a

RFLAGS = -lreadline

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@echo "Creating $(NAME)..."
	@$(CC) $(CFLAGS) $(IFLAGS) $(OBJ) $(LIBFT) $(RFLAGS) -o $(NAME) 
	@echo "Created $(NAME)"
	# @make clean

$(LIBFT):
	@make -C $(LIBFT_DIR) all

bonus: all

%.o: %.c
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	@echo "Removing objects files..."
	@make -C $(LIBFT_DIR) clean
	@rm -f $(OBJ)
	@echo "Objects files removed."

fclean: clean
	@echo "Removing $(NAME)..."
	@make -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)
	@echo "$(NAME) removed."

re: fclean all

debug: CFLAGS += -g3 -fpic
debug: clean all
	@make -C $(LIBFT_DIR) debug
	@echo "Enable debug mode"

valgrind:
	valgrind --leak-check=full ./minishell

fval:
	valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell

fsanitize: CFLAGS += -fsanitize=address,leak -g3 -O1
fsanitize: clean all
	@echo "AddressSanitizer enabled for $(NAME)"

fsan: fsanitize

.PHONY: all clean fclean re bonus debug valgrind fsanitize fsan fval
