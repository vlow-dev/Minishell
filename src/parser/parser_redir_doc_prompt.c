/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_doc_prompt.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:56:58 by vlow              #+#    #+#             */
/*   Updated: 2025/04/24 16:50:33 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <unistd.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/wait.h>

int		doc_quoted(char *str);
int		isvalid_buf(t_data *data, t_token *token);

static int	hd_prompt_child_rl(t_data *data, char **line, int *prompt_fd)
{
	char	*temp;

	temp = readline("> ");
	if (!temp)
	{
		close(prompt_fd[1]);
		tty_prompt_restore(data);
		exit (0);
	}
	*line = ft_strjoin_free(temp, "\n", 0);
	if (!*line)
	{
		close(prompt_fd[1]);
		tty_prompt_restore(data);
		exit (0);
	}
	return (0);
}

static int	hd_prompt_child(t_data *data, char *ptr, int len, int *prompt_fd)
{
	char	*line;

	line = NULL;
	signal_set(data, SIGINT, SIG_DFL);
	signal_set(data, SIGQUIT, SIG_DFL);
	close(prompt_fd[0]);
	tty_prompt_setfd(data);
	while (1)
	{
		hd_prompt_child_rl(data, &line, prompt_fd);
		if (!ft_strncmp(line, ptr, len) && line[len] == '\n')
		{
			ft_printf_fd(prompt_fd[1], "%s", line);
			break ;
		}
		ft_printf_fd(prompt_fd[1], "%s", line);
		free(line);
	}
	free(line);
	close(prompt_fd[1]);
	tty_prompt_restore(data);
	exit (0);
}

static int	hd_prompt_parent(t_token *token, char *ptr, int len, int *prompt_fd)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = get_next_line(prompt_fd[0]);
		if (!line)
			return (close(prompt_fd[0]), 0);
		if (!ft_strncmp(line, ptr, len) && line[len] == '\n')
		{
			token->buf = split_free_append(token->buf, line, 0);
			if (!token->buf)
				return (close(prompt_fd[0]), free(line), 0);
			break ;
		}
		token->buf = split_free_append(token->buf, line, 0);
		if (!token->buf)
			return (close(prompt_fd[0]), free(line), 0);
		free(line);
	}
	free(line);
	close(prompt_fd[0]);
	return (1);
}

static int	init_prompter(t_data *data, t_token *token, char *ptr, int len)
{
	pid_t	pid;
	int		prompt_fd[2];

	pid = -1;
	if (pipe(prompt_fd) == -1)
		return (perror("Prompt Pipe Fatal Error"), 0);
	pid = fork();
	if (pid == -1)
	{
		ft_printf_fd(2, "Minishell: Fork Fatal Error\n");
		return (0);
	}
	if (!pid)
		hd_prompt_child(data, ptr, len, prompt_fd);
	close(prompt_fd[1]);
	signal_set(data, SIGINT, SIG_IGN);
	if (!hd_prompt_parent(token, ptr, len, prompt_fd))
		return (0);
	waitpid(pid, NULL, 0);
	signal_restore(data, SIGINT);
	return (1);
}

int	init_heredoc_prompt(t_data *data, t_token *token)
{
	int		ret;
	char	*ptr;

	ret = isvalid_buf(data, token);
	if (ret)
		return (ret);
	ptr = ft_strdup(token->buf[2]);
	if (!ptr)
		return (0);
	if (doc_quoted(ptr))
		quotes_memmove(ptr);
	if (!init_prompter(data, token, ptr, ft_strlen(ptr)))
		return (free(ptr), 0);
	free(ptr);
	return (1);
}
