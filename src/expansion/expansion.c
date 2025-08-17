/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 21:03:58 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:05:07 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int		brace_expansion(t_tree *tree);
int		tilde_expansion(t_data *data, t_tree *leaf);
int		sign_expansion(t_data *data, t_tree *leaf);
int		glob_expansion(t_tree *leaf);
int		quotes_removal(t_data *data, t_tree *leaf);

int	sh_expansion(t_data *data, t_tree *leaf)
{
	if ((leaf->token->type == TOKEN_HERESTR
			|| leaf->token->type == TOKEN_HEREDOC))
	{
		if (ft_strchr(leaf->token->buf[2], '\'')
			|| ft_strchr(leaf->token->buf[2], '\"'))
			quotes_memmove(leaf->token->buf[2]);
		return (1);
	}
	else if (leaf->token->buf)
	{
		brace_expansion(leaf);
		tilde_expansion(data, leaf);
		sign_expansion(data, leaf);
		glob_expansion(leaf);
		quotes_removal(data, leaf);
		return (1);
	}
	return (0);
}

int	sh_doc_expansion(t_data *data, t_tree *leaf)
{
	if ((leaf->token->type == TOKEN_HERESTR
			|| leaf->token->type == TOKEN_HEREDOC))
	{
		if (ft_strchr(leaf->token->buf[2], '\'')
			|| ft_strchr(leaf->token->buf[2], '\"'))
			quotes_memmove(leaf->token->buf[2]);
		return (1);
	}
	else if (leaf->token->buf)
	{
		brace_expansion(leaf);
		tilde_expansion(data, leaf);
		sign_expansion(data, leaf);
		glob_expansion(leaf);
		return (1);
	}
	return (0);
}
