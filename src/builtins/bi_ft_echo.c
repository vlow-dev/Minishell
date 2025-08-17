/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_ft_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:29:00 by vlow              #+#    #+#             */
/*   Updated: 2025/04/16 23:04:20 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int	echo_valid_n(char *str)
{
	char	*ptr;

	ptr = str;
	if (*ptr && *ptr == '-' && ft_strlen(ptr) > 1)
		ptr++;
	else
		return (0);
	while (*ptr && *ptr == 'n')
		ptr++;
	if (*ptr)
		return (0);
	return (1);
}

static int	esplit_print(char *str, int i, int *nl)
{
	char	**new;

	new = ft_split(str, ' ');
	if (!new)
		return (0);
	if (echo_valid_n(new[i]))
	{
		*nl = 1;
		i++;
	}
	while (new[i])
	{
		if (!echo_valid_n(new[i]))
			break ;
		i++;
	}
	while (new[i])
	{
		ft_printf("%s", new[i]);
		if (new[i + 1])
			ft_printf(" ");
		i++;
	}
	split_free(new);
	return (1);
}

static int	echo_n_split(char *str, int *idx)
{
	int		nl;
	int		i;

	i = 0;
	nl = 0;
	if (ft_word_count(str, ' ') <= 1)
	{
		if (!echo_valid_n(str))
			return (0);
		return (1);
	}
	if (esplit_print(str, i, &nl))
		return (-1);
	(*idx)++;
	return (nl);
}

static void	process_echo_print(t_data *data, char **av, int i)
{
	while (av[i])
	{
		if ((data->echo_n >> i) & 1 ||!echo_valid_n(av[i]))
			break ;
		i++;
	}
	while (av[i])
	{
		ft_printf("%s", av[i]);
		if (av[i + 1])
			ft_printf(" ");
		i++;
	}
}

int	ft_echo(t_data *data, char **av)
{
	int	i;
	int	nl;

	i = 1;
	nl = 0;
	if (!av[i])
	{
		ft_printf("\n");
		return (0);
	}
	if (!ft_strncmp(av[i], "-n", 2) && !((data->echo_n >> i) & 1))
	{
		nl = echo_n_split(av[i], &i);
		if (nl == -1)
			return (1);
	}
	process_echo_print(data, av, i);
	if (!nl)
		ft_printf("\n");
	data->echo_n = 0;
	return (0);
}
