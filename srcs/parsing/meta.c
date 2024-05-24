/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: polepie <polepie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:53:35 by polepie           #+#    #+#             */
/*   Updated: 2024/05/24 19:10:56 by polepie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_meta(char c)
{
	if (c == '&')
		return (true);
	if (c == '|')
		return (true);
	if (c == '<')
		return (true);
	if (c == '>')
		return (true);
	if (c == ';')
		return (true);
	return (false);
}

int	handle_meta(char const *str, t_split_sh *sp, t_list **elem, t_ht *env)
{
	ft_lstadd_back(elem, create_str(
			ft_strndup((char *)(&str[sp->str_start]),
				sp->i - sp->str_start), false, env));
	sp->str_start = sp->i + 1;
	return (2);
}
