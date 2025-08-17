/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_seq_bg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:39:31 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:41:27 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

t_tree	*grow_branch(t_token *token);
int		grow_leaf(t_tree *root, t_tree *leaf);
int		leaf_type(t_token *leaf);

static int	process_branch_seq(t_data *data, t_tree **branch,
							t_tree **leaf, t_token **token)
{
	*branch = grow_branch(*token);
	if (!*branch)
		return (free_tree(*leaf), 0);
	if (!grow_leaf(*branch, *leaf))
		return (free_tree(*branch), free_tree(*leaf), 0);
	while (*token && leaf_type(*token) == 4)
	{
		data->i_token++;
		*leaf = parse_ctrl_op(data);
		if (!*leaf)
			return (1);
		if (!grow_leaf(*branch, *leaf))
			return (free_tree(*branch), free_tree(*leaf), 0);
		*token = get_token(data);
	}
	return (1);
}

t_tree	*parse_seq(t_data *data)
{
	t_token	*token;
	t_tree	*leaf;
	t_tree	*branch;

	leaf = parse_ctrl_op(data);
	if (!leaf)
		return (NULL);
	branch = leaf;
	token = get_token(data);
	if (token && leaf_type(token) == 4)
	{
		if (!process_branch_seq(data, &branch, &leaf, &token))
			return (NULL);
	}
	return (branch);
}

static int	process_branch_bg(t_data *data, t_tree **branch,
							t_tree **leaf, t_token **token)
{
	*branch = grow_branch(*token);
	if (!*branch)
		return (free_tree(*leaf), 0);
	if (!grow_leaf(*branch, *leaf))
		return (free_tree(*branch), free_tree(*leaf), 0);
	data->i_token++;
	*leaf = parse_seq(data);
	if (!*leaf)
		return (free_tree(*branch), 0);
	if (!grow_leaf(*branch, *leaf))
		return (free_tree(*branch), free_tree(*leaf), 0);
	*leaf = *branch;
	*token = get_token(data);
	return (1);
}

t_tree	*parse_bg(t_data *data)
{
	t_token	*token;
	t_tree	*leaf;
	t_tree	*branch;

	leaf = parse_seq(data);
	if (!leaf)
		return (NULL);
	branch = leaf;
	token = get_token(data);
	while (token && leaf_type(token) == 5)
	{
		if (!process_branch_bg(data, &branch, &leaf, &token))
			return (NULL);
	}
	return (branch);
}
