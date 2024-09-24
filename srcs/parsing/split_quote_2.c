/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:50:25 by pepie             #+#    #+#             */
/*   Updated: 2024/09/24 14:07:45 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_string_quote(t_split_sh *sp)
{
	sp->i = 0;
	sp->str_start = 0;
	sp->quote_start = 0;
	sp->is_simp_quote = false;
	sp->is_dbl_quote = false;
	return (0);
}

int	sq_replace_and_free(t_list *elements, char **ret)
{
	t_list		*e_tmp;
	char		*tmp;
	int			i;

	i = 0;
	while (elements)
	{
		tmp = elements->content;
		ret[i] = tmp;
		e_tmp = elements->next;
		free(elements);
		i++;
		elements = e_tmp;
	}
	free(elements);
	ret[i] = NULL;
	return (0);
}
