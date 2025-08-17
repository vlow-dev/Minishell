/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:15:44 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 21:17:55 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

char	*skip_quotes(const char *str)
{
	const char	*ptr;
	char		c;

	if (!str || !*str)
		return ((char *)str);
	ptr = str;
	if (*ptr && ft_strchr(M_QUOTES, *ptr))
	{
		c = *ptr;
		ptr++;
		while (*ptr && *ptr != c)
		{
			if (*ptr == '\\')
				ptr++;
			ptr++;
		}
	}
	return ((char *)ptr);
}

char	*skip_nested(const char *str)
{
	const char	*ptr;
	int			nested;

	nested = 0;
	ptr = str;
	if (*ptr && ft_strchr(M_PAREN_OPEN, *ptr) && !ft_isesc_ptr(str, ptr))
	{
		nested++;
		while (nested && *ptr)
		{
			if (*(ptr + 1))
				ptr++;
			else
				break ;
			if (*ptr && *ptr == '\\' && *(ptr + 1))
				ptr++;
			else if (*ptr && ft_strchr(M_PAREN_OPEN, *ptr)
				&& !ft_isesc_ptr(str, ptr))
				nested++;
			else if (*ptr && ft_strchr(M_PAREN_CLOSE, *ptr)
				&& !ft_isesc_ptr(str, ptr))
				nested--;
		}
	}
	return ((char *)ptr);
}

char	*skip_wspaces(const char *str)
{
	const char	*ptr;

	ptr = str;
	while (*ptr && ft_strchr(M_WSPACE, *ptr))
		ptr++;
	return ((char *)ptr);
}

char	*skip_to_wspaces(const char *str)
{
	const char	*ptr;

	ptr = str;
	while (*ptr && !ft_strchr(M_WSPACE, *ptr))
		ptr++;
	return ((char *)ptr);
}

char	*skip_to_wspaces_section(const char *str)
{
	const char	*ptr;

	ptr = str;
	while (*ptr && !ft_strchr(M_WSPACE, *ptr))
	{
		if (ft_strchr(M_QUOTES, *ptr) && !ft_isesc_ptr(str, ptr))
			ptr = skip_quotes(ptr);
		else if (ft_strchr(M_PAREN_OPEN, *ptr) && !ft_isesc_ptr(str, ptr))
			ptr = skip_nested(ptr);
		ptr++;
	}
	return ((char *)ptr);
}
