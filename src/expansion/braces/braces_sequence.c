/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   braces_sequence.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:15:56 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:18:35 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

int	brace_seq_isstep(char *str);

static char	**brace_seq_alpha(char *str)
{
	char	start;
	char	end;
	char	**new;
	int		step;
	char	buf[2];

	buf[1] = '\0';
	new = NULL;
	start = str[0];
	end = str[3];
	step = 1;
	if (brace_seq_isstep(str))
		step = ft_atoi((ft_strrchr(str, '.') + 1));
	while (start <= end)
	{
		buf[0] = start;
		new = split_free_append(new, ft_strdup(buf), 1);
		if (!new)
			return (free(str), NULL);
		start += step;
	}
	return (free(str), new);
}

static char	**brace_digit_desc(long int start, long int end, int step)
{
	char	**new;

	new = NULL;
	while (start >= end)
	{
		new = split_free_append(new, ft_ltoa(start), 1);
		if (!new)
			return (NULL);
		start -= step;
	}
	return (new);
}

static char	**brace_digit_asc(long int start, long int end, int step)
{
	char	**new;

	new = NULL;
	while (start <= end)
	{
		new = split_free_append(new, ft_ltoa(start), 1);
		if (!new)
			return (NULL);
		start += step;
	}
	return (new);
}

static char	**brace_seq_digit(char *str)
{
	long int		start;
	long int		end;
	int				step;
	int				i;

	i = 0;
	start = ft_atol(str);
	while (str[i] != '.')
		i++;
	i += 2;
	end = ft_atol(&str[i]);
	step = 1;
	if (brace_seq_isstep(str))
		step = ft_atoi((ft_strrchr(str, '.') + 1));
	if (start > end)
		return (free(str), brace_digit_desc(start, end, step));
	else
		return (free(str), brace_digit_asc(start, end, step));
	return (free(str), NULL);
}

char	**brace_sequence(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] != '.')
		i++;
	if ((i - j) == 1)
	{
		if (ft_isalpha(str[i - 1]))
			return (brace_seq_alpha(str));
		return (brace_seq_digit(str));
	}
	else
		return (brace_seq_digit(str));
}
