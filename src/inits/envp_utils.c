/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:55:06 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:55:31 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

char	*find_envp_str(t_data *data, char *str)
{
	int		i;
	char	*var;
	char	**ep;
	int		size;

	i = 0;
	ep = data->envp.arr;
	if (ft_strchr(str, '='))
		var = str;
	else
		var = ft_strjoin(str, "=");
	if (!var)
		return (str);
	size = ft_strlen(var);
	while (ep[i])
	{
		if (ft_strchr(ep[i], '=') && !ft_strncmp(var, ep[i], size))
			break ;
		i++;
	}
	free(var);
	if (!ep[i])
		return (NULL);
	return (ft_strdup(&ep[i][size]));
}

char	**find_envp(t_data *data, char *str)
{
	int		i;
	char	*var;
	char	**ep;
	int		size;

	i = 0;
	ep = data->envp.arr;
	if (ft_strchr(str, '='))
		var = str;
	else
		var = ft_strjoin(str, "=");
	if (!var)
		return (ep);
	size = ft_strlen(var);
	while (ep[i])
	{
		if (ft_strchr(ep[i], '=') && !ft_strncmp(var, ep[i], size))
			break ;
		i++;
	}
	free(var);
	if (!ep[i])
		return (NULL);
	return (ft_split(&ep[i][size], ':'));
}
