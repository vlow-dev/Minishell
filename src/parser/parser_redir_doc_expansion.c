/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_doc_expansion.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:54:44 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 11:08:29 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int		doc_quoted(char *str);
int		sh_doc_expansion(t_data *data, t_tree *leaf);

static int	doc_rewrite(t_tree *leaf, char **saved)
{
	char	**temp;
	int		i;

	i = 3;
	temp = leaf->token->buf;
	while (*temp)
	{
		free(saved[i]);
		saved[i] = ft_strdup(*temp);
		if (!saved[i])
		{
			split_free(saved);
			ft_printf_fd(2, "Minishell: heredoc: %s\n", strerror(ENOMEM));
			return (0);
		}
		i++;
		temp++;
	}
	split_free(leaf->token->buf);
	leaf->token->buf = saved;
	return (1);
}

static int	doc_expand(t_data *data, t_tree *leaf, char **new)
{
	char	**saved_buf;

	saved_buf = leaf->token->buf;
	leaf->token->buf = new;
	ft_subs_sp(leaf->token->buf);
	if (!sh_doc_expansion(data, leaf))
	{
		ft_printf_fd(2, "Minishell: heredoc: unexpected expansion error\n");
		return (0);
	}
	ft_subs_resp(leaf->token->buf);
	if (!doc_rewrite(leaf, saved_buf))
		return (0);
	return (1);
}

static int	doc_extract(t_data *data, t_tree *leaf)
{
	char	**new;
	int		i;
	int		size;

	i = 3;
	new = NULL;
	size = ft_split_size(leaf->token->buf);
	while (i < size)
	{
		new = split_free_append(new, leaf->token->buf[i], 0);
		if (!new)
		{
			ft_printf_fd(2, "Minishell: heredoc: %s\n", strerror(ENOMEM));
			return (0);
		}
		i++;
	}
	if (!doc_expand(data, leaf, new))
		return (0);
	return (1);
}

static int	doc_convert(t_data *data, t_tree *leaf)
{
	t_token_type	saved;

	saved = leaf->token->type;
	leaf->token->type = TOKEN_NULL;
	if (!doc_extract(data, leaf))
	{
		leaf->token->type = saved;
		return (0);
	}
	leaf->token->type = saved;
	return (1);
}

int	doc_expansion(t_data *data, t_tree *leaf)
{
	char	**temp;

	if (leaf->token->type != TOKEN_HEREDOC)
		return (1);
	temp = leaf->token->buf;
	if (!doc_quoted(temp[2]))
	{
		if (!doc_convert(data, leaf))
			return (0);
	}
	return (1);
}
