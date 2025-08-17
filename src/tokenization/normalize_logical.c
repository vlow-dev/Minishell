/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_logical.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:12:35 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:44:46 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int		token_type(const char *temp);

static int	token_len(int type)
{
	const char	*symbols[] = {NULL,
		T_HERESTR, T_REDIR_IDUP, T_REDIR_ODUP, T_REDIR_RW, T_HEREDOC, T_APPEND,
		T_REDIR_IN, T_REDIR_OUT, T_OR, T_AND, T_PIPE,
		T_PAREN_OPEN, T_PAREN_CLOSE, T_BRACES_OPEN, T_BRACES_CLOSE,
		T_BACKGROUND, T_SEMICOLON, T_NEWLINE,
		NULL};
	int			i;

	i = ft_strlen(symbols[type]);
	if (!type)
		return (0);
	return (i);
}

static int	process_literal_logical(char ***arr, char **iptr, char **jptr)
{
	if (*jptr != *iptr)
	{
		*arr = split_free_append(*arr, ft_substr(*jptr, 0, *iptr - *jptr), 1);
		if (!*arr)
			return (0);
	}
	*jptr = *iptr;
	if (**iptr && token_type(*iptr))
	{
		(*iptr) += token_len(token_type(*iptr));
		*arr = split_free_append(*arr, ft_substr(*jptr, 0, *iptr - *jptr), 1);
		if (!*arr)
			return (0);
	}
	return (1);
}

static int	process_first_part(char ***arr, char **iptr, char **jptr)
{
	if (ft_strchr(M_REDIR_OP, **iptr) && (token_type(*iptr) > TOKEN_WORD
			&& token_type(*iptr) <= TOKEN_REDIR_OUT))
	{
		if (token_type(*iptr) >= TOKEN_REDIR_IDUP
			&& token_type(*iptr) <= TOKEN_REDIR_RW)
		{
			*iptr += 2;
			while (**iptr && (!ft_strchr(M_WSPACE, **iptr)
					&& !ft_strchr(M_REDIR_OP, **iptr)))
				(*iptr)++;
			if (!process_literal_logical(arr, &*iptr, &*jptr))
				return (0);
		}
		else
		{
			*iptr += token_len(token_type(*iptr));
			*arr = split_free_append(*arr,
					ft_substr(*jptr, 0, *iptr - *jptr), 1);
			if (!*arr)
				return (0);
		}
		*iptr = skip_wspaces(*iptr);
		*jptr = *iptr;
	}
	return (1);
}

static int	process_valid_logical(char ***arr, char *str)
{
	char	*iptr;
	char	*jptr;

	iptr = str;
	while (*iptr)
	{
		iptr = skip_wspaces(iptr);
		jptr = iptr;
		while (*iptr && ft_isdigit(*iptr))
			iptr++;
		if (!process_first_part(arr, &iptr, &jptr))
			return (0);
		while (*iptr && (token_type(iptr) == TOKEN_WORD
				|| (token_type(iptr) >= TOKEN_PAREN_OPEN
					&& token_type(iptr) <= TOKEN_BACKGROUND)))
		{
			if (ft_strchr(M_QUOTES, *iptr) && !ft_isesc_ptr(jptr, iptr))
				iptr = skip_quotes(iptr);
			iptr++;
		}
		if (!process_literal_logical(arr, &iptr, &jptr))
			return (0);
	}
	return (1);
}

char	**normalize_logical(char **pre_token)
{
	char	**temp;
	char	**new;

	if (!pre_token)
		return (0);
	temp = pre_token;
	new = NULL;
	while (*temp)
	{
		if (!process_valid_logical(&new, *temp))
			return (split_free(new), split_free(pre_token), NULL);
		temp++;
	}
	return (split_free(pre_token), new);
}
