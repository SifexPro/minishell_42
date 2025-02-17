/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prefix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2025/02/04 15:18:41 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_fixed_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		pwd = ft_strdup("--deleted--");
	return (pwd);
}

char	*get_prefix(int last_status)
{
	char	*pwd;
	char	**splitted;
	char	*tmp;

	pwd = get_fixed_pwd();
	if (last_status == 0)
		tmp = ft_strdup("\e[1;32m");
	else
		tmp = ft_strdup("\e[1;31m");
	if (!tmp || !pwd)
		return (NULL);
	tmp = ft_strjoin_free(tmp, "[CUSTOM] \e[1;33m");
	splitted = ft_split(pwd, '/');
	if (!splitted || ft_strarr_len(splitted) == 0)
		return (free(tmp), NULL);
	tmp = ft_strjoin_free(tmp,
			splitted[ft_strarr_len(splitted) - 1]);
	tmp = ft_strjoin_free(tmp, " > \e[0;37m");
	(ft_freesplit(splitted), free(splitted), free(pwd));
	return (tmp);
}

int	handle_lst_status(t_ht *env, int last_status)
{
	char	*last_status_str;

	if (last_status > -1)
	{
		last_status_str = ft_uitoa(last_status);
		ht_deletef(env, "?");
		ht_insert(env, "?", last_status_str);
	}
	else
		last_status = ft_atoi(ht_search(env, "?"));
	return (last_status);
}
