/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_signal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:56:29 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:12:51 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

static void	signal_handle(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 1;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signal_set(t_data *data, int sig, void (*handler)(int))
{
	struct sigaction	new_sa;

	(void)data;
	ft_memset(&new_sa, 0, sizeof(new_sa));
	new_sa.sa_handler = handler;
	sigemptyset(&new_sa.sa_mask);
	sigaction(sig, &new_sa, NULL);
}

static void	init_signal_set(t_data *data, int sig,
				void (*handler)(int), int flags)
{
	struct sigaction	new_sa;

	ft_memset(&new_sa, 0, sizeof(new_sa));
	new_sa.sa_handler = handler;
	new_sa.sa_flags = flags;
	sigemptyset(&new_sa.sa_mask);
	sigaction(sig, &new_sa, NULL);
	if (sig == SIGINT)
		data->rl_sigint = new_sa;
	else if (sig == SIGCHLD)
		data->bg_sigchld = new_sa;
	else if (sig == SIGQUIT)
		data->rl_sigquit = new_sa;
}

void	signal_restore(t_data *data, int sig)
{
	if (sig == SIGINT)
		sigaction(SIGINT, &data->rl_sigint, NULL);
	else if (sig == SIGCHLD)
		sigaction(SIGCHLD, &data->bg_sigchld, NULL);
	else if (sig == SIGQUIT)
		sigaction(SIGQUIT, &data->rl_sigquit, NULL);
}

void	init_signals(t_data *data)
{
	g_signal = 0;
	init_signal_set(data, SIGINT, &signal_handle, SA_RESTART);
	init_signal_set(data, SIGQUIT, SIG_IGN, 0);
	init_signal_set(data, SIGCHLD, SIG_DFL, 0);
}
