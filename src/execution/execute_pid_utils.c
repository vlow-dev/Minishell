/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pid_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:24:24 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:25:41 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <sys/wait.h>

int	add_pid(t_data *data, pid_t pid)
{
	t_list	*temp;
	pid_t	*new_pid;

	new_pid = malloc(sizeof(pid_t));
	if (!new_pid)
		return (0);
	*new_pid = pid;
	temp = ft_lstnew(new_pid);
	if (!temp)
		return (0);
	if (!data->pid)
		ft_lstadd_front(&data->pid, temp);
	else
		ft_lstadd_back(&data->pid, temp);
	return (1);
}

pid_t	get_lastpid(t_data *data)
{
	pid_t	pid;
	t_list	*temp;

	temp = ft_lstlast(data->pid);
	if (!temp)
		return (-1);
	pid = *(pid_t *)temp->content;
	return (pid);
}

void	pid_wait(t_data *data)
{
	if (get_lastpid(data) == -1)
		return ;
	signal_restore(data, SIGINT);
	waitpid(get_lastpid(data), &data->status, 0);
	data->exit_status = WEXITSTATUS(data->status);
	while (wait(NULL) != -1)
		;
	ft_lstclear(&data->pid, free);
}
