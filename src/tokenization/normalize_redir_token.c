/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_redir_token.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:25:24 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:45:07 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int	redir_token_check(t_token *token);
int	redir_join(t_data *data, t_token *redir);
int	extract_redir_fd(t_token *token);
int	extract_redir_fd_set(char ***arr, char **buf);
int	extract_redir_fd_append(char ***arr, char **buf, int i);
int	token_reindex(t_data *data);

int	normalize_redir_token(t_data *data)
{
	t_token	*temp;

	temp = data->token;
	while (temp)
	{
		while (temp && (temp->type == TOKEN_WORD
				|| temp->type > TOKEN_REDIR_OUT))
			temp = temp->next;
		if (temp && (temp->type >= TOKEN_HERESTR
				&& temp->type <= TOKEN_REDIR_OUT))
		{
			if (!extract_redir_fd(temp))
				return (0);
			if (!redir_token_check(temp) && temp->next)
				redir_join(data, temp);
		}
		if (temp)
			temp = temp->next;
	}
	if (!token_reindex(data))
		return (0);
	return (1);
}
