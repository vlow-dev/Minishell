/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_access.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:32:01 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:33:04 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

static int	append_path(t_data *data, char **temp, char *cmd)
{
	char	*join;
	int		len;

	len = ft_strlen(*temp);
	join = NULL;
	if (len == 0 || (*temp)[len - 1] != '/')
	{
		join = ft_strjoin(*temp, "/");
		if (!join)
			return (0);
	}
	else
		join = ft_strdup(*temp);
	data->envp.cmd_ep = ft_strjoin_free(join, cmd, 0);
	if (!data->envp.cmd_ep)
		return (0);
	return (1);
}

static void	check_stat(char *cmd)
{
	struct stat	st;

	if (stat(cmd, &st))
	{
		ft_printf_fd(2, "Minishell: %s: %s\n", cmd, strerror(errno));
		exit(127);
	}
	if (S_ISDIR(st.st_mode))
	{
		ft_printf_fd(2, "Minishell: %s: Is a directory\n", cmd);
		exit(126);
	}
}

static int	check_relative(t_data *data, char *cmd)
{
	check_stat(cmd);
	data->envp.cmd_ep = cmd;
	return (1);
}

int	check_access(t_data *data, char *cmd)
{
	char	**temp;
	char	**paths;

	if (!cmd || !*cmd || !ft_strcmp(cmd, ".") || !ft_strcmp(cmd, ".."))
		return (0);
	if (!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "/", 1)
		|| !access(cmd, X_OK))
		return (check_relative(data, cmd));
	paths = find_envp(data, "PATH");
	if (!paths)
		return (1);
	temp = paths;
	while (*temp)
	{
		if (!append_path(data, temp, cmd))
			return (split_free(paths), 0);
		if (!access(data->envp.cmd_ep, X_OK))
			return (split_free(paths), 1);
		free(data->envp.cmd_ep);
		data->envp.cmd_ep = NULL;
		temp++;
	}
	split_free(paths);
	data->envp.cmd_ep = NULL;
	return (0);
}
