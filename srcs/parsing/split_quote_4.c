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

void	handle_content_after(t_pars *pars, t_splitted *tmp)
{
	pars->tmp_exec->argv[0] = ft_strdup(tmp->content);
	free(tmp->content);
	tmp->content = NULL;
	pars->tmp_exec->argv[1] = NULL;
	pars->last_neutral = pars->tmp_exec;
}

int	pipe_case(t_pars *pars, t_splitted *tmp)
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
		handle_content_after(pars, tmp);
	}
	else if (tmp->delimiter > -1 && tmp->delimiter != PIPE && pars->tmp
		&& pars->elements->next && pars->elements->next->content)
		pipe_case2(pars, tmp);
	return (0);
}

int	check_pipe_error(t_pars *pars, t_splitted *tmp, int delimiter, t_ht *env)
{
	if ((delimiter == PIPE && !pars->has_started)
		|| (!tmp->content && delimiter == tmp->delimiter && delimiter == PIPE))
	{
		ft_lstadd_back(pars->ret, ft_lstnew(pars->tmp_exec));
		ft_putstr_fd("bash: syntax error near unexpected token `|`\n", 2);
		ht_deletef(env, "?");
		ht_insert(env, "?", ft_strdup("2"));
		return (1);
	}
	if ((!tmp->content && tmp->delimiter != -1 && delimiter != PIPE)
		|| (!tmp->content && tmp->delimiter != -1 && !pars->elements->next))
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
