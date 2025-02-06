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

int	handle_reformat_start(t_pars *pars, int delimiter)
{
	if (pars->tmp_exec->i != 0)
	{
		ft_lstadd_back(pars->ret, ft_lstnew(pars->tmp_exec));
		pars->tmp_exec = init_exec();
		if (!pars->tmp_exec)
			return (1);
		pars->tmp_exec->argc = count_until_del(pars->elements);
		if (delimiter != PIPE)
		{
			pars->tmp_exec->argv = malloc(sizeof(char *)
					* (pars->tmp_exec->argc + 1));
			if (!pars->tmp_exec->argv)
				return (free(pars->tmp_exec), 1);
		}
		pars->tmp_exec->token_next = pars->tmp->delimiter;
		pars->tmp_exec->i = 0;
	}
	else
		pars->tmp_exec->token_next = pars->tmp->delimiter;
	return (0);
}

int	pipe_case(t_pars *pars, int delimiter, t_splitted *tmp, t_ht *env)
{
	pars->last_neutral = NULL;
	pars->tmp_exec->argc = 0;
	ft_lstadd_back(pars->ret, ft_lstnew(pars->tmp_exec));
	pars->tmp_exec = init_exec();
	if (!pars->tmp_exec)
		return (1);
	if (!create_argv(pars->tmp_exec, pars->elements->next))
		return (free(pars->tmp_exec), 1);
	if (pars->tmp->content)
	{
		pars->tmp_exec->argv[0] = ft_strdup(tmp->content);
		free(tmp->content);
		tmp->content = NULL;
		pars->tmp_exec->argv[1] = NULL;
		pars->last_neutral = pars->tmp_exec;

	}
	else if (tmp->delimiter > -1 && tmp->delimiter != PIPE)
	{
		pars->delimiter = tmp->delimiter;
		pars->elements = pars->elements->next;
		pars->tmp = pars->elements->next->content;
		pars->tmp_exec->token_next = delimiter;
		pars->tmp_exec->argv[0] = ft_strdup(pars->tmp->content);
		free(pars->tmp->content);
		pars->tmp->content = NULL;
		pars->tmp_exec->argv[1] = NULL;
		pars->tmp = pars->elements->content;
	}
	return (0);
}

int	check_pipe_error(t_pars *pars, t_splitted *tmp, int delimiter, t_ht *env)
{
	if ((delimiter == PIPE && !pars->has_started) || (!tmp->content && delimiter == tmp->delimiter && delimiter == PIPE))
	{
		ft_lstadd_back(pars->ret, ft_lstnew(pars->tmp_exec));
		ft_putstr_fd("bash: syntax error near unexpected token `|`\n", 2);
		ht_deletef(env, "?");
		ht_insert(env, "?", ft_strdup("2"));
		return (1);
	}
	if ((!tmp->content && tmp->delimiter != -1 && delimiter != PIPE) || (!tmp->content && tmp->delimiter != -1 && !pars->elements->next))
	{
		if (tmp->delimiter == REDIRECT_INPUT)
			ft_putstr_fd("bash: syntax error near unexpected token `<`\n", 2);
		else if (tmp->delimiter == REDIRECT_OUTPUT)
			ft_putstr_fd("bash: syntax error near unexpected token `>`\n", 2);
		else if (tmp->delimiter == HEREDOC)
			ft_putstr_fd("bash: syntax error near unexpected token `<<`\n", 2);
		else if (tmp->delimiter == APPEND)
			ft_putstr_fd("bash: syntax error near unexpected token `>>`\n", 2);
		ht_deletef(env, "?");
		ht_insert(env, "?", ft_strdup("2"));
		return (free(pars->tmp_exec->argv), free(pars->tmp_exec), 1);
	}
	return (0);
}

int	norme_1(t_pars *pars, int delimiter)
{
	if (delimiter != PIPE && pars->elements)
	{
		ft_lstadd_back(pars->ret, ft_lstnew(pars->tmp_exec));
		pars->tmp_exec = init_exec();
		if (!pars->tmp_exec)
			return (1);
		pars->tmp_exec->argc = count_until_del(pars->elements);
		pars->elements = pars->elements->next;
	}
	else
	{
		pars->elements = pars->elements->next;
	}
	return (0);
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
