/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_ft_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:14:00 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 20:14:59 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

static char	*get_valid_var(char *str);
static int	var_in_env(t_data *data, char *str, char *str_var);
static int	var_check(t_data *data, char *str);
static void	export_print(char **ep);

int	ft_export(t_data *data, char **av)
{
	char	**ep;
	int		i;

	i = 1;
	ep = data->envp.arr;
	if (!av[i])
		return (export_print(ep), 0);
	while (av[i])
	{
		if (!var_check(data, av[i]))
		{
			if (*av[i] == '-')
			{
				ft_printf_fd(2, "Minishell: export: %s: invalid option\n",
					av[i]);
				return (2);
			}
			ft_printf_fd(2,
				"Minishell: export: `%s\': not a valid identifier\n", av[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	export_print(char **ep)
{
	char	*dup;
	char	*ptr;

	while (*ep)
	{
		dup = ft_strdup(*ep);
		if (!dup)
			return ;
		ptr = ft_strchr(dup, '=');
		if (!ptr || !*ptr)
		{
			ft_printf("declare -x %s\n", dup);
			free(dup);
		}
		else
		{
			*ptr = '\0';
			ft_printf("declare -x %s=\"%s\"\n", dup, ptr + 1);
			free(dup);
		}
		ep++;
	}
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
		ft_printf_fd(2, "Error Placeholder in get_var\n");
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

static int	var_in_env(t_data *data, char *str, char *str_var)
{
	char	**ep;
	char	*ep_var;

	ep = data->envp.arr;
	while (*ep)
	{
		ep_var = get_valid_var(*ep);
		if (!ep_var)
			return (0);
		if (!ft_strcmp(ep_var, str_var))
		{
			free(*ep);
			*ep = ft_strdup(str);
			if (!*ep)
				return (free(ep_var), 0);
			return (free(ep_var), 1);
		}
		free(ep_var);
		ep++;
	}
	data->envp.arr = split_free_append(data->envp.arr, str, 0);
	if (!data->envp.arr)
		return (0);
	return (1);
}

static int	var_check(t_data *data, char *str)
{
	char	*str_var;

	str_var = get_valid_var(str);
	if (!str_var)
		return (0);
	if (!var_in_env(data, str, str_var))
	{
		free(str_var);
		return (0);
	}
	free(str_var);
	return (1);
}
