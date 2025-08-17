/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_redir_token_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:13:40 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:45:27 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int		token_merge(t_token *t1, t_token *t2);
void	ft_token_pop(t_data *data, t_token *pop);

int	extract_redir_fd_append(char ***arr, char **buf, int i)
{
	int	j;

	j = i;
	while ((*buf)[i] && ft_strchr(M_REDIR_OP "&", (*buf)[i]))
		i++;
	if (i > j)
	{
		if (!j)
			*arr = split_free_append(*arr, ft_substr(*buf, j, i), 1);
		else
			*arr = split_free_append(*arr, ft_substr(*buf, j, i - 1), 1);
		if (!*arr)
			return (0);
	}
	if ((*buf)[i])
	{
		*arr = split_free_append(*arr, ft_strdup(&(*buf)[i]), 1);
		if (!*arr)
			return (0);
	}
	return (1);
}

int	extract_redir_fd_set(char ***arr, char **buf)
{
	int	i;

	i = 0;
	while ((*buf)[i] && !ft_strchr(M_REDIR_OP "&", (*buf)[i]))
		i++;
	if (i)
		*arr = split_free_append(*arr, ft_substr(*buf, 0, i), 1);
	else
	{
		if ((*buf)[i] == '<')
			*arr = split_free_append(*arr, "0", 0);
		else
			*arr = split_free_append(*arr, "1", 0);
	}
	if (!*arr)
		return (0);
	return (extract_redir_fd_append(arr, buf, i));
}

int	extract_redir_fd(t_token *token)
{
	char	**new;

	new = NULL;
	if (token->type >= TOKEN_HERESTR && token->type <= TOKEN_REDIR_OUT)
	{
		if (!extract_redir_fd_set(&new, token->buf))
			return (split_free(new), 0);
	}
	split_free(token->buf);
	token->buf = new;
	return (1);
}

int	redir_token_check(t_token *token)
{
	if ((token->type >= TOKEN_REDIR_RW && token->type <= TOKEN_REDIR_OUT)
		|| token->type == TOKEN_HERESTR)
	{
		if (ft_isdigit(token->buf[0][0]) && ft_split_size(token->buf) < 3)
			return (0);
		else if (ft_strchr(M_REDIR_OP, token->buf[0][0])
				&& ft_split_size(token->buf) < 2)
			return (0);
	}
	return (1);
}

int	redir_join(t_data *data, t_token *redir)
{
	t_token	*get_word;

	if (redir->next)
		get_word = redir->next;
	else
		get_word = NULL;
	if (get_word && get_word->type == TOKEN_WORD)
	{
		if (!token_merge(redir, get_word))
			return (0);
		ft_token_pop(data, get_word);
	}
	return (1);
}
