/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:56:31 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:58:14 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

int	execute_cmd_filter(char **str);

static void	backslash_normal(char *start, char *end)
{
	int		i;
	char	*ptr;

	i = end - start;
	ptr = start;
	while (i >= 0)
	{
		if (ptr[i] && ft_strchr("\\", ptr[i]) && !ft_isesc_i(ptr, i + 1))
		{
			ft_memmove(&ptr[i], &ptr[i + 1], ft_strlen(&ptr[i + 1]) + 1);
			i -= 2;
		}
		else
			i--;
	}
}

static void	backslash_sp(char *start, char *end)
{
	const char	*charset = "\"\\$`";
	int			i;
	char		*ptr;

	i = end - start;
	ptr = start;
	while (i >= 0)
	{
		if (ptr[i] && ptr[i] == '\\' && ptr[i + 1]
			&& ft_strchr(charset, ptr[i + 1]) && !ft_isesc_i(ptr, i + 1))
		{
			ft_memmove(&ptr[i], &ptr[i + 1], ft_strlen(&ptr[i + 1]) + 1);
			i -= 2;
		}
		else
			i--;
	}
}

char	*quotes_memmove(char *str)
{
	char	*start_str;
	char	*end_str;
	char	*ptr;
	size_t	offset;

	ptr = str;
	while (*ptr)
	{
		while (*ptr && !(ft_strchr(M_QUOTES, *ptr) && !ft_isesc_ptr(str, ptr)))
			ptr++;
		if (!ptr || !*ptr)
			return (backslash_normal(str, ptr - 1), str);
		backslash_normal(str, ptr);
		start_str = ptr;
		end_str = skip_quotes(start_str);
		if (*start_str == '\"')
			backslash_sp(start_str, end_str);
		offset = end_str - str;
		ft_memmove(end_str, end_str + 1, ft_strlen(end_str + 1) + 1);
		ft_memmove(start_str, start_str + 1, ft_strlen(start_str + 1) + 1);
		ptr = str + offset - 1;
	}
	return (str);
}

static void	quotes_echo(t_data *data, char **str)
{
	char	**ptr;
	int		i;

	i = 1;
	ptr = str;
	while (ptr[i])
	{
		if (ft_strlen(ptr[i]) > 3 && ft_strchr(M_QUOTES, *ptr[i])
			&& !ft_strncmp(&ptr[i][1], "-n", 2))
		{
			data->echo_n |= (1 << i);
		}
		i++;
	}
}

int	quotes_removal(t_data *data, t_tree *leaf)
{
	char	**temp;
	char	**new;

	new = NULL;
	temp = leaf->token->buf;
	execute_cmd_filter(temp);
	if (*temp && !ft_strcmp(*temp, "echo"))
		quotes_echo(data, temp);
	while (*temp)
	{
		if (ft_strchr(*temp, '\"') || ft_strchr(*temp, '\'')
			|| ft_strchr(*temp, '\\'))
			new = split_free_append(new, quotes_memmove(*temp), 0);
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
