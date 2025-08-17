/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:19:40 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:23:41 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int	execute_redir(t_data *data, t_tree *leaf);

int	process_redir_fdups(t_data *data, t_tree *leaf)
{
	data->exec.fd[0] = ft_atoi(leaf->token->buf[0]);
	if (!ft_strcmp(leaf->token->buf[2], "-"))
	{
		close(data->exec.fd[0]);
		data->exec.fd[0] = -1;
	}
	else
	{
		data->exec.fd[1] = ft_atoi(leaf->token->buf[2]);
		if (dup2(data->exec.fd[1], data->exec.fd[0]) == -1)
		{
			ft_printf_fd(2, "Minishell: %s: %s\n",
				leaf->token->buf[2], strerror(errno));
			return (0);
		}
		if (data->exec.fd[1] > 2)
		{
			close(data->exec.fd[1]);
			data->exec.fd[1] = -1;
		}
	}
	return (1);
}

int	process_redir_rw(t_data *data, t_tree *leaf)
{
	data->exec.fd[0] = ft_atoi(leaf->token->buf[0]);
	data->exec.fd[1] = open(leaf->token->buf[2], O_RDWR | O_CREAT, 0644);
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

static int	redir_cleanup_fd(t_data *data)
{
	if (dup2(data->pipe.base_fd[0], STDIN_FILENO) == -1)
		perror("dup2 stdin restore failed");
	if (dup2(data->pipe.base_fd[1], STDOUT_FILENO) == -1)
		perror("dup2 stdin restore failed");
	if (dup2(data->pipe.base_fd[2], STDERR_FILENO) == -1)
		perror("dup2 stdin restore failed");
	close(data->pipe.base_fd[0]);
	close(data->pipe.base_fd[1]);
	close(data->pipe.base_fd[2]);
	data->pipe.base_fd[0] = -1;
	data->pipe.base_fd[1] = -1;
	data->pipe.base_fd[2] = -1;
	return (1);
}

int	parent_redir(t_data *data, t_tree *leaf)
{
	int	i;

	i = 0;
	if (leaf->size > 0 && !leaf->token)
	{
		data->pipe.base_fd[0] = dup(STDIN_FILENO);
		data->pipe.base_fd[1] = dup(STDOUT_FILENO);
		data->pipe.base_fd[2] = dup(STDERR_FILENO);
		while (i < leaf->size)
		{
			if (leaf->branch[i] && leaf->branch[i]->size != -1)
			{
				if (!execute_redir(data, leaf->branch[i]))
				{
					data->exit_status = 1;
					return (0);
				}
			}
			i++;
		}
		data->exit_status = 0;
		redir_cleanup_fd(data);
	}
	return (1);
}

int	exec_here_doc(t_data *data, t_tree *leaf)
{
	int		i;
	char	**temp;
	int		size;
	int		len;

	i = 3;
	temp = leaf->token->buf;
	if (!temp)
		return (0);
	size = ft_split_size(temp);
	while (i < size - 1)
	{
		len = ft_strlen(temp[i]);
		if (len && temp[i][len - 1] != '\n')
			ft_printf_fd(data->exec.fd[1], "%s\n", temp[i]);
		else
			ft_printf_fd(data->exec.fd[1], "%s", temp[i]);
		i++;
	}
	close(data->exec.fd[1]);
	data->exec.fd[1] = -1;
	return (1);
}
