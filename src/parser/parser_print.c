/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:33:38 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:59:59 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

void	print_tree(const t_tree *root, const char *indent, int last,
			int leaf_idx);

void	print_ast(t_data *data)
{
	ft_printf(CYAN"\n==================================================\n");
	ft_printf("Parsing AST\n");
	ft_printf("==================================================\n"COLOR);
	print_tree(data->root, "", 0, 0);
	ft_printf(CYAN"==================================================\n");
	ft_printf("End AST\n");
	ft_printf("==================================================\n"COLOR);
	ft_printf("\n");
}
