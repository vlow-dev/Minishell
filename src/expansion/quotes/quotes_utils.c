/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:57:48 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:58:28 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

int	execute_cmd_filter(char **str)
{
	char	**temp;
	int		cur_size;
	int		ori_size;

	temp = str;
	ori_size = ft_split_size(str);
	cur_size = ori_size - 1;
	while (cur_size >= 0)
	{
		temp[cur_size] = ft_strtrim_free(temp[cur_size], M_WSPACE);
		if (!*temp[cur_size])
		{
			free(temp[cur_size]);
			ft_memmove(&temp[cur_size], &temp[cur_size + 1],
				(ori_size - cur_size) * sizeof(char *));
		}
		cur_size--;
	}
	return (1);
}
