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
		if (tmp->is_delimiter && tmp->delimiter != HEREDOC && tmp->delimiter != REDIRECT_INPUT)
			break ;
		e_tmp = e_tmp->next;
		i++;
	}
	return (i);
}

t_exec	*init_exec(void)
{
	t_exec	*tmp_exec;

	tmp_exec = malloc(sizeof(t_exec));
	tmp_exec->argv = NULL;
	tmp_exec->argc = 0;
	tmp_exec->token_next = -1;
	return (tmp_exec);
}

int	sq_replace_and_free(t_list *elements, t_list **ret)
{
	t_list		*e_tmp;
	t_splitted	*tmp;
	int			i;
	t_exec		*tmp_exec;
	int			can_error;

	i = 0;
	tmp_exec = init_exec();
	if (!tmp_exec)
		return (1);
	tmp_exec->argc = count_until_del(elements);
 	tmp_exec->argv = malloc(sizeof(char *) * (tmp_exec->argc + 1));
	tmp_exec->token_next = -1;
	if (!tmp_exec->argv)
		return (1);
	while (elements)
	{
		tmp = elements->content;
		can_error = false;
		if (tmp->is_delimiter)
		{
			tmp_exec->argv[i] = NULL;
			if (!elements->next)
				can_error = true;
			else
			{
				if (i != 0)
				{
					tmp_exec->token_next = -1;
					ft_lstadd_back(ret, ft_lstnew(tmp_exec));
					tmp_exec = init_exec();
					if (!tmp_exec)
						return (1);
					tmp_exec->argc = count_until_del(elements);
					tmp_exec->argv = malloc(sizeof(char *) * (tmp_exec->argc + 1));
					tmp_exec->token_next = tmp->delimiter;
					if (!tmp_exec->argv)
						return (1);
					i = 0;
				}
				else
				{
					tmp_exec->token_next = tmp->delimiter;
				}
				tmp = elements->next->content;
				tmp_exec->argv[i] = tmp->content;
				tmp_exec->argv[i + 1] = NULL;
				elements = elements->next;
				tmp = elements->content;
			}
			ft_lstadd_back(ret, ft_lstnew(tmp_exec));
			elements = elements->next;
			tmp_exec = init_exec();
			if (!tmp_exec)
				return (1);
			tmp_exec->token_next = -1;
			tmp_exec->argc = count_until_del(elements);
			if (elements == NULL)
			{ 
				if (can_error)
				{
					printf("bash: syntax error near unexpected token `newline'\n");
					free(tmp_exec);
					return (1);
				}
				else
				{
					can_error = true;
					break;
				}
			}
			tmp_exec->argv = malloc(sizeof(char *) * (tmp_exec->argc + 1));
			tmp = elements->content;
			if (!tmp_exec->argv)
				return (1);
			i = 0;
			tmp = elements->content;
		}
		//ft_printf("tmp_exec->argv[%d] = %s\n", i, tmp->content);
		tmp_exec->argv[i] = tmp->content;
		e_tmp = elements->next;
		i++;
		elements = e_tmp;
	}
	if (tmp_exec->argv)
		tmp_exec->argv[i] = NULL;
	tmp_exec->token_next = -1;
	if (!can_error)
		ft_lstadd_back(ret, ft_lstnew(tmp_exec));
	return (0);
}

