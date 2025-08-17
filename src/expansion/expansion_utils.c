/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:07:35 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 23:00:35 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int	ft_isesc_i(const char *str, int idx)
{
	int	count;

	count = 0;
	idx--;
	while (idx > 0 && str[idx] == '\\')
	{
		count++;
		idx--;
	}
	return ((count % 2) != 0);
}

int	ft_isesc_ptr(const char *str, const char *idx)
{
	int	count;

	count = 0;
	idx--;
	while (idx >= str && *idx == '\\')
	{
		count++;
		idx--;
	}
	return ((count % 2) != 0);
}
