/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:18:12 by pepie             #+#    #+#             */
/*   Updated: 2024/12/03 15:54:49 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_last_status(char *str, t_ht *env, int i)
{
	char	*tmp;
	char	*res;

	res = ht_search(env, "?");
	if (!res)
		res = "0";
	tmp = ft_strndup(str, i);
	tmp = ft_strjoin_free(tmp, res);
	if (str[i + 2])
		tmp = ft_strjoin_free(tmp, &str[i + 2]);
	free(str);
	return (tmp);
}

char	*handle_env_var(char **s, t_ht *env, int i)
{
	char	*env_var;
	char	*tmp;
	char	*res;
	char	*str;
	int		j;

	str = *s;
	j = i + 1;
	while (ft_isalnum(str[j]) || str[j] == '_')
		j++;
	env_var = ft_strndup(&str[i + 1], j - i - 1);
	tmp = ft_strndup(str, i);
	res = ht_search(env, env_var);
	if (!res)
		res = ft_strdup("");
	tmp = ft_strjoin_free(tmp, res);
	tmp = ft_strjoin_free(tmp, &str[j]);
	free(env_var);
	free(str);
	return (tmp);
}

char	*handle_expansion(char *str, t_ht *env)
{
	int	i;

	i = 0;
	while (str && str[i] != 0)
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			str = handle_last_status(str, env, i);
		}
		else if (str[i] == '$' )
		{
			str = handle_env_var(&str, env, i);
		}
		i++;
	}
	return (str);
}

t_list	*create_str(char *str, bool is_simple_quote, t_ht *env)
{
	t_splitted	*content;

	content = malloc(sizeof(t_splitted));
	if (!content)
		return (NULL);
	content->is_delimiter = false;
	if (!is_simple_quote)
		content->content = handle_expansion(str, env);
	else
		content->content = str;
	return (ft_lstnew(content));
}

void	register_env_var(t_ht *env, char **envp)
{
	char	**splitted;
	char	*val;
	int		i;

	i = 0;
	while (envp[i])
	{
		splitted = ft_split(envp[i], '=');
		if (!splitted)
			return ;
		if (splitted[1])
		{
			val = ft_strdup(splitted[1]);
			ht_insert(env, splitted[0], val);
		}
		ft_freesplit(splitted);
		free(splitted);
		i++;
	}
}
