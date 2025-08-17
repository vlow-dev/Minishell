/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:53:53 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:20:44 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

int			ft_export(t_data *data, char **av);

static int	env_add_cwd(t_data *data)
{
	char	*ep_cwd;
	char	**ep;

	ep_cwd = getcwd(NULL, 0);
	if (!ep_cwd)
		return (0);
	ep = data->envp.arr;
	while (*ep)
	{
		if (!ft_strncmp(*ep, "PATH=", 5))
		{
			*ep = ft_strjoin_free(ft_strjoin_free(*ep, ":", 0), ep_cwd, 0);
			if (!*ep)
			{
				free(ep_cwd);
				return (0);
			}
		}
		ep++;
	}
	free(ep_cwd);
	return (1);
}

static int	env_shlvl_append(t_data *data, char *str)
{
	char	**new;

	if (!str)
		return (0);
	new = split_free_append(split_free_append(NULL, "export", 0), str, 1);
	if (!new)
	{
		free(str);
		return (0);
	}
	if (ft_export(data, new))
	{
		split_free(new);
		return (0);
	}
	split_free(new);
	return (1);
}

static int	env_shlvl(t_data *data)
{
	int		lvl;
	char	**path;

	path = find_envp(data, "SHLVL");
	if (!path || !*path)
		return (0);
	lvl = ft_atoi(*path);
	lvl++;
	free(*path);
	*path = ft_itoa(lvl);
	if (!*path)
		return (0);
	if (!env_shlvl_append(data, ft_strjoin("SHLVL=", *path)))
	{
		split_free(path);
		return (0);
	}
	split_free(path);
	return (1);
}

static void	init_envp_var(t_data *data)
{
	if (!env_add_cwd(data))
	{
		ft_printf_fd(2, "Minishell: Path Cwd: Error!\n");
	}
	if (!env_shlvl(data))
	{
		ft_printf_fd(2, "Minishell: SHLVL: Error!\n");
	}
	data->envp.paths = find_envp(data, "PATH");
	if (!data->envp.paths)
	{
		ft_printf_fd(2, "Minishell: Path: Error!\n");
	}
}

int	init_envp(t_data *data, char **ep)
{
	char	**temp;

	data->envp.ep = ep;
	data->envp.arr = malloc((ft_split_size(ep) + 1) * sizeof(char *));
	if (!data->envp.arr)
		return (0);
	temp = data->envp.arr;
	while (*ep)
	{
		*temp = ft_strdup(*ep);
		if (!*temp)
			return (0);
		temp++;
		ep++;
	}
	*temp = NULL;
	init_envp_var(data);
	return (1);
}
