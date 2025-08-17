/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:28:46 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 21:29:31 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

t_token	*get_token(t_data *data)
{
	t_token	*temp;
	int		i;

	i = 0;
	temp = data->token;
	while (i++ < data->i_token && temp)
		temp = temp->next;
	return (temp);
}

t_tree	*grow_branch(t_token *token)
{
	t_tree	*leaf;

	leaf = malloc(sizeof(t_tree));
	if (!leaf)
		return (NULL);
	ft_memset(leaf, 0, sizeof(t_tree));
	leaf->token = token;
	return (leaf);
}

t_tree	*empty_branch(void)
{
	t_tree	*leaf;

	leaf = malloc(sizeof(t_tree));
	if (!leaf)
		return (NULL);
	ft_memset(leaf, 0, sizeof(t_tree));
	leaf->size = -1;
	return (leaf);
}

int	grow_leaf(t_tree *root, t_tree *leaf)
{
	t_tree	**temp;
	int		i;

	i = 0;
	temp = malloc(sizeof(t_tree *) * (root->size + 2));
	if (!temp)
		return (0);
	ft_memset(temp, 0, sizeof(t_tree *) * (root->size + 2));
	while (root->branch && i < root->size)
	{
		temp[i] = root->branch[i];
		i++;
	}
	temp[i] = leaf;
	free(root->branch);
	root->branch = temp;
	root->size++;
	return (1);
}

t_tree	*grow_branch_with_leaf(t_tree *leaf, t_token *token)
{
	t_tree	*branch;

	branch = grow_branch(token);
	if (!branch)
		return (free_tree(leaf), NULL);
	if (!grow_leaf(branch, leaf))
		return (free_tree(branch), free_tree(leaf), NULL);
	return (branch);
}
