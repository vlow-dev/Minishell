/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_user2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:37:54 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:40:32 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

char	*tilde_user_extract(const char *str);

static int	tilde_type_user(const char *str)
{
	int	i;
	int	j;

	i = 1;
	if (!ft_isalpha(str[i]))
		return (0);
	i++;
	j = i;
	while (str[i] && !ft_strchr(M_WSPACE "/", str[i]))
		i++;
	while (j < i)
	{
		if (!(ft_isalnum(str[j]) || str[j] == '-'))
			return (0);
		j++;
	}
	return (1);
}

int	tilde_type(const char *str)
{
	if (str[0] == '~')
	{
		if (str[1] && str[1] == '+' && (!str[2]
				|| ft_strchr(M_WSPACE "/", str[2])))
			return (2);
		else if (str[1] && str[1] == '-' && (!str[2]
				|| ft_strchr(M_WSPACE "/", str[2])))
			return (3);
		else if (str[1] && tilde_type_user(str))
			return (4);
		else if (!str[1] || ft_strchr(M_WSPACE "/", str[1]))
			return (1);
	}
	return (0);
}

static char	*tilde_user_join(char *replace, const char *str)
{
	int		i;
	char	*ptr;

	ptr = tilde_user_extract(str);
	if (!ptr)
		return (NULL);
	i = ft_strlen(ptr);
	free(ptr);
	return (ft_strjoin(replace, (char *)&str[i + 1]));
}

char	*tilde_extract(char *replace, const char *str, int type)
{
	char	*new;

	new = NULL;
	if (type == 1)
	{
		new = ft_strjoin_free(replace, (char *)&str[1], 0);
		if (!new)
			return (ft_strdup(str));
	}
	else if (type < 4)
	{
		new = ft_strjoin_free(replace, (char *)&str[2], 0);
		if (!new)
			return (ft_strdup(str));
	}
	else
	{
		new = tilde_user_join(replace, str);
		if (!new)
			return (free(replace), ft_strdup(str));
		free(replace);
	}
	return (new);
}
