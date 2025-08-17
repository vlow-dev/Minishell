/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:47:47 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:14:14 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <errno.h>

t_tree	*grow_tree_leaf(t_token *token);
int		init_heredoc_prompt(t_data *data, t_token *token);
int		doc_expansion(t_data *data, t_tree *leaf);

int	operator_prompt(t_data *data)
{
	char	*op_line;

	op_line = NULL;
	while (1)
	{
		if (!ast_prompt_rl(data, &op_line))
			return (0);
		if (!input_check(data))
		{
			op_line = ft_strjoin_free(op_line, data->line, 1);
			data->line = NULL;
			continue ;
		}
		data->line = ft_strjoin_free(op_line, data->line, 1);
		if (!data->line || !*data->line)
			continue ;
		add_history(data->line);
		if (!init_token(data, data->line, 1))
		{
			ft_printf_fd(2, "Minishell: Fatal Error: %s\n", strerror(ENOMEM));
			return (0);
		}
		break ;
	}
	return (1);
}

int	redir_valid(t_data *data, t_token *token)
{
	int	ret;

	if (!token->buf[2])
		return (0);
	if (token->type == TOKEN_HERESTR || token->type == TOKEN_HEREDOC)
	{
		ret = init_heredoc_prompt(data, token);
		if (ret <= 0)
			return (ret);
	}
	return (1);
}

void	ft_subs_sp(char **arr)
{
	int		i;
	char	*ptr;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		ptr = arr[i];
		while (*ptr)
		{
			if (*ptr == ' ')
				*ptr = M_US_C;
			ptr++;
		}
		i++;
	}
}

void	ft_subs_resp(char **arr)
{
	int		i;
	char	*ptr;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		ptr = arr[i];
		while (*ptr)
		{
			if (*ptr == M_US_C)
				*ptr = ' ';
			ptr++;
		}
		i++;
	}
}

t_tree	*parse_redir(t_data *data)
{
	t_token	*token;
	t_tree	*leaf;

	leaf = NULL;
	token = get_token(data);
	if (!token)
		return (NULL);
	if (token && token->type <= TOKEN_REDIR_OUT)
	{
		if (!redir_valid(data, token))
			return (NULL);
		leaf = grow_tree_leaf(token);
		if (!leaf)
			return (NULL);
		if (!doc_expansion(data, leaf))
			return (free_tree(leaf), NULL);
		data->i_token++;
	}
	return (leaf);
}
