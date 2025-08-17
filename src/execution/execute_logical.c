/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_logical.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:16:01 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:16:25 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	process_or(t_data *data, t_tree *leaf)
{
	int	left;
	int	right;

	left = -1;
	right = -1;
	execute_tree(data, leaf->branch[0]);
	pid_wait(data);
	left = data->status;
	if (left)
	{
		execute_tree(data, leaf->branch[1]);
		pid_wait(data);
		right = data->status;
		return (right);
	}
	return (left);
}

static int	process_and(t_data *data, t_tree *leaf)
{
	int	left;
	int	right;

	left = -1;
	right = -1;
	execute_tree(data, leaf->branch[0]);
	pid_wait(data);
	left = data->status;
	if (!left)
	{
		execute_tree(data, leaf->branch[1]);
		pid_wait(data);
		right = data->status;
		return (right);
	}
	return (left);
}

int	execute_logical(t_data *data, t_tree *leaf)
{
	if (leaf->token->type == TOKEN_OR)
		return (process_or(data, leaf));
	else
		return (process_and(data, leaf));
	return (-1);
}
