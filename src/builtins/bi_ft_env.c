/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_ft_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:21:08 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 09:51:38 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <errno.h>
#include <string.h>

int	ft_env(t_data *data, char **cmd)
{
	char	**ep;

	if (ft_split_size(cmd) > 1)
	{
		ft_printf_fd(2, "Minishell: env: %s: %s\n", cmd[1], strerror(ENOENT));
		return (127);
	}
	ep = data->envp.arr;
	while (*ep)
	{
		if (ft_strchr(*ep, '='))
			ft_printf("%s\n", *ep);
		ep++;
	}
	return (0);
}
