/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_words2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:29:41 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:14:55 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdio.h>

int		execute_redir(t_data *data, t_tree *leaf);
int		execute_builtins(t_data *data, char **cmds);

static int	word_bi_cleanup_fd(t_data *data)
{
	if (dup2(data->pipe.base_fd[0], STDIN_FILENO) == -1)
		perror("dup2 stdin restore failed");
	if (dup2(data->pipe.base_fd[1], STDOUT_FILENO) == -1)
		perror("dup2 stdout restore failed");
	if (dup2(data->pipe.base_fd[2], STDERR_FILENO) == -1)
		perror("dup2 stderr restore failed");
	close(data->pipe.base_fd[0]);
	close(data->pipe.base_fd[1]);
	close(data->pipe.base_fd[2]);
	return (1);
}

int	word_builtins(t_data *data, t_tree *leaf)
{
	int	i;

	i = 0;
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
				return (1);
			}
		}
		i++;
	}
	data->exit_status = execute_builtins(data, leaf->token->buf);
	word_bi_cleanup_fd(data);
	return (1);
}
