/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 14:18:43 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:34:30 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

int		execute_word(t_data *data, t_tree *leaf);
int		execute_pipe(t_data *data, t_tree *leaf);
int		execute_logical(t_data *data, t_tree *leaf);
int		execute_sequence(t_data *data, t_tree *leaf);
int		execute_subshell(t_data *data, t_tree *leaf);
int		parent_redir(t_data *data, t_tree *leaf);

int	execution(t_data *data)
{
	ft_lstclear(&data->pid, free);
	execute_tree(data, data->root);
	pid_wait(data);
	return (1);
}

int	execute_tree(t_data *data, t_tree *root)
{
	if (!root)
		return (0);
	if (!root->token && root->size != 0)
		return (parent_redir(data, root));
	if (root->token->type == TOKEN_WORD)
		return (execute_word(data, root));
	if (root->token->type == TOKEN_PIPE)
		return (execute_pipe(data, root));
	if (root->token->type == TOKEN_OR || root->token->type == TOKEN_AND)
		return (execute_logical(data, root));
	if (root->token->type == TOKEN_PAREN_OPEN
		|| root->token->type == TOKEN_BRACKET_OPEN)
		return (execute_subshell(data, root));
	if (root->token->type == TOKEN_SEMICOLON
		|| root->token->type == TOKEN_NEWLINE)
		return (execute_sequence(data, root));
	return (1);
}

int	ft_3min(int a, int b, int c)
{
	if (a <= b && a <= c)
		return (a);
	if (b <= a && b <= c)
		return (b);
	return (c);
}
