/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:26:01 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:17:50 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	ft_cd(t_data *data, char **av);
int	ft_echo(t_data *data, char **av);
int	ft_env(t_data *data, char **cmd);
int	ft_exit(t_data *data, char **av);
int	ft_export(t_data *data, char **av);
int	ft_pwd(void);
int	ft_unset(t_data *data, char **av);

int	is_builtins(t_tree *leaf)
{
	const char	*cmds[] = {
		"cd", "echo", "env", "exit", "export", "pwd", "unset", NULL};
	int			i;

	i = 0;
	while (cmds[i])
	{
		if (leaf->token->buf && leaf->token->buf[0]
			&& !ft_strcmp(cmds[i], leaf->token->buf[0]))
			return (1);
		i++;
	}
	return (0);
}

static int	execute_builtins_helper(t_data *data, char **cmds, int i)
{
	if (!i)
		return (ft_cd(data, cmds));
	else if (i == 1)
		return (ft_echo(data, cmds));
	else if (i == 2)
		return (ft_env(data, cmds));
	else if (i == 3)
		return (ft_exit(data, cmds));
	else if (i == 4)
		return (ft_export(data, cmds));
	else if (i == 5)
		return (ft_pwd());
	else if (i == 6)
		return (ft_unset(data, cmds));
	return (-1);
}

int	execute_builtins(t_data *data, char **cmds)
{
	const char	*bi_cmds[] = {
		"cd", "echo", "env", "exit", "export", "pwd", "unset", NULL};
	int			i;

	i = 0;
	while (bi_cmds[i])
	{
		if (!ft_strcmp(bi_cmds[i], cmds[0]))
			break ;
		i++;
	}
	return (execute_builtins_helper(data, cmds, i));
}
