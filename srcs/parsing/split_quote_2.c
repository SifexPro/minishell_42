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

int	init_string_quote(t_split_sh *sp)
{
	sp->i = 0;
	sp->str_start = 0;
	sp->quote_start = 0;
	sp->is_simp_quote = false;
	sp->is_dbl_quote = false;
	sp->prev_meta = false;
	sp->pretext = NULL;
	return (0);
}

int	count_until_del(t_list *ret)
{
	t_list		*e_tmp;
	t_splitted	*tmp;
	int			i;

	e_tmp = ret;
	i = 0;
	while (e_tmp)
	{
		tmp = e_tmp->content;
		if (tmp->is_delimiter && tmp->delimiter == PIPE)
			break ;
		e_tmp = e_tmp->next;
		if (tmp->content)
			i++;
	}
	return (i);
}

t_exec	*init_exec(void)
{
	t_exec	*tmp_exec;

	tmp_exec = malloc(sizeof(t_exec));
	if (!tmp_exec)
		return (NULL);
	tmp_exec->argv = NULL;
	tmp_exec->argc = 0;
	tmp_exec->token_next = -1;
	tmp_exec->i = 0;
	return (tmp_exec);
}

void	end_replace(t_parsing *pars)
{
	if (pars->tmp_exec->argv)
		pars->tmp_exec->argv[pars->tmp_exec->i] = NULL;
	if (pars->tmp_exec->i > 0)
	{
		ft_lstadd_back(pars->ret, ft_lstnew(pars->tmp_exec));
	}
	else
	{
		free(pars->tmp_exec->argv);
		free(pars->tmp_exec);
	}
}

bool	create_argv(t_exec *tmp_exec, t_list *elements)
{
	tmp_exec->argv = malloc(sizeof(char *) * (tmp_exec->argc + 1));
	if (!tmp_exec->argv)
		return (false);
	tmp_exec->argc = count_until_del(elements);
	return (true);
}
