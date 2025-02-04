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

void	fix_argc(t_list *ret)
{
	t_exec	*tmp;
	int		i;

	while (ret)
	{
		i = 0;
		tmp = ret->content;
		while (tmp->argv && tmp->argv[i])
			i++;
		tmp->argc = i;
		ret = ret->next;
	}
}
