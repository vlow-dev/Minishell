/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:31:35 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 21:36:30 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

int	branch_type(t_tree *leaf)
{
	if (leaf->token->type == TOKEN_WORD
		|| (leaf->token->type >= TOKEN_PAREN_OPEN
			&& leaf->token->type <= TOKEN_BRACKET_CLOSE))
		return (0);
	else if (leaf->token->type >= TOKEN_HERESTR
		&& leaf->token->type <= TOKEN_REDIR_OUT)
		return (1);
	else if (leaf->token->type == TOKEN_PIPE)
		return (2);
	else if (leaf->token->type >= TOKEN_OR && leaf->token->type <= TOKEN_AND)
		return (3);
	else if (leaf->token->type >= TOKEN_SEMICOLON
		&& leaf->token->type <= TOKEN_NEWLINE)
		return (4);
	return (5);
}

int	leaf_type(t_token *leaf)
{
	if (leaf->type == TOKEN_WORD || (leaf->type >= TOKEN_PAREN_OPEN
			&& leaf->type <= TOKEN_BRACKET_CLOSE))
		return (TOKEN_WORD);
	else if (leaf->type >= TOKEN_HERESTR && leaf->type <= TOKEN_REDIR_OUT)
		return (1);
	else if (leaf->type == TOKEN_PIPE)
		return (2);
	else if (leaf->type >= TOKEN_OR && leaf->type <= TOKEN_AND)
		return (3);
	else if (leaf->type >= TOKEN_SEMICOLON && leaf->type <= TOKEN_NEWLINE)
		return (4);
	return (5);
}

t_tree	*grow_tree_leaf(t_token *token)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_tree));
	node->token = token;
	return (node);
}

void	free_tree(t_tree *root)
{
	int	i;

	if (!root)
		return ;
	i = 0;
	if (root->size == -1)
	{
		if (root->token)
		{
			split_free(root->token->buf);
			free(root->token);
		}
		return (free(root));
	}
	while (i < root->size)
	{
		free_tree(root->branch[i]);
		i++;
	}
	if (root->branch)
		free(root->branch);
	free(root);
	root = NULL;
}
