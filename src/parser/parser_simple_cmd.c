/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_simple_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:37:44 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:09:57 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

t_tree	*grow_branch(t_token *token);
int		grow_leaf(t_tree *root, t_tree *leaf);
t_tree	*empty_branch(void);

t_tree	*parse_cmd(t_data *data)
{
	t_token	*token;

	token = get_token(data);
	if (token && token->type == TOKEN_PAREN_OPEN)
		return (parse_subshell(data));
	else if (token && token->type == TOKEN_BRACKET_OPEN)
		return (parse_groups(data));
	return (parse_simple_cmd(data));
}

static void	parse_merge_child(t_tree *branch, t_tree *leaf)
{
	int	i;

	i = 0;
	while (i < leaf->size)
	{
		grow_leaf(branch, leaf->branch[i]);
		i++;
	}
	if (leaf->branch)
		free(leaf->branch);
	free(leaf);
}

static int	process_branch_sc(t_data *data, t_tree **branch,
							t_tree **leaf, t_token **token)
{
	if (*token && (*token)->type == TOKEN_WORD)
	{
		*leaf = *branch;
		*branch = grow_branch(*token);
		if (!*branch)
			return (free_tree(*leaf), 0);
		parse_merge_child(*branch, *leaf);
		data->i_token++;
	}
	else
	{
		*leaf = parse_redir(data);
		if (!*leaf)
			return (free_tree(*branch), 0);
		if (!grow_leaf(*branch, *leaf))
			return (free_tree(*branch), free_tree(*leaf), 0);
	}
	*token = get_token(data);
	return (1);
}

t_tree	*parse_simple_cmd(t_data *data)
{
	t_token	*token;
	t_tree	*leaf;
	t_tree	*branch;

	branch = NULL;
	token = get_token(data);
	if (!token || token->type > TOKEN_REDIR_OUT)
		return (branch);
	if (token && !token->type)
		branch = parse_word(data);
	else if (token && token->type)
	{
		branch = empty_branch();
		if (!branch)
			return (NULL);
		branch->size = 0;
		while (token && token->type <= TOKEN_REDIR_OUT)
		{
			if (!process_branch_sc(data, &branch, &leaf, &token))
				return (NULL);
		}
	}
	return (branch);
}
