/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_print_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:32:08 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 21:34:29 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

void	print_tree(const t_tree *root, const char *indent, int last,
			int leaf_idx);

static int	print_branch(const t_tree *root, const char **token_names,
							int last, int leaf_idx)
{
	if (last)
		ft_printf(GREEN "└── " COLOR);
	else
		ft_printf(GREEN "├── " COLOR);
	if (!root)
	{
		ft_printf("[NULL:(%d)]\n", leaf_idx);
		return (0);
	}
	if (root->token)
		ft_printf(BLUE "(%d)" COLOR "[%s:%d]\n", leaf_idx,
			token_names[root->token->type], root->token->id);
	else
		ft_printf(BLUE "(%d)" COLOR "[%s:%d]\n", leaf_idx,
			token_names[TOKEN_NULL], root->size);
	return (1);
}

static int	print_root(const t_tree *root, const char **token_names,
						int leaf_idx)
{
	if (!root)
	{
		ft_printf("[NULL:(%d)]\n", leaf_idx);
		return (0);
	}
	ft_printf("[Root]\n");
	if (root->token)
		ft_printf("[%s:%d]\n", token_names[root->token->type],
			root->token->id);
	return (1);
}

static void	print_update_indent(char *indentation, const char *indent, int last)
{
	ft_strcpy(indentation, indent);
	if (!*indent)
		ft_strcpy(indentation, "    ");
	else
	{
		if (last)
			ft_strcat(indentation, "    ");
		else
			ft_strcat(indentation, "│   ");
	}
}

static void	print_child_rec(const t_tree *root, const char *indentation)
{
	int	i;
	int	last_child;

	i = 0;
	while (i < root->size)
	{
		last_child = 0;
		if (i == root->size - 1)
			last_child = 1;
		print_tree(root->branch[i], indentation, last_child, i);
		i++;
	}
}

void	print_tree(const t_tree *root, const char *indent, int last,
				int leaf_idx)
{
	char		indentation[1024];
	const char	*token_names[] = {
		"TOKEN_WORD", "TOKEN_HERESTR", "TOKEN_REDIR_IDUP",
		"TOKEN_REDIR_ODUP", "TOKEN_REDIR_RW", "TOKEN_HEREDOC", "TOKEN_APPEND",
		"TOKEN_REDIR_IN", "TOKEN_REDIR_OUT", "TOKEN_OR",
		"TOKEN_AND", "TOKEN_PIPE", "TOKEN_PAREN_OPEN",
		"TOKEN_PAREN_CLOSE", "TOKEN_BRACKET_OPEN", "TOKEN_BRACKET_CLOSE",
		"TOKEN_BACKGROUND", "TOKEN_SEMICOLON", "TOKEN_NEWLINE",
		"TOKEN_NULL"};

	ft_printf("\n");
	if (!*indent)
	{
		if (!print_root(root, token_names, leaf_idx))
			return ;
	}
	else
	{
		ft_printf(GRAY "%s" COLOR, indent);
		if (!print_branch(root, token_names, last, leaf_idx))
			return ;
	}
	print_update_indent(indentation, indent, last);
	print_child_rec(root, indentation);
}
