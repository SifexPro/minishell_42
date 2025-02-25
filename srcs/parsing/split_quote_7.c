/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:50:25 by pepie             #+#    #+#             */
/*   Updated: 2024/12/03 17:33:44 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_case2(t_pars *pars, t_splitted *tmp)
{
	pars->delimiter = tmp->delimiter;
	if (!pars->elements->next->next)
		return (1);
	pars->elements = pars->elements->next;
	if (!pars->elements->next)
		return (1);
	pars->tmp = pars->elements->next->content;
	if (!pars->tmp)
		return (1);
	pars->tmp_exec->token_next = pars->delimiter;
	if (pars->tmp->content)
	{
		pars->tmp_exec->argv[0] = ft_strdup(pars->tmp->content);
		free(pars->tmp->content);
		pars->tmp->content = NULL;
		pars->tmp_exec->argv[1] = NULL;
	}
	pars->tmp = pars->elements->content;
	return (0);
}
