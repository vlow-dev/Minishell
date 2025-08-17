/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 14:57:05 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:07:11 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

volatile sig_atomic_t	g_signal = 0;

int		input_check(t_data *data);
void	init_signals(t_data *data);
int		init_envp(t_data *data, char **ep);
int		init_prompt(t_data *data);
int		init_token_parser(t_data *data);

/*
 * #############################################################################
 * #								Main                                       #
 * #############################################################################
 */

int	main(int ac, char **av, char **ep)
{
	t_data	data;

	(void)ac;
	(void)av;
	ft_memset(&data, 0, sizeof(t_data));
	data.tty.fd = -1;
	data.tty.s_out = -1;
	if (!init_envp(&data, ep))
	{
		clear_data(&data);
		ft_printf_fd(2, "Minishell: Envp: Fatal Error: %s\n", strerror(ENOMEM));
		return (1);
	}
	init_signals(&data);
	while (1)
	{
		if (!init_prompt(&data) || !init_token_parser(&data))
			continue ;
		execution(&data);
		free(data.line);
		data.line = NULL;
	}
	clear_data(&data);
	return (data.exit_status);
}
