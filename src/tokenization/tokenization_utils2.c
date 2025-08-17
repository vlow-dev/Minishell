/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:17:00 by vlow              #+#    #+#             */
/*   Updated: 2025/04/19 00:13:43 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "stdlib.h"
#include <stdio.h>

void	free_token(t_data *data)
{
	t_token	*token;
	t_token	*temp;

	token = data->token;
	while (token)
	{
		temp = token->next;
		split_free(token->buf);
		free(token);
		token = temp;
	}
	data->token = NULL;
}

static void	print_token_ex(t_token *token, const char *token_names[])
{
	while (token)
	{
		printf(GRAY"==================================================\n"COLOR);
		printf("token->id = [%d]\n", token->id);
		printf("token->type = [%s]\n", token_names[token->type]);
		printf("\n");
		split_print(token->buf);
		printf("\n");
		token = token->next;
		printf(GRAY"==================================================\n"COLOR);
	}
}

void	print_token(t_data *data)
{
	t_token		*token;
	const char	*token_names[] = {
		"TOKEN_WORD", "TOKEN_HERESTR", "TOKEN_REDIR_IDUP", "TOKEN_REDIR_ODUP",
		"TOKEN_REDIR_RW", "TOKEN_HEREDOC", "TOKEN_APPEND", "TOKEN_REDIR_IN",
		"TOKEN_REDIR_OUT", "TOKEN_OR", "TOKEN_AND", "TOKEN_PIPE",
		"TOKEN_PAREN_OPEN", "TOKEN_PAREN_CLOSE", "TOKEN_BRACKET_OPEN",
		"TOKEN_BRACKET_CLOSE", "TOKEN_BACKGROUND", "TOKEN_SEMICOLON",
		"TOKEN_NEWLINE", "TOKEN_NULL"
	};

	token = data->token;
	printf(GREEN"\n==================================================\n");
	printf("Token data\n");
	printf("==================================================\n"COLOR);
	print_token_ex(token, token_names);
	printf(GREEN"==================================================\n");
	printf("Token End\n");
	printf("==================================================\n"COLOR);
	printf("\n");
}
