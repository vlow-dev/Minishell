/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   braces_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:10:42 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:00:43 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

char	**brace_extract(char *str, int flag);

int	find_open_pair(const char *str, char c)
{
	int		i;

	if (!str || !*str)
		return (-1);
	i = 0;
	while (str[i] && !(str[i] == c && !ft_isesc_i(str, i)))
		i++;
	if (!str[i])
		return (-1);
	return (i);
}

int	find_close_pair(const char *str, char c, int open)
{
	int		i;

	if (!str || !*str)
		return (-1);
	i = ft_strlen(str) - 1;
	while (i > open && str[i] && !(str[i] == c && !ft_isesc_i(str, i)))
		i--;
	if (!str[i] || str[i] != c)
		return (-1);
	return (i);
}

static int	valid_braces(const char *str)
{
	int	open;
	int	close;

	open = find_open_pair(str, '{');
	if (open == -1)
		return (0);
	close = find_close_pair(str, '}', open);
	if (close == -1)
		return (0);
	return (1);
}

int	brace_expansion(t_tree *leaf)
{
	char	**new;
	char	**temp;

	new = NULL;
	temp = leaf->token->buf;
	while (*temp)
	{
		if (valid_braces(*temp) && expansion_inquotes(*temp, "{}"))
		{
			new = split_free_join(new, brace_extract(*temp, 0), 1);
			if (!new)
				return (0);
		}
		else
		{
			new = split_free_append(new, *temp, 0);
			if (!new)
				return (0);
		}
		temp++;
	}
	split_free(leaf->token->buf);
	leaf->token->buf = new;
	return (1);
}
