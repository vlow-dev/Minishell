/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:45:03 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:42:28 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <errno.h>

t_tree	*grow_branch(t_token *token);
int		grow_leaf(t_tree *root, t_tree *leaf);

static int	process_subshell_close(t_data *data, t_tree **branch,
								t_tree **leaf)
{
	t_token	*token;

	token = get_token(data);
	if (!token || token->type != TOKEN_PAREN_CLOSE)
		return (free_tree(*branch), 0);
	*leaf = grow_branch(token);
	if (!*leaf)
		return (free_tree(*branch), 0);
	if (!grow_leaf(*branch, *leaf))
		return (free_tree(*branch), free_tree(*leaf), 0);
	data->i_token++;
	return (1);
}

static int	subshell_valid_prompt(t_data *data, char *set)
{
	char	*ptr;

	ptr = data->line;
	while (*ptr && !ft_strchr(set, *ptr) && !ft_isesc_ptr(data->line, ptr))
		ptr++;
	if (!*ptr)
		return (0);
	return (1);
}

static int	subshell_process_prompt(t_data *data, char **op_line)
{
	if (!input_check(data))
	{
		*op_line = ft_strjoin_free(*op_line, data->line, 1);
		data->line = NULL;
		return (0);
	}
	if (!subshell_valid_prompt(data, T_PAREN_CLOSE))
	{
		*op_line = ft_strjoin_free(*op_line, data->line, 1);
		data->line = NULL;
		return (0);
	}
	data->line = ft_strjoin_free(*op_line, data->line, 1);
	if (!data->line || !*data->line)
		return (0);
	return (1);
}

static int	process_subshell_prompt(t_data *data)
{
	char	*op_line;

	op_line = NULL;
	while (1)
	{
		if (!ast_prompt_rl(data, &op_line))
			return (ft_free(op_line), 0);
		if (!subshell_process_prompt(data, &op_line))
			continue ;
		add_history(data->line);
		if (!init_token(data, data->line, 1))
		{
			ft_printf_fd(2, "Minishell: Fatal Error: %s\n", strerror(ENOMEM));
			return (ft_free_setn((void **)&data->line), 0);
		}
		ft_free_setn((void **)&data->line);
		break ;
	}
	return (1);
}

t_tree	*parse_subshell(t_data *data)
{
	t_token	*token;
	t_tree	*leaf;
	t_tree	*branch;

	token = get_token(data);
	branch = grow_branch(token);
	if (!branch)
		return (NULL);
	data->i_token++;
	leaf = parse_bg(data);
	if (!leaf)
		return (free_tree(branch), NULL);
	if (!grow_leaf(branch, leaf))
		return (free_tree(branch), free_tree(leaf), NULL);
	token = get_token(data);
	if (!token || token->type != TOKEN_PAREN_CLOSE)
	{
		if (!process_subshell_prompt(data))
			return (free_tree(branch), NULL);
	}
	if (!process_subshell_close(data, &branch, &leaf))
		return (NULL);
	return (branch);
}
