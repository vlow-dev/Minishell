/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:55:43 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:46:36 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "stdlib.h"

char	**normalize_subshell(char **pre_token);
char	**normalize_grouping(char **pre_token);
char	**normalize_logical(char **pre_token);
int		normalize_word_token(t_data *data);
int		normalize_redir_token(t_data *data);
int		create_token(t_data *data, char **pre_token);
char	**normalize_pretoken(char **pre_token);

int	init_token(t_data *data, const char *line, int prompt)
{
	const char	*ptr = line;
	char		**pre_token;

	if (!line || !*line)
		return (1);
	pre_token = NULL;
	while (*ptr)
	{
		ptr = skip_wspaces(ptr);
		line = ptr;
		ptr = skip_to_wspaces_section(ptr);
		pre_token = split_free_append(pre_token,
				ft_substr(line, 0, ptr - line), 1);
		if (!pre_token)
			return (0);
		if (!ptr || !*ptr)
			break ;
	}
	if (!prompt)
		free_token(data);
	if (!create_token(data, normalize_pretoken(pre_token)))
		return (0);
	normalize_redir_token(data);
	normalize_word_token(data);
	return (1);
}

char	**normalize_pretoken(char **pre_token)
{
	char	**new;

	new = NULL;
	new = normalize_subshell(pre_token);
	if (!new)
		return (NULL);
	new = normalize_grouping(new);
	if (!new)
		return (NULL);
	new = normalize_logical(new);
	if (!new)
		return (NULL);
	return (new);
}
