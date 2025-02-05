/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:50:25 by pepie             #+#    #+#             */
/*   Updated: 2024/12/03 17:33:44 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_next_elem(t_pars *pars, int delimiter, t_ht *env)
{
	if (handle_reformat_start(pars, delimiter))
		return (1);
	pars->tmp = pars->elements->next->content;
	if (delimiter == PIPE)
		pipe_case(pars, delimiter, pars->tmp, env);
	else
	{
		pars->tmp_exec->argv[pars->tmp_exec->i] = pars->tmp->content;
		pars->tmp_exec->argv[pars->tmp_exec->i + 1] = NULL;
	}
	pars->elements = pars->elements->next;
	pars->tmp = pars->elements->content;
	pars->tmp_exec->i++;
	return (0);
}

int	handle_no_next(t_pars *pars, int delimiter, t_ht *env)
{
	if (pars->can_error)
	{
		ft_putstr_fd("bash: syntax error near unexpected", 2);
		ft_putstr_fd(" token `newline'\n", 2);
		free(pars->tmp_exec);
		ht_deletef(env, "?");
		ht_insert(env, "?", ft_strdup("2"));
		return (1);
	}
	else
	{
		if (delimiter != PIPE)
			pars->can_error = true;
		return (3);
	}
	return (0);
}

int	handle_delimiter(t_pars *pars, t_ht *env)
{
	int			delimiter;
	int			status;

	pars->tmp = pars->elements->content;
	pars->can_error = false;
	delimiter = pars->tmp->delimiter;
	pars->tmp_exec->argv[pars->tmp_exec->i] = NULL;
	if (!pars->elements->next)
		pars->can_error = true;
	else
		if (process_next_elem(pars, delimiter, env))
			return (1);
	if (check_pipe_error(pars, pars->tmp, delimiter, env))
		return (1);
	if (norme_1(pars, delimiter))
		return (1);
	if (pars->elements == NULL)
	{
		status = handle_no_next(pars, delimiter, env);
		if (status == 1)
			return (1);
		else if (status == 3)
			return (3);
	}
	return (norme_2(pars, delimiter));
}

t_pars	*create_pars(t_list **ret, t_list *elements)
{
	t_pars	*pars;

	pars = malloc(sizeof(t_pars));
	if (!pars)
		return (NULL);
	pars->ret = ret;
	pars->elements = elements;
	pars->tmp_exec = init_exec();
	pars->last_neutral = NULL;
	pars->has_started = false;
	if (!pars->tmp_exec || !create_argv(pars->tmp_exec, elements))
		return (free(pars), NULL);
	return (pars);

}

int	to_argv(t_pars *pars)
{
	if (pars->last_neutral)
	{
		append_to_argv(pars, pars->last_neutral);
		return (1);
	}
	else if (pars->tmp_exec->token_next == -1)
		pars->last_neutral = pars->tmp_exec;
	pars->has_started = true;
	pars->tmp_exec->argv[pars->tmp_exec->i] = ((t_splitted *)
			pars->elements->content)->content;
	pars->tmp_exec->argv[pars->tmp_exec->i + 1] = NULL;
	pars->tmp_exec->i++;
	pars->elements = pars->elements->next;
	return (0);
}
