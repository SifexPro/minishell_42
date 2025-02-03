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
	tmp_exec->argv = NULL;
	tmp_exec->argc = 0;
	tmp_exec->token_next = -1;
	tmp_exec->i = 0;
	return (tmp_exec);
}

void	append_to_argv(t_exec *exec, char *str)
{
	char	**old_argv;
	int		i;
	int		old_argc;

	old_argv = exec->argv;
	old_argc = exec->argc;
	exec->argv = malloc(sizeof(char *) * (old_argc + 1));
	if (!exec->argv)
		return ;
	exec->argc = old_argc + 1;
	i = 0;
	while (old_argv[i])
	{
		exec->argv[i] = old_argv[i];
		i++;
	}
	exec->argv[exec->i] = str;
	exec->argv[exec->i + 1] = NULL;
	exec->i++;
	free(old_argv);
}


int	sq_replace_and_free(t_list *elements, t_list **ret, t_ht *env)
{
	t_list		*e_tmp;
	t_splitted	*tmp;
	t_exec		*tmp_exec;
	int			can_error;
	int			delimiter;
	t_exec		*last_neutral;

	tmp_exec = init_exec();
	if (!tmp_exec)
		return (1);
	tmp_exec->argc = count_until_del(elements);
 	tmp_exec->argv = malloc(sizeof(char *) * (tmp_exec->argc + 1));
	if (!tmp_exec->argv)
		return (1);
	can_error = false;
	last_neutral = NULL;
	while (elements)
	{
		tmp = elements->content;
		can_error = false;
		if (tmp->is_delimiter)
		{
			delimiter = tmp->delimiter;
			tmp_exec->argv[tmp_exec->i] = NULL;
			if (!elements->next)
				can_error = true;
			else
			{
				if (tmp_exec->i != 0)
				{
					ft_lstadd_back(ret, ft_lstnew(tmp_exec));
					tmp_exec = init_exec();
					if (!tmp_exec)
						return (1);
					tmp_exec->argc = count_until_del(elements);
					if (delimiter != PIPE)
						tmp_exec->argv = malloc(sizeof(char *) * (tmp_exec->argc + 1));
					tmp_exec->token_next = tmp->delimiter;
					/* if (!tmp_exec->argv)
						return (1); */
					tmp_exec->i = 0;
				}
				else
					tmp_exec->token_next = tmp->delimiter;
				tmp = elements->next->content;
				if (delimiter == PIPE)
				{
					last_neutral = NULL;
					tmp_exec->argc = 0;
					ft_lstadd_back(ret, ft_lstnew(tmp_exec));
					tmp_exec = init_exec();
					if (!tmp_exec)
						return (1);
					tmp_exec->argc = count_until_del(elements->next);
					tmp_exec->argv = malloc(sizeof(char *) * (tmp_exec->argc + 1));
					if (tmp->content)
						tmp_exec->argv[0] = tmp->content;
					else if (tmp->delimiter > -1)
					{
						delimiter = tmp->delimiter;
						elements = elements->next;
						tmp = elements->next->content;
						tmp_exec->token_next = delimiter;
						tmp_exec->argv[0] = tmp->content;
						tmp = elements->content;
					}
				}
				else
				{
					tmp_exec->argv[tmp_exec->i] = tmp->content;
					tmp_exec->argv[tmp_exec->i + 1] = NULL;
				}
				elements = elements->next;
				tmp = elements->content;
				tmp_exec->i++;
			}
			if (!tmp->content && tmp->delimiter != -1 && delimiter != PIPE)
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
				return (free(tmp_exec->argv), free(tmp_exec), 1);
			}
			if (delimiter != PIPE && elements)
			{
				ft_lstadd_back(ret, ft_lstnew(tmp_exec));
				tmp_exec = init_exec();
				if (!tmp_exec)
					return (1);
				tmp_exec->argc = count_until_del(elements);
				elements = elements->next;
			}
			else
			{
				elements = elements->next;
			}
			if (elements == NULL)
			{
				if (can_error)
				{
					ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);////
					free(tmp_exec);
					ht_deletef(env, "?");
					ht_insert(env, "?", ft_strdup("2"));
					return (1);
				}
				else
				{
					if (delimiter != PIPE)
						can_error = true;
					break;
				}
			}
			if (delimiter != PIPE)
			{
				tmp_exec->argv = malloc(sizeof(char *) * (tmp_exec->argc + 1));
				tmp = elements->content;
				if (!tmp_exec->argv)
					return (1);
				tmp_exec->i = 0;
				tmp = elements->content;
				continue;
			}
			else
			{
				tmp = elements->content;
				continue;
			}
		}
		else
			delimiter = -1;
		if (last_neutral)
		{
			append_to_argv(last_neutral, tmp->content);
			elements = elements->next;
			continue;
		}
		else if (tmp_exec->token_next == -1)
		{
			last_neutral = tmp_exec;
		}
		tmp_exec->argv[tmp_exec->i] = tmp->content;
		e_tmp = elements->next;
		tmp_exec->i++;
		elements = e_tmp;
	}
	if (tmp_exec->argv)
		tmp_exec->argv[tmp_exec->i] = NULL;
	if (!can_error && tmp_exec->i > 0) {
		ft_lstadd_back(ret, ft_lstnew(tmp_exec));
	} else {
		free(tmp_exec->argv);
		free(tmp_exec);
	}
	return (0);
}
