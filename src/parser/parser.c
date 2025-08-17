/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:50:33 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:59:31 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	ast_error_handler(t_token *token)
{
	const char	*token_names[] = {
		"CMD", T_HERESTR, T_REDIR_IDUP, T_REDIR_ODUP, T_REDIR_RW,
		T_HEREDOC, T_APPEND, T_REDIR_IN, T_REDIR_OUT, T_OR, T_AND,
		T_PIPE, T_PAREN_OPEN, T_PAREN_CLOSE, T_BRACES_OPEN,
		T_BRACES_CLOSE, T_BACKGROUND, T_SEMICOLON, T_NEWLINE,
		"NULL"
	};

	if (!token)
		return ;
	if (token->type >= TOKEN_WORD && token->type <= TOKEN_NULL && !g_signal)
		ft_printf_fd(2,
			"Minishell: %d: syntax error near unexpected token '%s'\n",
			token->id, token_names[token->type]);
}

int	ast(t_data *data)
{
	t_token	*token;

	data->root = parse_bg(data);
	token = get_token(data);
	if (!data->root || token)
	{
		ast_error_handler(token);
		return (0);
	}
	return (1);
}

int	init_parsing(t_data *data)
{
	free_tree(data->root);
	data->root = NULL;
	data->i_token = 0;
	if (!ast(data))
		return (0);
	return (1);
}
