/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   braces_sequence_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:17:12 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:20:29 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int	seq_valid_digit(char *str, int *param)
{
	while (param[1] < param[0])
	{
		if (ft_issign(str[param[1]]))
			param[4]++;
		else if (!ft_isdigit(str[param[1]]))
			return (0);
		param[1]++;
	}
	if (param[4] > 1)
		return (0);
	param[4] = 0;
	return (1);
}

static int	brace_process_seq(char *str, int *param)
{
	if (!param[2] && (param[0] - param[1]) > 1)
	{
		if (!seq_valid_digit(str, param))
			return (0);
	}
	else if (!param[2] && (param[0] - param[1]) == 1)
	{
		if (ft_isalpha(str[param[1]]))
			param[3]++;
		else if (!ft_isalnum(str[param[1]]))
			return (0);
	}
	else if (param[3] && (param[0] - param[1]) == 1)
	{
		if (!ft_isalpha(str[param[1]]))
			return (0);
	}
	else if (param[2])
		if (!seq_valid_digit(str, param))
			return (0);
	param[0] += 2;
	param[1] = param[0];
	param[2]++;
	return (1);
}

static int	brace_process_step(char *str, int *param)
{
	while (param[1] < param[0])
	{
		if (!ft_isdigit(str[param[1]]))
			return (0);
		param[1]++;
	}
	return (1);
}

// param = i, j, count, ischar, sign
int	brace_is_seq(char *str)
{
	int	param[5];

	ft_memset(param, 0, sizeof(param));
	while (str[param[0]])
	{
		while (str [param[0]] && str[param[0]] != '.')
			param[0]++;
		if (!ft_strncmp(&str[param[0]], "..", 2))
		{
			if (!brace_process_seq(str, param))
				return (0);
		}
		else if (param[2] >= 2)
			return (brace_process_step(str, param));
		else if (!param[2])
			return (0);
	}
	return (1);
}

int	brace_seq_isstep(char *str)
{
	char	*ptr;

	ptr = ft_strnstr(str, "..", ft_strlen(str));
	if (!ptr)
		return (0);
	ptr += 2;
	if (!ft_strnstr(ptr, "..", ft_strlen(ptr)))
		return (0);
	return (1);
}
