/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:16:39 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 12:51:25 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdio.h>

static int	pre_pipe_assign(t_data *data, int *prev_fd, int i)
{
	data->pipe.fd[0] = *prev_fd;
	if (i != data->pipe.size - 1)
		data->pipe.fd[1] = data->pipe.child[1];
	else
		data->pipe.fd[1] = -1;
	return (1);
}

static int	post_pipe_assign(t_data *data, int *prev_fd, int i)
{
	if (*prev_fd != -1)
	{
		close(*prev_fd);
		*prev_fd = -1;
		data->pipe.fd[0] = -1;
	}
	if (i != data->pipe.size - 1)
	{
		*prev_fd = data->pipe.child[0];
		close(data->pipe.child[1]);
		data->pipe.child[0] = -1;
		data->pipe.child[1] = -1;
	}
	return (1);
}

static int	pipefd_cleanup(t_data *data)
{
	if (data->pipe.fd[0] != -1)
	{
		close(data->pipe.fd[0]);
		data->pipe.fd[0] = -1;
	}
	if (data->pipe.fd[1] != -1)
	{
		close(data->pipe.fd[1]);
		data->pipe.fd[1] = -1;
	}
	return (1);
}

int	execute_pipe(t_data *data, t_tree *leaf)
{
	int	i;
	int	prev_fd;

	i = 0;
	prev_fd = -1;
	data->pipe.size = leaf->size;
	while (i < leaf->size)
	{
		if (i != data->pipe.size - 1 && pipe(data->pipe.child) == -1)
		{
			perror("Execute Pipe Error!");
			return (0);
		}
		pre_pipe_assign(data, &prev_fd, i);
		if (leaf->branch[i] && leaf->branch[i]->size != -1)
			execute_tree(data, leaf->branch[i]);
		post_pipe_assign(data, &prev_fd, i);
		i++;
	}
	pipefd_cleanup(data);
	data->pipe.size = 0;
	return (1);
}
