/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote_6.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:50:25 by pepie             #+#    #+#             */
/*   Updated: 2024/12/03 17:33:44 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sq_replace_and_free(t_list *elements, t_list **ret, t_ht *env)
{
	int		status;
	t_pars	*pars;

	pars = create_pars(ret, elements);
	if (!pars)
		return (1);
	while (pars->elements)
	{
		if (((t_splitted *)pars->elements->content)->is_delimiter)
		{
			status = handle_delimiter(pars, env);
			if (status == 1)
				return (1);
			else if (status == 2)
				continue ;
			else if (status == 3)
				break ;
		}
		if (to_argv(pars))
			continue ;
	}
	return (end_replace(pars), 0);
}
