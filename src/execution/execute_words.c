/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_words.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:26:52 by vlow              #+#    #+#             */
/*   Updated: 2025/04/24 16:47:09 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int		execute_redir(t_data *data, t_tree *leaf);
int		execute_builtins(t_data *data, char **cmds);
int		is_builtins(t_tree *leaf);
int		check_access(t_data *data, char *cmd);
int		word_builtins(t_data *data, t_tree *leaf);

static int	word_forks_child_setfd(t_data *data)
{
	if (data->pipe.fd[1] != -1)
	{
		dup2(data->pipe.fd[1], STDOUT_FILENO);
		close(data->pipe.fd[1]);
		data->pipe.fd[1] = -1;
	}
	if (data->pipe.fd[0] != -1)
	{
		dup2(data->pipe.fd[0], STDIN_FILENO);
		close(data->pipe.fd[0]);
		data->pipe.fd[0] = -1;
	}
	if (data->pipe.child[0] != -1)
	{
		close(data->pipe.child[0]);
		data->pipe.child[0] = -1;
	}
	if (data->pipe.child[1] != -1)
	{
		close(data->pipe.child[1]);
		data->pipe.child[1] = -1;
	}
	return (1);
}

static int	word_forks_child_exec(t_data *data, t_tree *leaf)
{
	if (is_builtins(leaf))
	{
		data->exit_status = execute_builtins(data, leaf->token->buf);
		exit(data->exit_status);
	}
	else
	{
		if (!check_access(data, leaf->token->buf[0]))
		{
			ft_printf_fd(2, "Minishell: %s: %s\n", leaf->token->buf[0],
				strerror(ENOENT));
			exit (127);
		}
		execve(data->envp.cmd_ep, leaf->token->buf, data->envp.arr);
		ft_printf_fd(2, "Minishell: %s: %s\n", leaf->token->buf[0],
			strerror(EACCES));
		exit(126);
	}
	return (1);
}

static int	word_forks_child(t_data *data, t_tree *leaf)
{
	int	i;

	i = 0;
	signal_set(data, SIGINT, SIG_DFL);
	signal_set(data, SIGQUIT, SIG_DFL);
	if (data->pipe.size > 0)
		word_forks_child_setfd(data);
	while (i < leaf->size)
	{
		if (leaf->branch[i] && leaf->branch[i]->size != -1)
		{
			if (!execute_redir(data, leaf->branch[i]))
			{
				data->exit_status = 1;
				exit (1);
			}
		}
		i++;
	}
	word_forks_child_exec(data, leaf);
	return (1);
}

static int	word_forks(t_data *data, t_tree *leaf)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Word Fork Fatal Error!");
		return (0);
	}
	if (!pid)
	{
		word_forks_child(data, leaf);
	}
	signal_set(data, SIGINT, SIG_IGN);
	if (!add_pid(data, pid))
		return (0);
	return (1);
}

int	execute_word(t_data *data, t_tree *leaf)
{
	if (!sh_expansion(data, leaf))
	{
		ft_printf_fd(2, "sh_expansion failed\n");
		return (0);
	}
	if (!leaf->token->buf || !*leaf->token->buf)
		return (0);
	if (is_builtins(leaf) && !data->pipe.size)
		return (word_builtins(data, leaf));
	else
		word_forks(data, leaf);
	return (1);
}
