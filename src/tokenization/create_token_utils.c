/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:05:34 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:44:14 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int		token_type(const char *temp);

static int	redir_type(const char *temp)
{
	int	i;

	i = 0;
	while (ft_isdigit(temp[i]))
		i++;
	if (i && (temp[i] == '<' || temp[i] == '>'))
		return (token_type(&temp[i]));
	return (0);
}

int	token_type(const char *temp)
{
	const char	*symbols[] = {NULL,
		T_HERESTR, T_REDIR_IDUP, T_REDIR_ODUP, T_REDIR_RW, T_HEREDOC, T_APPEND,
		T_REDIR_IN, T_REDIR_OUT, T_OR, T_AND, T_PIPE,
		T_PAREN_OPEN, T_PAREN_CLOSE, T_BRACES_OPEN, T_BRACES_CLOSE,
		T_BACKGROUND, T_SEMICOLON, T_NEWLINE,
		NULL};
	int			i;

	i = 1;
	while (symbols[i])
	{
		if (!ft_strncmp(symbols[i], temp, ft_strlen(symbols[i])))
			return (i);
		i++;
	}
	return (redir_type(temp));
}
