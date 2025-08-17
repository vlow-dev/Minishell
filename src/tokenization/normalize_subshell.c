/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_subshell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:10:34 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 21:11:15 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int	extract_valid_subshell(char ***arr, char **iptr)
{
	char	*jptr;

	jptr = *iptr;
	if (ft_strchr(M_QUOTES, **iptr))
	{
		*iptr = skip_quotes(*iptr);
		*arr = split_free_append(*arr, ft_substr(jptr, 0, *iptr - jptr + 1), 1);
	}
	else
		*arr = split_free_append(*arr, ft_substr(*iptr, 0, 1), 1);
	if (!arr || !*arr)
		return (0);
	(*iptr)++;
	return (1);
}

static int	extract_literal_subshell(char ***arr, char **iptr)
{
	char	*jptr;

	jptr = *iptr;
	while (**iptr && !ft_strchr(M_WSPACE T_PAREN_OPEN T_PAREN_CLOSE, **iptr)
		&& !ft_isesc_ptr(jptr, *iptr))
		(*iptr)++;
	if (**iptr && ft_strchr(T_PAREN_OPEN T_PAREN_CLOSE, **iptr))
	{
		*arr = split_free_append(*arr, ft_substr(jptr, 0, *iptr - jptr), 1);
		if (!arr || !*arr)
			return (0);
		return (1);
	}
	else if (**iptr)
		*arr = split_free_append(*arr, ft_substr(jptr, 0, *iptr - jptr), 1);
	else
		*arr = split_free_append(*arr, ft_strdup(jptr), 1);
	if (!arr || !*arr)
		return (0);
	if (**iptr)
		(*iptr)++;
	return (1);
}

static int	subshell_valid(char *str, char ***arr)
{
	char	*iptr;

	iptr = str;
	while (*iptr)
	{
		iptr = skip_wspaces(iptr);
		if (ft_strchr(M_QUOTES T_PAREN_OPEN T_PAREN_CLOSE, *iptr)
			&& !ft_isesc_ptr(str, iptr))
		{
			if (!extract_valid_subshell(arr, &iptr))
				return (0);
		}
		else
		{
			if (!extract_literal_subshell(arr, &iptr))
				return (0);
		}
	}
	return (1);
}

char	**normalize_subshell(char **pre_token)
{
	char	**temp;
	char	**new;

	if (!pre_token)
		return (NULL);
	temp = pre_token;
	new = NULL;
	while (*temp)
	{
		if (**temp && ft_strchr(T_PAREN_OPEN, **temp))
		{
			if (!subshell_valid(*temp, &new))
				return (split_free(new), split_free(pre_token), NULL);
		}
		else
		{
			new = split_free_append(new, ft_strdup(*temp), 1);
			if (!new)
				return (NULL);
		}
		temp++;
	}
	return (split_free(pre_token), new);
}
