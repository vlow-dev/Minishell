/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:18:18 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:14:48 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int	process_redir_rw(t_data *data, t_tree *leaf);
int	process_redir_fdups(t_data *data, t_tree *leaf);
int	exec_here_doc(t_data *data, t_tree *leaf);

static int	process_redir_in(t_data *data, t_tree *leaf)
{
	data->exec.fd[0] = ft_atoi(leaf->token->buf[0]);
	data->exec.fd[1] = open(leaf->token->buf[2], O_RDONLY);
	if (data->exec.fd[1] == -1)
	{
		ft_printf_fd(2, "Minishell: %s: %s\n",
			leaf->token->buf[2], strerror(errno));
		return (0);
	}
	if (dup2(data->exec.fd[1], data->exec.fd[0]) == -1)
	{
		ft_printf_fd(2, "Minishell: %s: %s\n",
			leaf->token->buf[2], strerror(errno));
		close(data->exec.fd[1]);
		data->exec.fd[1] = -1;
		return (0);
	}
	close(data->exec.fd[1]);
	data->exec.fd[1] = -1;
	return (1);
}

static int	process_redir_out(t_data *data, t_tree *leaf)
{
	data->exec.fd[0] = ft_atoi(leaf->token->buf[0]);
	data->exec.fd[1] = open(leaf->token->buf[2],
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->exec.fd[1] == -1)
	{
		ft_printf_fd(2, "Minishell: %s: %s\n",
			leaf->token->buf[2], strerror(errno));
		return (0);
	}
	if (dup2(data->exec.fd[1], data->exec.fd[0]) == -1)
	{
		ft_printf_fd(2, "Minishell: %s: %s\n",
			leaf->token->buf[2], strerror(errno));
		close(data->exec.fd[1]);
		data->exec.fd[1] = -1;
		return (0);
	}
	close(data->exec.fd[1]);
	data->exec.fd[1] = -1;
	return (1);
}

static int	process_redir_append(t_data *data, t_tree *leaf)
{
	data->exec.fd[0] = ft_atoi(leaf->token->buf[0]);
	data->exec.fd[1] = open(leaf->token->buf[2],
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (data->exec.fd[1] == -1)
	{
		ft_printf_fd(2, "Minishell: %s: %s\n",
			leaf->token->buf[2], strerror(errno));
		return (0);
	}
	if (dup2(data->exec.fd[1], data->exec.fd[0]) == -1)
	{
		ft_printf_fd(2, "Minishell: %s: %s\n",
			leaf->token->buf[2], strerror(errno));
		close(data->exec.fd[1]);
		data->exec.fd[1] = -1;
		return (0);
	}
	close(data->exec.fd[1]);
	data->exec.fd[1] = -1;
	return (1);
}

static int	process_redir_heres(t_data *data, t_tree *leaf)
{
	int	len;

	len = 0;
	if (pipe(data->exec.fd) == -1)
		perror("<<|<<< pipe failed");
	if (leaf->token->type == TOKEN_HEREDOC)
		exec_here_doc(data, leaf);
	else
	{
		len = ft_strlen(leaf->token->buf[2]);
		if (len && leaf->token->buf[2][len - 1] != '\n')
			ft_printf_fd(data->exec.fd[1], "%s\n", leaf->token->buf[2]);
		else
			ft_printf_fd(data->exec.fd[1], "%s", leaf->token->buf[2]);
		close(data->exec.fd[1]);
		data->exec.fd[1] = -1;
	}
	dup2(data->exec.fd[0], STDIN_FILENO);
	close(data->exec.fd[0]);
	data->exec.fd[0] = -1;
	return (1);
}

int	execute_redir(t_data *data, t_tree *leaf)
{
	if (!sh_expansion(data, leaf))
	{
		ft_printf_fd(2, "sh_expansion failed\n");
		return (0);
	}
	if (leaf->token->type == TOKEN_REDIR_IN)
		return (process_redir_in(data, leaf));
	else if (leaf->token->type == TOKEN_REDIR_OUT)
		return (process_redir_out(data, leaf));
	else if (leaf->token->type == TOKEN_APPEND)
		return (process_redir_append(data, leaf));
	else if (leaf->token->type == TOKEN_HERESTR
		|| leaf->token->type == TOKEN_HEREDOC)
		return (process_redir_heres(data, leaf));
	else if (leaf->token->type == TOKEN_REDIR_IDUP
		|| leaf->token->type == TOKEN_REDIR_ODUP)
		return (process_redir_fdups(data, leaf));
	else if (leaf->token->type == TOKEN_REDIR_RW)
		return (process_redir_rw(data, leaf));
	return (-1);
}
