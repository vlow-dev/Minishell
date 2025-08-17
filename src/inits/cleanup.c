/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 00:06:11 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:06:40 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void	free_nl(t_data *data)
{
	if (data->nl)
	{
		if (data->nl->token)
		{
			split_free(data->nl->token->buf);
			free(data->nl->token);
			data->nl->token = NULL;
		}
		free_tree(data->nl);
		data->nl = NULL;
	}
}

void	clear_data(t_data *data)
{
	free_token(data);
	if (data->root)
		free_tree(data->root);
	split_free(data->envp.arr);
	split_free(data->envp.paths);
	ft_free(data->envp.cmd_ep);
	if (data->pipe.pid)
		ft_lstclear(&data->pipe.pid, free);
	if (data->pid)
		ft_lstclear(&data->pid, free);
	free_nl(data);
	ft_free(data->line);
	rl_clear_history();
}
