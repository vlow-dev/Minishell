/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:59:26 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:03:43 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

static int	buf_nl_append(t_data *data, t_token *token, int size)
{
	char	**nl;
	int		i;

	nl = data->nl->token->buf;
	i = 1;
	while (i <= size)
	{
		token->buf = split_free_append(token->buf,
				ft_strjoin_free(nl[i], "\n", 0), 1);
		if (!token->buf)
			return (0);
		i++;
	}
	while (size)
	{
		ft_memmove(&data->nl->token->buf[size], &data->nl->token->buf[size + 1],
			(ft_split_size(&data->nl->token->buf[size + 1]) + 1)
			* sizeof(char *));
		size--;
	}
	return (1);
}

int	doc_quoted(char *str)
{
	char	*ptr;

	ptr = str;
	while (*ptr && !ft_strchr(M_QUOTES, *ptr))
		ptr++;
	if (*ptr && ft_strchr(M_QUOTES, *ptr))
		ptr = skip_quotes(ptr);
	if (*ptr && ft_strchr(M_QUOTES, *ptr))
		return (1);
	return (0);
}

static int	buf_nl_check(t_data *data, t_token *token)
{
	int		i;
	char	**nl;
	char	*ptr;

	if (!data->nl)
		return (0);
	i = 0;
	nl = data->nl->token->buf;
	ptr = ft_strdup(token->buf[2]);
	if (!ptr)
		return (0);
	if (doc_quoted(ptr))
		quotes_memmove(ptr);
	while (nl[i])
	{
		if (*nl[i] && !ft_strcmp(ptr, nl[i]))
			break ;
		i++;
	}
	free(ptr);
	if (!buf_nl_append(data, token, i))
		return (0);
	if (!nl || !*nl)
		return (0);
	return (1);
}

int	isvalid_buf(t_data *data, t_token *token)
{
	int	i;
	int	size;

	i = 3;
	size = ft_split_size(token->buf);
	if (token->type == TOKEN_HERESTR && size == 3)
		return (1);
	if (size <= 3)
		return (buf_nl_check(data, token));
	while (token->buf[i] && ft_strcmp(token->buf[2], token->buf[i]))
		i++;
	if (!token->buf[i])
		return (0);
	return (1);
}
