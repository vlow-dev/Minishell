/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:48:26 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:51:41 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

int	input_check_nl(t_data *data, char **line);

int	input_wait(t_data *data)
{
	char	*wait;

	wait = NULL;
	if (!ast_prompt_rl(data, &wait))
		return (0);
	data->line = ft_strjoin_free(wait, data->line, 1);
	if (!data->line)
		return (0);
	input_check(data);
	return (1);
}

int	input_quotes(t_data *data, char **temp)
{
	*temp = skip_quotes(*temp);
	if (*temp && **temp && ft_strchr(M_QUOTES, **temp))
		return (1);
	if (!input_wait(data))
		return (0);
	return (1);
}

static int	input_valid_nl(char *str)
{
	char	*ptr;

	ptr = str;
	while (*ptr && *ptr != '\n')
	{
		if (ft_strchr(M_QUOTES, *ptr))
			ptr = skip_quotes(ptr);
		if (!ptr || !*ptr)
			return (0);
		ptr++;
	}
	if (*ptr && *ptr == '\n')
		return (1);
	return (0);
}

int	input_check(t_data *data)
{
	char	*temp;
	char	*esc;

	temp = ft_strtrim(data->line, M_WSPACE);
	if (!temp)
		return (0);
	if (input_valid_nl(temp) && !data->nl)
	{
		if (!input_check_nl(data, &temp))
			return (0);
	}
	esc = temp;
	while (*temp)
	{
		if (ft_strchr(M_QUOTES, *temp) && !ft_isesc_ptr(esc, temp))
		{
			if (!input_quotes(data, &temp))
				return (free(esc), 0);
		}
		if (*temp)
			temp++;
	}
	free(esc);
	return (1);
}
