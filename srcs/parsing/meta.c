/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:53:35 by polepie           #+#    #+#             */
/*   Updated: 2024/12/03 15:35:44 by pepie            ###   ########.fr       */
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
	printf("str: %s\n", (char *)(&str[sp->str_start]));
	printf("tes %s\n", ft_strndup((char *)(&str[sp->str_start]),
				sp->i - sp->str_start));
	ft_lstadd_back(elem, create_str(
			ft_strndup((char *)(&str[sp->str_start]),
				sp->i - sp->str_start), false, env));
	sp->str_start = sp->i + 1;
	return (2);
}
