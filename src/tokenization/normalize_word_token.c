/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_word_token.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:19:31 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:46:02 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

int		token_merge(t_token *t1, t_token *t2);
void	ft_token_pop(t_data *data, t_token *pop);

int	token_reindex(t_data *data)
{
	t_token	*temp;
	int		i;

	i = 1;
	temp = data->token;
	while (temp)
	{
		if (temp->id != i)
			temp->id = i;
		temp = temp->next;
		i++;
	}
	return (1);
}

static int	word_join(t_data *data, t_token *word1, t_token *section)
{
	t_token	*word2;
	t_token	*next;
	int		issection;

	if (word1->next)
		word2 = word1->next;
	else
		word2 = NULL;
	while (word2)
	{
		next = word2->next;
		issection = (word2 == section);
		if (word2->type == TOKEN_WORD)
		{
			if (!token_merge(word1, word2))
				return (0);
			ft_token_pop(data, word2);
			if (issection)
				break ;
		}
		if (issection || !next)
			break ;
		word2 = next;
	}
	return (1);
}

int	token_merge(t_token *t1, t_token *t2)
{
	char	**temp;

	if (!t1 && !t2)
		return (0);
	temp = ft_split_join(t1->buf, t2->buf);
	if (!temp)
		return (0);
	split_free(t1->buf);
	t1->buf = temp;
	return (1);
}

void	ft_token_pop(t_data *data, t_token *pop)
{
	t_token	*temp;

	if (!data->token || !pop)
		return ;
	temp = data->token;
	if (temp == pop)
	{
		data->token = pop->next;
		if (temp->buf)
			split_free(temp->buf);
		free(temp);
		return ;
	}
	while (temp)
	{
		if (temp->next && temp->next == pop)
		{
			temp->next = pop->next;
			if (pop->buf)
				split_free(pop->buf);
			free(pop);
			return ;
		}
		temp = temp->next;
	}
}

int	normalize_word_token(t_data *data)
{
	t_token	*word;
	t_token	*temp;
	t_token	*next;

	temp = data->token;
	while (temp && temp->next)
	{
		word = NULL;
		next = NULL;
		while (temp && temp->next && temp->type != TOKEN_WORD)
			temp = temp->next;
		if (temp && temp->next && temp->type == TOKEN_WORD)
			word = temp;
		while (temp && temp->next && !(temp->type >= TOKEN_OR))
			temp = temp->next;
		next = temp->next;
		if (word && !word_join(data, word, temp))
			return (0);
		temp = next;
	}
	if (!token_reindex(data))
		return (0);
	return (1);
}
