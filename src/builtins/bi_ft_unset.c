/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_ft_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:27:26 by vlow              #+#    #+#             */
/*   Updated: 2025/04/16 23:03:03 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

static char	*get_valid_var(char *str);
static int	var_in_env(t_data *data, char *str_var);
static int	var_check(t_data *data, char *str);

int	ft_unset(t_data *data, char **av)
{
	int		i;

	i = 1;
	while (av[i])
	{
		if (!var_check(data, av[i]))
		{
			if (*av[i] == '-')
			{
				ft_printf_fd(2,
					"Minishell: export: %s: invalid option\n", av[i]);
				return (2);
			}
		}
		i++;
	}
	return (0);
}

static char	*get_valid_var(char *str)
{
	char	*ptr;
	char	*var;

	ptr = ft_strchr(str, '=');
	if (!ptr || !*ptr)
		var = ft_strdup(str);
	else
		var = ft_substr(str, 0, ptr - str);
	if (!var)
	{
		ft_printf_fd(2, "Error Placeholder in get_valid_var\n");
		return (NULL);
	}
	ptr = var;
	if (!(ft_isalpha(*ptr) || *ptr == '_'))
		return (free(var), NULL);
	while (*(++ptr))
	{
		if (*ptr && (!ft_isalnum(*ptr) && *ptr != '_'))
			return (free(var), NULL);
	}
	return (var);
}

static int	var_in_env(t_data *data, char *str_var)
{
	char	**ep;
	char	*ep_var;
	int		i;
	char	*ptr;

	i = -1;
	ptr = NULL;
	ep = data->envp.arr;
	while (ep[++i])
	{
		ep_var = get_valid_var(ep[i]);
		if (!ep_var)
			return (0);
		if (!ft_strcmp(ep_var, str_var))
		{
			ptr = ep[i];
			ft_memmove(&ep[i], &ep[i + 1], (ft_split_size(&ep[i + 1]) + 1)
				* sizeof(char *));
			free(ptr);
			free(ep_var);
			break ;
		}
		free(ep_var);
	}
	return (1);
}

static int	var_check(t_data *data, char *str)
{
	char	*str_var;

	str_var = get_valid_var(str);
	if (!str_var)
		return (0);
	if (!var_in_env(data, str_var))
	{
		free(str_var);
		return (0);
	}
	free(str_var);
	return (1);
}
