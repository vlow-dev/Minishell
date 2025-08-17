/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sign_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlow <vlow@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:41:34 by vlow              #+#    #+#             */
/*   Updated: 2025/04/18 22:48:06 by vlow             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

static char	*sign_expand(t_data *data, const char *str, int flag);
int			sign_extract_index(const char *suffix);
char		*sign_extract_var(char *suffix, int len);
int			sign_issp(char *suffix);
char		*sign_sp_replace(t_data *data, char *suffix);
int			sign_isvalid(const char *suffix);
char		*sign_replace(t_data *data, char *str);

static char	*sign_extract(t_data *data, char *prefix, char *suffix)
{
	int		len;
	char	*replace;

	if (!sign_isvalid(suffix))
		return (ft_strjoin_free(prefix, suffix, 1));
	else if (sign_issp(suffix))
	{
		len = 2;
		replace = sign_sp_replace(data, suffix);
	}
	else
	{
		len = sign_extract_index(suffix);
		replace = sign_replace(data, sign_extract_var(suffix, len));
	}
	if (!replace)
		return (free(prefix), free(suffix), NULL);
	replace = ft_strjoin_free(replace, &suffix[len], 0);
	if (!replace)
		return (free(prefix), free(suffix), NULL);
	free(suffix);
	replace = ft_strjoin_free(prefix, replace, 1);
	if (!replace)
		return (NULL);
	return (sign_expand(data, replace, 1));
}

static void	sign_process_invalid(char *str)
{
	char	*ptr;

	ptr = str;
	while (*ptr)
	{
		if (ft_strchr(M_QUOTES, *ptr) && !ft_isesc_ptr(str, ptr))
		{
			ptr = skip_quotes(ptr);
			ptr++;
			continue ;
		}
		if (*ptr && *(ptr + 1) && *ptr == '$' && !ft_isesc_ptr(str, ptr)
			&& !sign_isvalid(ptr))
			ft_memmove(ptr, ptr + 1, ft_strlen(ptr + 1) + 1);
		else if (*ptr)
			ptr++;
	}
	return ;
}

static char	*sign_expand(t_data *data, const char *str, int flag)
{
	char	*prefix;
	char	*suffix;
	char	*new;
	int		len;

	new = NULL;
	sign_process_invalid(((char *)str));
	if (!str || !ft_strchr(str, '$') || !expansion_inquotes(str, "$"))
	{
		new = ft_strdup(str);
		if (flag)
			return (free((char *)str), new);
		return (new);
	}
	len = ft_strchr(str, '$') - (char *)str;
	prefix = ft_substr(str, 0, len);
	suffix = ft_strdup(&str[len]);
	if (!prefix || !suffix)
		return (ft_free_setn((void **)&prefix),
			ft_free_setn((void **)&suffix), NULL);
	if (flag)
		free((char *)str);
	return (sign_extract(data, prefix, suffix));
}

static int	cmd_idx_update(t_tree *leaf, char **arr)
{
	char	**new;
	int		i;

	new = NULL;
	i = -1;
	while (arr[++i])
	{
		if (!ft_strchr(arr[i], ' ') || !expansion_inquotes(arr[i], " "))
		{
			new = split_free_append(new, arr[i], 0);
			if (!new)
				return (0);
			continue ;
		}
		new = split_free_join(new, ft_split(arr[i], ' '), 1);
		if (!new)
			return (0);
	}
	split_free(arr);
	split_free(leaf->token->buf);
	leaf->token->buf = new;
	return (1);
}

int	sign_expansion(t_data *data, t_tree *leaf)
{
	char	**temp;
	char	**new;

	new = NULL;
	temp = leaf->token->buf;
	while (*temp)
	{
		if (ft_strchr(*temp, '$') && expansion_inquotes(*temp, "$"))
			new = split_free_append(new, sign_expand(data, *temp, 0), 1);
		else
			new = split_free_append(new, *temp, 0);
		if (!new)
			return (0);
		temp++;
	}
	if (!cmd_idx_update(leaf, new))
	{
		ft_printf_fd(2, "Minishell: sign expansion: cmd index error!\n");
		return (split_free(new), 0);
	}
	return (1);
}
