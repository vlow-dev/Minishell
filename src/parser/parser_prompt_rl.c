/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_prompt_rl.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:49:01 by vlow              #+#    #+#             */
/*   Updated: 2025/04/24 16:46:46 by vlow             ###   ########.fr       */
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

int	tty_prompt_setfd(t_data *data)
{
	data->tty.s_out = -1;
	data->tty.fd = -1;
	if (isatty(STDIN_FILENO))
	{
		data->tty.fd = open("/dev/tty", O_WRONLY);
		if (data->tty.fd == -1)
			return (0);
		data->tty.s_out = dup(STDOUT_FILENO);
		if (data->tty.s_out == -1)
		{
			close(data->tty.fd);
			data->tty.fd = -1;
			return (0);
		}
		if (dup2(data->tty.fd, STDOUT_FILENO) == -1)
		{
			close(data->tty.fd);
			close(data->tty.s_out);
			data->tty.fd = -1;
			data->tty.s_out = -1;
			return (0);
		}
	}
	return (1);
}

void	tty_prompt_restore(t_data *data)
{
	if (data->tty.s_out != -1)
	{
		dup2(data->tty.s_out, STDOUT_FILENO);
		close(data->tty.s_out);
		data->tty.s_out = -1;
	}
	if (data->tty.fd != -1)
	{
		close(data->tty.fd);
		data->tty.fd = -1;
	}
}

static int	rl_prompt_child(t_data *data, int *prompt_fd)
{
	char	*line;

	signal_set(data, SIGINT, SIG_DFL);
	signal_set(data, SIGQUIT, SIG_DFL);
	close(prompt_fd[0]);
	tty_prompt_setfd(data);
	line = readline("> ");
	if (!line)
		exit (0);
	ft_printf_fd(prompt_fd[1], "%s", line);
	free(line);
	close(prompt_fd[1]);
	tty_prompt_restore(data);
	exit (0);
}

static int	rl_prompt_parent(t_data *data, int *prompt_fd,
							char **op_line, int pid)
{
	signal_set(data, SIGINT, SIG_IGN);
	waitpid(pid, NULL, 0);
	signal_restore(data, SIGINT);
	if (data->line)
	{
		*op_line = ft_strjoin_free(*op_line, data->line, 1);
		data->line = NULL;
		if (!*op_line)
			return (close(prompt_fd[0]), 0);
	}
	data->line = get_next_line(prompt_fd[0]);
	if (!data->line)
	{
		if (!g_signal)
			ft_printf_fd(2,
				"Minishell: syntax error: unexpected end of file\n");
		ft_free_setn((void **)op_line);
		close(prompt_fd[0]);
		return (0);
	}
	close(prompt_fd[0]);
	return (1);
}

int	ast_prompt_rl(t_data *data, char **op_line)
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
		rl_prompt_child(data, prompt_fd);
	close(prompt_fd[1]);
	if (!rl_prompt_parent(data, prompt_fd, op_line, pid))
		return (0);
	return (1);
}
