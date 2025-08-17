/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_grouping.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:11:44 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 21:12:09 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int	check_grouping(char *temp)
{
	if (*temp && *(temp + 1) && ft_strchr(T_BRACES_OPEN, *temp)
		&& ft_isspace(*(temp + 1)))
	{
		if (ft_strchr(temp, ';') || ft_strchr(temp, '\n'))
			return (1);
	}
	return (0);
}

static int	process_literal_groups(char ***arr, char **iptr)
{
	char	*jptr;

	jptr = *iptr;
	while (**iptr && !ft_strchr(M_WSPACE T_BRACES_OPEN T_BRACES_CLOSE, **iptr))
		(*iptr)++;
	if (**iptr && ft_strchr(T_BRACES_OPEN T_BRACES_CLOSE, **iptr))
	{
		*arr = split_free_append(*arr, ft_substr(jptr, 0, *iptr - jptr), 1);
		if (!*arr)
			return (0);
		return (1);
	}
	else if (*iptr)
		*arr = split_free_append(*arr, ft_substr(jptr, 0, *iptr - jptr), 1);
	else
		*arr = split_free_append(*arr, ft_strdup(jptr), 1);
	if (!*arr)
		return (0);
	return (1);
}

static int	process_quotes_groups(char ***arr, char **iptr)
{
	char	*jptr;

	jptr = *iptr;
	*iptr = skip_quotes(*iptr);
	*arr = split_free_append(*arr, ft_substr(jptr, 0, *iptr - jptr + 1), 1);
	if (!*arr)
		return (0);
	return (1);
}

static int	process_valid_groups(char ***arr, char *str)
{
	char	*iptr;

	iptr = str;
	while (*iptr)
	{
		iptr = skip_wspaces(iptr);
		if (ft_strchr(T_BRACES_OPEN T_BRACES_CLOSE, *iptr))
		{
			*arr = split_free_append(*arr, ft_substr(iptr, 0, 1), 1);
			if (!*arr)
				return (0);
		}
		else if (ft_strchr(M_QUOTES, *iptr))
		{
			if (!process_quotes_groups(arr, &iptr))
				return (0);
		}
		else
		{
			if (!process_literal_groups(arr, &iptr))
				return (0);
		}
		iptr++;
	}
	return (1);
}

char	**normalize_grouping(char **pre_token)
{
	char	**temp;
	char	**new;

	if (!pre_token)
		return (0);
	temp = pre_token;
	new = NULL;
	while (*temp)
	{
		if (check_grouping(*temp))
		{
			if (!process_valid_groups(&new, *temp))
				return (split_free(new), split_free(pre_token), NULL);
		}
		else
		{
			new = split_free_append(new, ft_strdup(*temp), 1);
			if (!new)
				return (split_free(new), split_free(pre_token), NULL);
		}
		temp++;
	}
	return (split_free(pre_token), new);
}
