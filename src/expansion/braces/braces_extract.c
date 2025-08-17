/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   braces_extract.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:13:10 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:00:50 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

char	**brace_extract(char *str, int flag);
char	**brace_sequence(char *str);
int		brace_is_seq(char *str);
char	**brace_commas(char *str);
int		find_close_pair(const char *str, char c, int open);
int		find_open_pair(const char *str, char c);

static char	*braces_cat(char *prefix, char *inner, char *suffix)
{
	char	*result;
	char	*temp;

	temp = ft_strjoin(prefix, inner);
	if (!temp)
		return (NULL);
	result = ft_strjoin_free(temp, suffix, 0);
	if (!result)
		return (NULL);
	return (result);
}

static char	**brace_assign(char **arr, char *prefix, char *suffix)
{
	char	**new;
	char	**temp;
	char	**result;

	if (!arr)
		return (free(prefix), free(suffix), NULL);
	new = NULL;
	result = NULL;
	temp = arr;
	while (*temp)
	{
		new = brace_extract(braces_cat(prefix, *temp, suffix), 1);
		if (!new)
			return (split_free(arr), free(prefix), free(suffix), NULL);
		result = split_free_join(result, new, 1);
		if (!result)
			return (split_free(arr), free(prefix), free(suffix), NULL);
		new = NULL;
		temp++;
	}
	return (split_free(arr), free(prefix), free(suffix), result);
}

static char	**brace_separate(char *str, int open, int close, int flag)
{
	char	*prefix;
	char	*inner;
	char	*suffix;
	char	**new;

	new = NULL;
	prefix = ft_substr(str, 0, open);
	inner = ft_substr(str, open + 1, close - open - 1);
	suffix = ft_strdup(&str[close + 1]);
	if (!prefix || !inner || !suffix)
	{
		if (flag)
			free(str);
		return (ft_free(prefix), ft_free(inner), ft_free(suffix), NULL);
	}
	if (brace_is_seq(inner))
		new = brace_sequence(inner);
	else
		new = brace_commas(inner);
	if (flag)
		free(str);
	return (brace_assign(new, prefix, suffix));
}

char	**brace_extract(char *str, int flag)
{
	int		open;
	int		close;
	char	**new;

	new = NULL;
	open = find_open_pair(str, '{');
	if (open == -1)
	{
		new = split_free_append(new, str, flag);
		if (!new)
			return (NULL);
		return (new);
	}
	close = find_close_pair(str, '}', open);
	if (close == -1)
	{
		new = split_free_append(new, str, flag);
		if (!new)
			return (NULL);
		return (new);
	}
	new = brace_separate(str, open, close, flag);
	if (!new)
		return (NULL);
	return (new);
}
