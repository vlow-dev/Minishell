/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_inquotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:08:19 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:08:38 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static const char	*get_set_start(const char *str, const char *set)
{
	const char	*ptr = str;

	while (*ptr)
	{
		while (*ptr && !ft_strchr(set, *ptr))
			ptr++;
		if (*ptr && !ft_isesc_ptr(str, ptr))
			return (ptr);
		if (*ptr)
			ptr++;
	}
	return (NULL);
}

static const char	*get_set_start_asterisk(const char *str, const char *set)
{
	const char	*ptr = str;

	while (*ptr)
	{
		if (ft_strchr(M_QUOTES, *ptr) && !ft_isesc_ptr(str, ptr))
		{
			ptr = skip_quotes(ptr);
			ptr++;
			continue ;
		}
		if (*ptr && ft_strchr(set, *ptr) && !ft_isesc_ptr(str, ptr))
			return (ptr);
		ptr++;
	}
	return (NULL);
}

static int	valid_set(const char **ptr, const char *start_set, const char *set)
{
	const char	*start_str;
	const char	*end_str;

	start_str = *ptr;
	end_str = skip_quotes(start_str);
	if (start_set > start_str && start_set < end_str)
	{
		if (*start_str == '\"')
		{
			if (!ft_strcmp(set, "{}") || !ft_strcmp(set, "~")
				|| !ft_strcmp(set, "*") || !ft_strcmp(set, " "))
				return (0);
			return (1);
		}
		return (0);
	}
	else if (start_set < start_str)
		return (1);
	*ptr = end_str + 1;
	return (-1);
}

int	expansion_inquotes(const char *str, const char *set)
{
	const char	*start_set;
	const char	*ptr = str;
	int			ret;

	if (!ft_strcmp(set, "*"))
		start_set = get_set_start_asterisk(str, set);
	else
		start_set = get_set_start(str, set);
	if (!start_set || !*start_set)
		return (0);
	while (*ptr)
	{
		while (*ptr && !ft_strchr(M_QUOTES, *ptr))
			ptr++;
		if (!ptr || !*ptr)
			return (1);
		ret = valid_set(&ptr, start_set, set);
		if (ret >= 0)
			return (ret);
	}
	return (0);
}
