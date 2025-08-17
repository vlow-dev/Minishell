/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:51:56 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:53:11 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

char	**glob_expand(char **dir_list, char *str, int flag);
char	**glob_init_dir(void);

static int	glob_result_literal(char **new)
{
	char	**temp;
	char	*ptr;
	char	*ptr2;
	int		size;

	if (!new)
		return (0);
	temp = new;
	while (*temp)
	{
		ptr = *temp;
		size = ft_strlen(*temp);
		ptr2 = malloc(size + 3);
		if (!ptr2)
			return (0);
		ptr2[0] = '\'';
		ft_memcpy(ptr2 + 1, ptr, size);
		ptr2[size + 1] = '\'';
		ptr2[size + 2] = '\0';
		free(ptr);
		*temp = ptr2;
		temp++;
	}
	return (1);
}

static char	**glob_result(char **dir_list, char **pre_list)
{
	char	**new;
	int		i;

	new = NULL;
	i = 0;
	while (dir_list[i])
	{
		if (ft_strcmp(pre_list[i], M_US))
		{
			new = split_free_append(new, dir_list[i], 0);
			if (!new)
				break ;
		}
		i++;
	}
	split_free(dir_list);
	split_free(pre_list);
	ft_qsort_arr(new);
	if (!glob_result_literal(new))
		return (split_free(new), NULL);
	return (new);
}

static char	**glob_remove_hidden(char **dir_list)
{
	int		i;
	char	**new;

	i = -1;
	new = NULL;
	while (dir_list[++i])
	{
		if (*dir_list[i] == '.')
		{
			new = split_free_append(new, M_US, 0);
			if (!new)
				return (NULL);
			continue ;
		}
		new = split_free_append(new, dir_list[i], 0);
		if (!new)
			return (NULL);
	}
	return (new);
}

static char	**glob_hidden(char **dir_list, char *str)
{
	char	**new;
	int		i;

	new = NULL;
	i = 0;
	while (str[i] && !ft_strchr(".*", str[i]))
		i++;
	if (str[i] && str[i] == '.')
		return (split_free_join(new, dir_list, 0));
	return (glob_remove_hidden(dir_list));
}

char	**glob_init(char *str)
{
	char	**dir_list;
	char	**pre_list;

	pre_list = NULL;
	dir_list = glob_init_dir();
	if (!dir_list)
		return (NULL);
	pre_list = glob_hidden(dir_list, str);
	if (!pre_list)
		return (split_free(dir_list), NULL);
	if (!ft_strcmp(str, "*"))
		return (glob_result(dir_list, pre_list));
	pre_list = glob_expand(pre_list, str, 0);
	if (!pre_list)
		return (split_free(dir_list), NULL);
	return (glob_result(dir_list, pre_list));
}
