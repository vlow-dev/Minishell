/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_ft_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 01:21:37 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:20:20 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "errno.h"

static int	update_env(t_data *data, char *cwd);
static char	*get_target(t_data *data, char **av);
int			ft_export(t_data *data, char **av);

int	ft_cd(t_data *data, char **av)
{
	char	*cwd;
	char	*target;

	if (ft_split_size(av) > 2)
	{
		ft_printf_fd(2, "Minishell: cd: too many arguments\n");
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("Minishell: getcwd error");
		return (1);
	}
	target = get_target(data, av);
	if (!target)
		return (free(cwd), 1);
	if (chdir(target))
	{
		ft_printf_fd(2, "Minishell: cd: %s: %s\n", target, strerror(ENOENT));
		return (free(cwd), free(target), 1);
	}
	if (!update_env(data, cwd))
		return (free(cwd), free(target), 1);
	return (free(cwd), free(target), 0);
}

static char	*extract_env(t_data *data, char *str)
{
	char	**temp;
	char	*val;

	temp = find_envp(data, str);
	if (!temp)
		return (NULL);
	val = ft_strdup(*temp);
	if (!val)
		return (split_free(temp), NULL);
	return (split_free(temp), val);
}

static char	*get_target(t_data *data, char **av)
{
	char	*target;

	if (!av[1] || !ft_strcmp(av[1], "--"))
	{
		target = extract_env(data, "HOME");
		if (!target)
		{
			perror("Minishell: cd: HOME not set");
			return (NULL);
		}
		return (target);
	}
	else if (!ft_strcmp(av[1], "-"))
	{
		target = extract_env(data, "OLDPWD");
		if (!target)
		{
			perror("Minishell: cd: OLDPWD not set");
			return (NULL);
		}
		printf("%s\n", target);
		return (target);
	}
	return (ft_strdup(av[1]));
}

static int	set_export(t_data *data, char *str)
{
	char	**new;

	new = NULL;
	new = split_free_append(new, "export", 0);
	if (!new)
		return (0);
	new = split_free_append(new, str, 0);
	if (!new)
		return (0);
	if (ft_export(data, new))
	{
		split_free(new);
		return (0);
	}
	split_free(new);
	return (1);
}

static int	update_env(t_data *data, char *cwd)
{
	char	*old;
	char	*cur;
	char	*temp;

	old = ft_strjoin("OLDPWD=", cwd);
	if (!old)
		return (0);
	temp = getcwd(NULL, 0);
	if (!temp)
		return (free(old), 0);
	cur = ft_strjoin("PWD=", temp);
	if (!cur)
		return (free(old), free(temp), 0);
	if (!set_export(data, old) || !set_export(data, cur))
		return (free(old), free(cur), free(temp), 0);
	return (free(old), free(cur), free(temp), 1);
}
