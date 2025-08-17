/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:34:25 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:40:59 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

char	*tilde_extract(char *replace, const char *str, int type);
int		tilde_type(const char *str);
char	*tilde_user(const char *str);

static char	*tilde_oldpwd(t_data *data, const char *str)
{
	char	**oldpwd;
	char	*val;

	oldpwd = find_envp(data, "OLDPWD");
	if (!oldpwd)
		return (NULL);
	val = NULL;
	if (oldpwd[0])
	{
		val = ft_strdup(oldpwd[0]);
		if (!val)
			return (split_free(oldpwd), NULL);
	}
	split_free(oldpwd);
	return (tilde_extract(val, str, 2));
}

static char	*tilde_pwd(t_data *data, const char *str)
{
	char	**pwd;
	char	*val;

	pwd = find_envp(data, "PWD");
	if (!pwd)
		return (NULL);
	val = NULL;
	if (pwd[0])
	{
		val = ft_strdup(pwd[0]);
		if (!val)
			return (split_free(pwd), NULL);
	}
	split_free(pwd);
	return (tilde_extract(val, str, 2));
}

static char	*tilde_home(t_data *data, const char *str)
{
	char	**home;
	char	*val;

	home = find_envp(data, "HOME");
	if (!home)
		return (NULL);
	val = NULL;
	if (home[0])
	{
		val = ft_strdup(home[0]);
		if (!val)
			return (split_free(home), NULL);
	}
	split_free(home);
	return (tilde_extract(val, str, 1));
}

char	*tilde_expand(t_data *data, const char *str)
{
	if (tilde_type(str) == 1)
		return (tilde_home(data, str));
	if (tilde_type(str) == 2)
		return (tilde_pwd(data, str));
	if (tilde_type(str) == 3)
		return (tilde_oldpwd(data, str));
	if (tilde_type(str) == 4)
		return (tilde_user(str));
	return (ft_strdup(str));
}

int	tilde_expansion(t_data *data, t_tree *leaf)
{
	char	**temp;
	char	**new;

	new = NULL;
	temp = leaf->token->buf;
	while (*temp)
	{
		if (ft_strchr(M_TILDE, **temp) && expansion_inquotes(*temp, "~"))
		{
			new = split_free_append(new, tilde_expand(data, *temp), 1);
			if (!new)
				return (0);
		}
		else
		{
			new = split_free_append(new, *temp, 0);
			if (!new)
				return (0);
		}
		temp++;
	}
	split_free(leaf->token->buf);
	leaf->token->buf = new;
	return (0);
}
