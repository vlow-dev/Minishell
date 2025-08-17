/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:53:37 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:55:20 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <dirent.h>
#include <stdio.h>

int	glob_isvalid(const char *str)
{
	const char	*ptr = str;

	while (*ptr)
	{
		if (ft_strchr(M_QUOTES, *ptr) && !ft_isesc_ptr(str, ptr))
		{
			ptr = skip_quotes(ptr);
			ptr++;
			continue ;
		}
		if (*ptr && *ptr == '*')
			return (1);
		ptr++;
	}
	return (0);
}

int	glob_get_len(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (ft_strchr(M_QUOTES, str[i]) && !ft_isesc_i(str, i))
		{
			i = skip_quotes(&str[i]) - str;
			continue ;
		}
		if (str[i] && str[i] == '*')
			break ;
	}
	return (i);
}

char	**glob_init_dir(void)
{
	DIR				*dir;
	struct dirent	*dir_list;
	char			**list;

	dir = opendir(".");
	if (dir == NULL)
	{
		perror("opendir failed");
		return (NULL);
	}
	list = NULL;
	dir_list = readdir(dir);
	while (dir_list)
	{
		list = split_free_append(list, dir_list->d_name, 0);
		if (!list)
		{
			closedir(dir);
			return (NULL);
		}
		dir_list = readdir(dir);
	}
	closedir(dir);
	return (list);
}
