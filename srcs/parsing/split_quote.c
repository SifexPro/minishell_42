/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: polepie <polepie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:50:25 by pepie             #+#    #+#             */
/*   Updated: 2024/05/24 19:11:15 by polepie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_strings_quote(char const *str, t_list **elem, t_ht *env);

void	double_quote(char const *str, t_split_sh *sp, t_list **elem, t_ht *env)
{
	if (!sp->is_dbl_quote && !sp->is_simp_quote)
	{
		sp->is_dbl_quote = true;
		sp->quote_start = sp->i;
	}
	else if (sp->is_dbl_quote)
	{
		ft_lstadd_back(elem, create_str(
				ft_strndup((char *)(&str[sp->quote_start + 1]),
					sp->i - sp->quote_start - 1), false, env));
		sp->str_start = sp->i + 1;
		sp->quote_start = 0;
		sp->is_dbl_quote = false;
	}
}

void	simple_quote(char const *str, t_split_sh *sp, t_list **elem, t_ht *env)
{
	if (!sp->is_simp_quote && !sp->is_dbl_quote)
	{
		sp->is_simp_quote = true;
		sp->quote_start = sp->i;
	}
	else if (sp->is_simp_quote)
	{
		ft_lstadd_back(elem, create_str(
				ft_strndup((char *)(&str[sp->quote_start + 1]),
					sp->i - sp->quote_start - 1), true, env));
		sp->str_start = sp->i + 1;
		sp->quote_start = 0;
		sp->is_simp_quote = false;
	}
}

int	loop_char(char const *str, t_split_sh *sp, t_list **elem, t_ht *env)
{
	if (str[sp->i] == ' ' && sp->str_start == sp->i)
		sp->str_start++;
	else if (is_meta(str[sp->i]))
		return (handle_meta(str, sp, elem, env));
	else if (str[sp->i] == '\"')
		double_quote(str, sp, elem, env);
	else if (str[sp->i] == '\'')
		simple_quote(str, sp, elem, env);
	else if ((str[sp->i] == ' ' && sp->quote_start == 0)
		|| str[sp->i + 1] == 0 || is_meta(str[sp->i]))
	{
		if (str[sp->i + 1] == 0)
			sp->i++;
		ft_lstadd_back(elem, create_str(
				ft_strndup((char *)(&str[sp->str_start]),
					sp->i - sp->str_start), false, env));
		sp->str_start = sp->i + 1;
		if (str[sp->i] == 0)
			return (1);
	}
	return (0);
}

int	create_strings_quote(char const *str, t_list **elem, t_ht *env)
{
	t_split_sh	*sp;
	int			res;

	sp = malloc(sizeof(t_split_sh));
	if (!sp)
		return (printf("Malloc error"), 1);
	sp->i = 0;
	sp->str_start = 0;
	sp->quote_start = 0;
	sp->is_simp_quote = false;
	sp->is_dbl_quote = false;
	while (str[sp->i])
	{
		res = loop_char(str, sp, elem, env);
		if (res > 0)
			break ;
		sp->i++;
	}
	if (sp->is_simp_quote || sp->is_dbl_quote)
		return (free(sp), ft_lstclear(elem, &free),
			printf("Quote non finished!\n"), 1);
	if (res == 2)
		return (hashtable_insert(env, "next_line", ft_strdup((char *)(&str[sp->str_start + 1]))),
			free(sp), 0);
	return (free(sp), 0);
}

char	**ft_split_quote(char const *str, t_ht *env)
{
	t_list		*elements;
	char		*tmp;
	char		**ret;
	int			i;

	i = 0;
	if (!str)
		return (NULL);
	elements = NULL;
	if (create_strings_quote(str, &elements, env))
		return (NULL);
	ret = malloc(sizeof(char *) * (ft_lstsize(elements) + 1));
	if (!ret)
		return (NULL);
	while (elements)
	{
		tmp = elements->content;
		ret[i] = tmp;
		i++;
		elements = elements->next;
	}
	ret[i] = NULL;
	return (ret);
}
