/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell_seq.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:12:10 by vlow              #+#    #+#             */
/*   Updated: 2025/04/24 16:47:36 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include "errno.h"
#include <string.h>

int	execute_sequence(t_data *data, t_tree *leaf)
{
	int	i;

	i = 0;
	while (leaf->branch[i] && leaf->branch[i]->size != -1)
	{
		execute_tree(data, leaf->branch[i]);
		pid_wait(data);
		if (g_signal)
			break ;
		i++;
	}
	return (0);
}

static int	ss_child_setfd(t_data *data)
{
	if (data->pipe.size > 0)
	{
		if (data->pipe.fd[1] != -1)
		{
			dup2(data->pipe.fd[1], STDOUT_FILENO);
			close(data->pipe.fd[1]);
			data->pipe.fd[1] = -1;
		}
		else
			close(data->pipe.child[1]);
		if (data->pipe.fd[0] != -1)
		{
			dup2(data->pipe.fd[0], STDIN_FILENO);
			close(data->pipe.fd[0]);
			data->pipe.fd[0] = -1;
		}
		else
			close(data->pipe.child[0]);
	}
	return (1);
}

static int	process_subshell_child(t_data *data, t_tree *leaf)
{
	int	i;

	i = 0;
	signal_set(data, SIGINT, SIG_DFL);
	signal_set(data, SIGQUIT, SIG_DFL);
	ss_child_setfd(data);
	while (i < leaf->size && leaf->branch[i])
	{
		if (leaf->branch[i]->size != -1
			&& (leaf->branch[i]->token->type != TOKEN_PAREN_CLOSE
				&& leaf->branch[i]->token->type != TOKEN_BRACKET_CLOSE))
		{
			ft_lstclear(&data->pid, free);
			execute_tree(data, leaf->branch[i]);
			pid_wait(data);
		}
		i++;
	}
	exit(data->exit_status);
	return (1);
}

int	execute_subshell(t_data *data, t_tree *leaf)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_printf_fd(2, "Minishell: Fork: %s\n", strerror(errno));
		return (0);
	}
	if (!pid)
		process_subshell_child(data, leaf);
	signal_set(data, SIGINT, SIG_IGN);
	if (!add_pid(data, pid))
		return (0);
	return (1);
}
