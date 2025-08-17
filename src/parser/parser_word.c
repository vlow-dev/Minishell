/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:46:52 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:42:54 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

t_tree	*grow_branch(t_token *token);
int		grow_leaf(t_tree *root, t_tree *leaf);

static int	process_branch_word(t_data *data, t_tree **branch,
							t_tree **leaf, t_token **token)
{
	*leaf = parse_redir(data);
	if (!*leaf)
		return (free_tree(*branch), 0);
	if (!grow_leaf(*branch, *leaf))
		return (free_tree(*branch), free_tree(*leaf), 0);
	*token = get_token(data);
	return (1);
}

t_tree	*parse_word(t_data *data)
{
	t_token	*token;
	t_tree	*leaf;
	t_tree	*branch;

	branch = NULL;
	token = get_token(data);
	if (!token)
		return (NULL);
	if (token && token->type == TOKEN_WORD)
	{
		branch = grow_branch(token);
		if (!branch)
			return (NULL);
		data->i_token++;
		token = get_token(data);
		while (token && token->type <= TOKEN_REDIR_OUT)
		{
			if (!process_branch_word(data, &branch, &leaf, &token))
				return (NULL);
		}
	}
	return (branch);
}
