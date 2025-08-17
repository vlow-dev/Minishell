/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sign_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:43:03 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:46:06 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

char	*sign_getvar(char *str);

int	sign_extract_index(const char *suffix)
{
	int	i;

	i = 1;
	if (suffix[i] == '{')
		i = skip_nested(&suffix[i]) - suffix;
	else
	{
		if (!(ft_isalpha(suffix[i]) || suffix[i] != '_'))
			return (i);
		i++;
		while (suffix[i] && (ft_isalnum(suffix[i]) || suffix[i] == '_'))
			i++;
	}
	return (i);
}

char	*sign_extract_var(char *suffix, int len)
{
	char	*ptr;
	int		i;

	i = 1;
	ptr = suffix;
	if (ptr[i] && ptr[i] == '{')
		i++;
	ptr = sign_getvar(ft_substr(suffix, i, len - i));
	if (!ptr)
		return (NULL);
	return (ptr);
}

int	sign_issp(char *suffix)
{
	if (suffix[1] && !ft_strchr(M_SIGN_SP, suffix[1]))
		return (0);
	return (1);
}

char	*sign_sp_replace(t_data *data, char *suffix)
{
	if (suffix[1] == '?')
		return (ft_itoa(data->exit_status));
	return (ft_strdup(""));
}

int	sign_isvalid(const char *suffix)
{
	int	i;

	i = 0;
	if (suffix[i] != '$')
		return (0);
	i++;
	if (suffix[i] && (ft_isalpha(suffix[i]) || suffix[i] == '_'
			|| suffix[i] == '{' || ft_strchr(M_SIGN_SP, suffix[i])))
		return (1);
	return (0);
}
