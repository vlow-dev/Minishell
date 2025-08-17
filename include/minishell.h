/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 14:56:04 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:19:20 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <unistd.h>
# include <signal.h>

extern volatile sig_atomic_t	g_signal;

// Colors prompt
# define CYAN "\033[1;36m"
# define PURPLE "\033[1;35m"
# define BLUE "\033[1;34m"
# define YELLOW "\033[1;33m"
# define GREEN "\033[1;32m"
# define RED "\033[1;31m"
# define BLACK       "\033[1;30m"
# define WHITE       "\033[1;37m"
# define LIGHT_RED   "\033[0;91m"
# define LIGHT_GREEN "\033[0;92m"
# define LIGHT_YELLOW "\033[0;93m"
# define LIGHT_BLUE  "\033[0;94m"
# define LIGHT_MAGENTA "\033[0;95m"
# define LIGHT_CYAN  "\033[0;96m"
# define GRAY        "\033[0;90m"
# define COLOR "\033[0m"
# define RL_CYAN "\001\033[1;36m\002"
# define RL_COLOR "\001\033[0m\002"

// Token define
# define T_HERESTR "<<<"
# define T_REDIR_IDUP "<&"
# define T_REDIR_ODUP ">&"
# define T_REDIR_RW "<>"
# define T_HEREDOC "<<"
# define T_APPEND ">>"
# define T_REDIR_IN "<"
# define T_REDIR_OUT ">"
# define T_OR "||"
# define T_AND "&&"
# define T_PIPE "|"
# define T_PAREN_OPEN "("
# define T_PAREN_CLOSE ")"
# define T_BRACES_OPEN "{"
# define T_BRACES_CLOSE "}"
# define T_BACKGROUND "&"
# define T_SEMICOLON ";"
# define T_NEWLINE "\n"
# define T_NULL

// Meta character
# define M_WSPACE " \t\n"
# define M_CTRL_OP "&|(){};"
# define M_REDIR_OP "<>"
# define M_META_CHAR "<>|&(){};\'\"$"
# define M_QUOTES "\'\"`"
# define M_PAREN_OPEN "({"
# define M_PAREN_CLOSE ")}"
# define M_BRAC_OPEN "["
# define M_BRAC_CLOSE "]"
# define M_TILDE "~"
# define M_SIGN_SP "?$#"
# define M_RS "\x1e"
# define M_US "\x1f"
# define M_RS_C '\x1e'
# define M_US_C '\x1f'
# define M_GLOB_PREFIX 0
# define M_GLOB_SUFFIX 1

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_HERESTR,
	TOKEN_REDIR_IDUP,
	TOKEN_REDIR_ODUP,
	TOKEN_REDIR_RW,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_PIPE,
	TOKEN_PAREN_OPEN,
	TOKEN_PAREN_CLOSE,
	TOKEN_BRACKET_OPEN,
	TOKEN_BRACKET_CLOSE,
	TOKEN_BACKGROUND,
	TOKEN_SEMICOLON,
	TOKEN_NEWLINE,
	TOKEN_NULL,
}	t_token_type;

typedef struct s_tty
{
	int	fd;
	int	s_out;
}	t_tty;

typedef struct s_pipe
{
	int		fd[2];
	int		child[2];
	int		size;
	int		base_fd[3];
	t_list	*pid;
}	t_pipe;

typedef struct s_token
{
	int				id;
	char			**buf;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_tree
{
	t_token			*token;
	struct s_tree	**branch;
	int				size;
}	t_tree;

typedef struct s_envp
{
	char	**ep;
	char	**arr;
	char	**paths;
	char	*cmd_ep;
}	t_envp;

typedef struct s_exec
{
	int	fd[2];
	int	file;
	int	flag;
}	t_exec;

typedef struct s_data
{
	struct sigaction	rl_sigint;
	struct sigaction	bg_sigchld;
	struct sigaction	rl_sigquit;
	t_token				*token;
	t_tree				*root;
	t_envp				envp;
	t_exec				exec;
	t_pipe				pipe;
	t_list				*pid;
	t_tty				tty;
	char				*line;
	int					exit_status;
	int					i_token;
	int					status;
	t_tree				*nl;
	int					nl_expand;
	int					echo_n;
}	t_data;

// Inits
int		init_parsing(t_data *data);
int		execution(t_data *data);
int		init_token(t_data *data, const char *str, int prompt);

// Parser AST
t_tree	*parse_simple_cmd(t_data *data);
t_tree	*parse_bg(t_data *data);
t_tree	*parse_seq(t_data *data);
t_tree	*parse_cmd(t_data *data);
t_tree	*parse_ctrl_op(t_data *data);
t_tree	*parse_pipe(t_data *data);
t_tree	*parse_groups(t_data *data);
t_tree	*parse_subshell(t_data *data);
t_tree	*parse_word(t_data *data);
t_tree	*parse_redir(t_data *data);
t_token	*get_token(t_data *data);

// Prompt Utils
int		ast_prompt_rl(t_data *data, char **op_line);
int		tty_prompt_setfd(t_data *data);
void	tty_prompt_restore(t_data *data);
int		operator_prompt(t_data *data);

// Expansion
int		expansion_inquotes(const char *str, const char *set);
char	*quotes_memmove(char *str);

// Execution
int		execute_tree(t_data *data, t_tree *root);
void	pid_wait(t_data *data);
int		add_pid(t_data *data, pid_t pid);

// Main Utils
char	**find_envp(t_data *data, char *str);
char	*find_envp_str(t_data *data, char *str);
int		input_check(t_data *data);
void	signal_set(t_data *data, int sig, void (*handler)(int));
void	signal_restore(t_data *data, int sig);
int		sh_expansion(t_data *data, t_tree *leaf);

// Print Utils
void	print_token(t_data *data);
void	print_ast(t_data *data);

// Free Utils
void	free_nl(t_data *data);
void	free_token(t_data *data);
void	free_tree(t_tree *root);
void	clear_data(t_data *data);

// Utils
char	*skip_quotes(const char *str);
char	*skip_nested(const char *str);
char	*skip_wspaces(const char *str);
char	*skip_to_wspaces(const char *str);
char	*skip_to_wspaces_section(const char *str);
int		ft_isesc_i(const char *str, int idx);
int		ft_isesc_ptr(const char *str, const char *idx);
void	ft_subs_sp(char **arr);
void	ft_subs_resp(char **arr);

#endif
