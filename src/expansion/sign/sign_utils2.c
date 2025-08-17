/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sign_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:43:55 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:45:57 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

char	*sign_getvar(char *str)
{
	char	*new;
	int		i;

	i = 0;
	new = NULL;
	if (!(ft_isalpha(str[i]) || str[i] != '_'))
		return ((char *)str);
	i++;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (i)
	{
		new = ft_substr(str, 0, i);
		if (!new)
			return (free(str), NULL);
	}
	free(str);
	return (new);
}

char	*sign_replace(t_data *data, char *str)
{
	char	*val;

	val = find_envp_str(data, str);
	if (!val)
		return (free(str), ft_strdup(""));
	free(str);
	return (val);
}
