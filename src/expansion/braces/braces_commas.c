/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   braces_commas.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:21:08 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:22:29 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

static int	brace_commas_append(char ***arr, char *str, int *i, int *j)
{
	int		nested;

	nested = 0;
	while (str[*i])
	{
		if (str[*i] == '{')
			nested++;
		if (str[*i] == '}')
			nested--;
		if (str[*i] == ',' && !nested)
		{
			*arr = split_free_append(*arr, ft_substr(str, *j, *i - *j), 1);
			if (!*arr)
				return (0);
			(*i)++;
			*j = *i;
		}
		else
			(*i)++;
	}
	return (1);
}

char	**brace_commas(char *str)
{
	char	**new;
	int		i;
	int		j;

	new = NULL;
	i = 0;
	j = 0;
	if (!brace_commas_append(&new, str, &i, &j))
		return (free(str), NULL);
	if (j < i)
	{
		new = split_free_append(new, ft_strdup(&str[j]), 1);
		if (!new)
			return (free(str), NULL);
	}
	return (free(str), new);
}
