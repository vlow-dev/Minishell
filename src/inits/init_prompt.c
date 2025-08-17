/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_prompt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:59:11 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:01:01 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

int		input_check(t_data *data);
int		input_handle_nl(t_data *data);

static int	init_prompt_check(t_data *data)
{
	if (!input_check(data))
	{
		if (data->line)
			ft_free_setn((void **)&data->line);
		return (0);
	}
	add_history(data->line);
	data->line = ft_strtrim_free(data->line, M_WSPACE);
	if (!data->line || !*data->line)
		return (ft_free_setn((void **)&data->line), 0);
	return (1);
}

static void	init_prompt_input(t_data *data)
{
	char	*line;

	if (isatty(STDIN_FILENO))
	{
		if (g_signal)
			ft_printf("\n");
		data->line = readline(RL_CYAN "vsh$> " RL_COLOR);
	}
	else
	{
		line = get_next_line(STDIN_FILENO);
		while (line)
		{
			data->line = ft_strjoin_free(data->line, line, 1);
			if (!data->line)
			{
				ft_gnl_flush(STDIN_FILENO);
				ft_printf_fd(2, "Minishell: TTY: Fatal Error: %s\n",
					strerror(ENOMEM));
				clear_data(data);
				exit (1);
			}
			line = get_next_line(STDIN_FILENO);
		}
	}
}

static void	init_prompt_options(t_data *data)
{
	if (input_handle_nl(data))
	{
		data->line = ft_strdup(data->nl->token->buf[0]);
		if (!data->line)
		{
			ft_printf_fd(2, "Minishell: TTY: Fatal Error: %s\n",
				strerror(ENOMEM));
			clear_data(data);
			exit (1);
		}
	}
	else
	{
		init_prompt_input(data);
	}
	g_signal = 0;
}

int	init_prompt(t_data *data)
{
	init_prompt_options(data);
	if (!data->line)
	{
		clear_data(data);
		exit (data->exit_status);
	}
	if (!init_prompt_check(data))
		return (0);
	return (1);
}

int	init_token_parser(t_data *data)
{
	if (!init_token(data, data->line, 0))
	{
		clear_data(data);
		ft_printf_fd(2, "Minishell: Token: Fatal Error: %s\n",
			strerror(ENOMEM));
		exit (1);
	}
	free(data->line);
	data->line = NULL;
	if (!init_parsing(data))
	{
		free_token(data);
		free_tree(data->root);
		data->root = NULL;
		free_nl(data);
		data->exit_status = 2;
		return (0);
	}
	return (1);
}
