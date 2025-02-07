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

void	append_to_argv(t_pars *pars, t_exec *elem)
{
	char	**old_argv;
	int		i;
	int		old_argc;

	old_argv = elem->argv;
	old_argc = elem->argc;
	if (!old_argv)
		return ;
	elem->argv = malloc(sizeof(char *) * (old_argc + 1));
	if (!elem->argv)
		return ;
	elem->argc = old_argc + 1;
	i = 0;
	while (old_argv && old_argv[i] && i < elem->i)
	{
		elem->argv[i] = old_argv[i];
		i++;
	}
	elem->argv[i] = ft_strdup(((t_splitted *)pars->elements->content)->content);
	free(((t_splitted *)pars->elements->content)->content);
	((t_splitted *)pars->elements->content)->content = NULL;
	elem->argv[i + 1] = NULL;
	elem->i++;
	free(old_argv);
	pars->elements = pars->elements->next;
}

int	norme_2(t_pars *pars, int delimiter)
{
	if (delimiter != PIPE)
	{
		pars->tmp_exec->argv = malloc(sizeof(char *)
				* (pars->tmp_exec->argc + 1));
		if (!pars->tmp_exec->argv)
			return (free(pars->tmp_exec), 1);
		pars->tmp_exec->i = 0;
		pars->tmp = pars->elements->content;
		return (2);
	}
	else
	{
		pars->tmp = pars->elements->content;
		return (2);
	}
}

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
			pars->tmp = pars->elements->content;
			pars->can_error = false;
			pars->delimiter = pars->tmp->delimiter;
			status = handle_delimiter(pars, env);
			if (status == 1)
				return (free(pars), 1);
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
