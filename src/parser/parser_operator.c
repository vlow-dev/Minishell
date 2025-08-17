/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_operator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:41:40 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:38:37 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

t_tree	*grow_branch(t_token *token);
int		grow_leaf(t_tree *root, t_tree *leaf);
int		leaf_type(t_token *leaf);

static int	process_branch_pipe(t_data *data, t_tree **branch,
								t_tree **leaf, t_token **token)
{
	data->i_token++;
	*leaf = parse_cmd(data);
	if (!*leaf)
	{
		if (!get_token(data) && !operator_prompt(data))
		{
			free_tree(*branch);
			if (g_signal)
				return (0);
			clear_data(data);
			exit (2);
		}
		*leaf = parse_cmd(data);
		if (!*leaf)
			return (free_tree(*branch), 0);
	}
	if (!grow_leaf(*branch, *leaf))
		return (free_tree(*branch), free_tree(*leaf), 0);
	*token = get_token(data);
	return (1);
}

t_tree	*parse_pipe(t_data *data)
{
	t_token	*token;
	t_tree	*leaf;
	t_tree	*branch;

	leaf = parse_cmd(data);
	if (!leaf)
		return (NULL);
	branch = leaf;
	token = get_token(data);
	if (token && token->type == TOKEN_PIPE)
	{
		branch = grow_branch(token);
		if (!branch)
			return (free_tree(leaf), NULL);
		if (!grow_leaf(branch, leaf))
			return (free_tree(branch), free_tree(leaf), NULL);
		while (token && token->type == TOKEN_PIPE)
		{
			if (!process_branch_pipe(data, &branch, &leaf, &token))
				return (0);
		}
	}
	return (branch);
}

static int	process_branch_op(t_data *data, t_tree **branch,
							t_tree **leaf, t_token **token)
{
	*branch = grow_branch(*token);
	if (!*branch)
		return (free_tree(*leaf), 0);
	if (!grow_leaf(*branch, *leaf))
		return (free_tree(*branch), free_tree(*leaf), 0);
	data->i_token++;
	*leaf = parse_pipe(data);
	if (!*leaf)
	{
		if (!operator_prompt(data))
		{
			free_tree(*branch);
			if (g_signal)
				return (0);
			clear_data(data);
			exit (2);
		}
		*leaf = parse_pipe(data);
		if (!*leaf)
			return (free_tree(*branch), 0);
	}
	if (!grow_leaf(*branch, *leaf))
		return (free_tree(*branch), free_tree(*leaf), 0);
	*leaf = *branch;
	return (1);
}

t_tree	*parse_ctrl_op(t_data *data)
{
	t_token	*token;
	t_tree	*leaf;
	t_tree	*branch;

	leaf = parse_pipe(data);
	if (!leaf)
		return (NULL);
	branch = leaf;
	token = get_token(data);
	while (token && leaf_type(token) == 3)
	{
		if (!process_branch_op(data, &branch, &leaf, &token))
			return (NULL);
		token = get_token(data);
	}
	return (branch);
}
