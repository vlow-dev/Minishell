/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_nl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:50:28 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:53:17 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

t_tree	*empty_branch(void);

static int	handle_valid_nl(t_data *data, char **nl, int size)
{
	if (size == 1 && ft_strstr(nl[0], "<<"))
	{
		free_nl(data);
		return (0);
	}
	if (size > 1)
	{
		free(nl[0]);
		ft_memmove(nl, nl + 1, size * sizeof(char *));
		return (1);
	}
	else
	{
		free_nl(data);
		return (0);
	}
	return (0);
}

int	input_handle_nl(t_data *data)
{
	int		size;
	char	**nl;

	if (!data->nl)
		return (0);
	nl = data->nl->token->buf;
	size = ft_split_size(nl);
	if (nl && size >= 1)
		return (handle_valid_nl(data, nl, size));
	return (0);
}

static int	ft_subs_nl(char *str)
{
	char	*ptr;
	int		state;

	ptr = str;
	state = 0;
	while (*ptr)
	{
		if (ft_strchr(M_QUOTES, *ptr) && !ft_isesc_ptr(str, ptr))
			state = !state;
		else if (*ptr == '\n' && state)
			*ptr = M_US_C;
		ptr++;
	}
	return (state);
}

static void	ft_subs_renl(char **arr)
{
	char	**temp;
	char	*str;

	temp = arr;
	while (*temp)
	{
		str = *temp;
		while (*str)
		{
			if (*str == M_US_C)
				*str = '\n';
			str++;
		}
		temp++;
	}
}

int	input_check_nl(t_data *data, char **line)
{
	ft_subs_nl(*line);
	data->nl = empty_branch();
	if (!data->nl)
		return (0);
	data->nl->token = malloc(sizeof(t_token));
	if (!data->nl->token)
		return (ft_free(line), free_nl(data), 0);
	ft_memset(data->nl->token, 0, sizeof(t_token));
	data->nl->token->buf = ft_split(*line, '\n');
	if (!data->nl->token->buf)
		return (ft_free(line), free_nl(data), 0);
	free(*line);
	free(data->line);
	ft_subs_renl(data->nl->token->buf);
	data->line = ft_strdup(data->nl->token->buf[0]);
	if (!data->line)
		return (0);
	*line = ft_strtrim(data->nl->token->buf[0], M_WSPACE);
	if (!*line)
		return (0);
	return (1);
}
