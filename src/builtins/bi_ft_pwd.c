/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_ft_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 01:52:56 by vlow              #+#    #+#             */
/*   Updated: 2025/03/26 15:36:45 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

int	ft_pwd(void)
{
	char	*ep_cwd;

	ep_cwd = getcwd(NULL, 0);
	if (!ep_cwd)
	{
		ft_printf_fd(2, "Minishell: Failed Pwd");
		return (1);
	}
	ft_printf("%s\n", ep_cwd);
	free(ep_cwd);
	return (0);
}
