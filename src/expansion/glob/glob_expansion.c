/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:48:43 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:55:40 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

char	**glob_expand(char **dir_list, char *str, int flag);
char	**glob_init(char *str);
int		glob_isvalid(const char *str);
int		glob_get_len(const char *str);

static char	**glob_separate_mid(char **dir_list, char *prefix, char *suffix)
{
	char	**pre_list;
	int		pre_len;
	int		i;
	char	*mid;

	i = 0;
	pre_list = NULL;
	pre_len = ft_strlen(prefix);
	while (dir_list[i])
	{
		mid = ft_strnstr(dir_list[i], prefix, ft_strlen(dir_list[i]));
		if (mid)
			pre_list = split_free_append(pre_list, mid + pre_len, 0);
		else
			pre_list = split_free_append(pre_list, M_US, 0);
		if (!pre_list)
			return (split_free(dir_list), free(prefix), free(suffix), NULL);
		i++;
	}
	split_free(dir_list);
	free(prefix);
	return (glob_expand(pre_list, suffix, 1));
}

static char	**glob_separate(char **dir_list, char *prefix, char *suffix)
{
	char	**pre_list;
	int		pre_len;
	int		i;

	i = 0;
	pre_list = NULL;
	pre_len = ft_strlen(prefix);
	while (dir_list[i])
	{
		if (!ft_strncmp(dir_list[i], prefix, pre_len))
			pre_list = split_free_append(pre_list, dir_list[i] + pre_len, 0);
		else
			pre_list = split_free_append(pre_list, M_US, 0);
		if (!pre_list)
			return (split_free(dir_list), free(prefix), free(suffix), NULL);
		i++;
	}
	split_free(dir_list);
	free(prefix);
	return (glob_expand(pre_list, suffix, 1));
}

static char	**glob_match(char **dir_list, char *suffix)
{
	int		list_len;
	int		suf_len;
	int		i;

	i = 0;
	suf_len = ft_strlen(suffix);
	while (dir_list[i])
	{
		list_len = ft_strlen(dir_list[i]);
		if (list_len < suf_len
			|| ft_strcmp(dir_list[i] + (list_len - suf_len), suffix))
		{
			free(dir_list[i]);
			dir_list[i] = ft_strdup(M_US);
			if (!dir_list[i])
				return (split_free(dir_list), free(suffix), NULL);
		}
		i++;
	}
	free(suffix);
	return (dir_list);
}

char	**glob_expand(char **dir_list, char *str, int flag)
{
	int		pre_len;
	char	*prefix;
	char	*suffix;

	if (!glob_isvalid(str))
		return (glob_match(dir_list, str));
	pre_len = glob_get_len(str);
	prefix = quotes_memmove(ft_substr(str, 0, pre_len));
	suffix = ft_strdup(&str[pre_len + 1]);
	if (!prefix || !suffix)
	{
		if (flag)
			free(str);
		return (ft_free_setn((void **)&prefix),
			ft_free_setn((void **)&suffix), NULL);
	}
	if (!pre_len)
	{
		if (flag)
			free(str);
		return (free(prefix), glob_expand(dir_list, suffix, 1));
	}
	if (flag)
		return (free(str), glob_separate_mid(dir_list, prefix, suffix));
	return (glob_separate(dir_list, prefix, suffix));
}

int	glob_expansion(t_tree *leaf)
{
	char	**temp;
	char	**new;

	new = NULL;
	temp = leaf->token->buf;
	while (*temp)
	{
		if (ft_strchr(*temp, '*') && expansion_inquotes(*temp, "*"))
			new = split_free_join(new, glob_init(*temp), 1);
		else
			new = split_free_append(new, *temp, 0);
		if (!new)
			return (0);
		temp++;
	}
	split_free(leaf->token->buf);
	leaf->token->buf = new;
	return (0);
}
