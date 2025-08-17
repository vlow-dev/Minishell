/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_ft_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 02:03:18 by vlow              #+#    #+#             */
/*   Updated: 2025/04/16 16:29:26 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

static int	set_n_exit(t_data *data, long long exit_code);
static int	isvalid_digit(char *str);

int	ft_exit(t_data *data, char **av)
{
	long long	exit_code;
	char		*ptr;

	ptr = NULL;
	if (!av[1])
		exit(data->exit_status);
	else if (!*av[1] || !isvalid_digit(av[1]))
	{
		ft_printf_fd(2,
			"Minishell: exit: %s: numeric argument required\n", av[1]);
		exit(2);
	}
	else if (ft_split_size(av) > 2)
	{
		ft_putstr_fd("Minishell: exit: too many arguments\n", 2);
		return (1);
	}
	exit_code = strtol(av[1], &ptr, 10) % 256;
	if (*ptr)
	{
		ft_printf_fd(2,
			"Minishell: exit: %s: numeric argument required\n", av[1]);
		exit(2);
	}
	return (set_n_exit(data, exit_code));
}

static int	set_n_exit(t_data *data, long long exit_code)
{
	data->exit_status = (int)exit_code;
	exit((int)exit_code);
	return ((int)exit_code);
}

static int	isvalid_digit(char *str)
{
	while (ft_isspace(*str))
		str++;
	if (ft_issign(*str))
		str++;
	while (ft_isdigit(*str))
		str++;
	if (*str)
		return (0);
	return (1);
}
