/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_user.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:35:56 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:40:48 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>

char	*tilde_extract(char *replace, const char *str, int type);

char	*tilde_user_extract(const char *str)
{
	int		i;

	i = 1;
	while (str[i] && !ft_strchr(M_WSPACE "/", str[i]))
		i++;
	if (i >= 1 && str[i])
		return (ft_substr(&str[1], 0, i - 1));
	else if (i >= 1)
		return (ft_strdup(&str[1]));
	return (NULL);
}

static char	*tilde_user_cmp_h(char *line, char *val)
{
	char	**list;
	char	*home;

	home = NULL;
	list = ft_split(line, ':');
	if (!list)
		return (NULL);
	if (ft_split_size(list) >= 5 && !ft_strcmp(val, list[0]))
	{
		home = ft_strdup(list[5]);
		if (!home)
			return (NULL);
	}
	split_free(list);
	return (home);
}

static char	*tilde_user_cmp(int fd, char *val)
{
	char	*line;
	char	*home;

	home = NULL;
	line = get_next_line(fd);
	while (1)
	{
		if (line && !home)
			home = tilde_user_cmp_h(line, val);
		if (line)
			free(line);
		line = get_next_line(fd);
		if (!line)
			break ;
	}
	return (home);
}

static char	*tilde_user_load(char *val)
{
	int		fd;
	char	*home;

	if (!val)
		return (NULL);
	home = NULL;
	fd = open("/etc/passwd", O_RDONLY);
	if (fd == -1)
		return (free(val), NULL);
	home = tilde_user_cmp(fd, val);
	if (!home)
		return (close(fd), free(val), NULL);
	return (close(fd), free(val), home);
}

char	*tilde_user(const char *str)
{
	char	*val;

	val = tilde_user_load(tilde_user_extract(str));
	if (!val)
		return (ft_strdup(str));
	return (tilde_extract(val, str, 4));
}
