/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:03:18 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:13:24 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

static int	add_token(t_data *data, char **buf, t_token_type token_type);
int			token_type(const char *temp);

int	create_token(t_data *data, char **pre_token)
{
	char	**temp;
	char	**token_buf;

	if (!pre_token)
		return (0);
	temp = pre_token;
	while (*temp)
	{
		token_buf = NULL;
		token_buf = split_free_append(token_buf, *temp, 0);
		if (!token_buf)
			return (0);
		if (!add_token(data, token_buf, token_type(*temp)))
			return (0);
		token_buf = NULL;
		temp++;
	}
	split_free(pre_token);
	return (1);
}

static int	token_group_check(char **buf, t_token_type token_type)
{
	if (token_type != TOKEN_BRACKET_OPEN && token_type != TOKEN_BRACKET_CLOSE)
		return (1);
	if (token_type == TOKEN_BRACKET_OPEN)
	{
		if (!ft_strcmp(*buf, T_BRACES_OPEN))
			return (1);
	}
	else if (token_type == TOKEN_BRACKET_CLOSE)
	{
		if (!ft_strcmp(*buf, T_BRACES_CLOSE))
			return (1);
	}
	return (0);
}

static int	add_head_token(t_data *data, char **buf, t_token_type token_type)
{
	t_token	*token;
	int		id;

	id = 0;
	data->token = malloc(sizeof(t_token));
	if (!data->token)
		return (0);
	token = data->token;
	ft_memset(token, 0, sizeof(t_token));
	token->id = id + 1;
	token->type = token_type;
	if (token_type >= TOKEN_WORD && token_type <= TOKEN_REDIR_OUT)
		token->buf = buf;
	else
		split_free(buf);
	return (1);
}

static int	add_child_token(t_data *data, char **buf, t_token_type token_type)
{
	t_token	*token;
	int		id;

	id = 0;
	token = data->token;
	while (token->next)
		token = token->next;
	id = token->id;
	token->next = malloc(sizeof(t_token));
	if (!token->next)
		return (0);
	token = token->next;
	ft_memset(token, 0, sizeof(t_token));
	token->id = id + 1;
	token->type = token_type;
	if (token_type >= TOKEN_WORD && token_type <= TOKEN_REDIR_OUT)
		token->buf = buf;
	else
		split_free(buf);
	return (1);
}

static int	add_token(t_data *data, char **buf, t_token_type token_type)
{
	if (!token_group_check(buf, token_type))
		token_type = TOKEN_WORD;
	if (!data->token)
		add_head_token(data, buf, token_type);
	else
		add_child_token(data, buf, token_type);
	return (1);
}
