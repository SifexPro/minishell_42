/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:50:25 by pepie             #+#    #+#             */
/*   Updated: 2024/12/06 16:20:10 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_strings_quote(char const *str, t_list **elem, t_ht *env);

void	double_quote(char const *str, t_split_sh *sp, t_list **elem, t_ht *env)
{
	if (sp->i != sp->str_start && !sp->is_simp_quote && !sp->is_dbl_quote)
	{
		ft_lstadd_back(elem, create_str(
				ft_strndup((char *)(&str[sp->str_start]),
					sp->i - sp->str_start), false, env));
		sp->str_start = sp->i;
	}
	if (!sp->is_dbl_quote && !sp->is_simp_quote)
	{
		sp->is_dbl_quote = true;
		sp->quote_start = sp->i;
	}
	else if (sp->is_dbl_quote)
	{
		if (str[sp->i - 1] != '\"')
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
	if (sp->i != sp->str_start && !sp->is_simp_quote && !sp->is_dbl_quote)
	{
		ft_lstadd_back(elem, create_str(
				ft_strndup((char *)(&str[sp->str_start]),
					sp->i - sp->str_start), false, env));
		sp->str_start = sp->i;
	}
	if (!sp->is_simp_quote && !sp->is_dbl_quote)
	{
		sp->is_simp_quote = true;
		sp->quote_start = sp->i;
	}
	else if (sp->is_simp_quote)
	{
		if (str[sp->i - 1] != '\'')
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
		handle_meta(str, sp, elem);
	else if (str[sp->i] == '\"')
		double_quote(str, sp, elem, env);
	else if (str[sp->i] == '\'')
		simple_quote(str, sp, elem, env);
	else if ((str[sp->i] == ' ' && sp->quote_start == 0)
		|| str[sp->i + 1] == 0 || is_meta(str[sp->i]))
	{
		if (str[sp->str_start] == 0)
			return (1);
		if (str[sp->i + 1] == 0 && str[sp->i] != ' ')
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
	init_string_quote(sp);
	while (str[sp->i] || str[sp->i - 1])
	{
		res = loop_char(str, sp, elem, env);
		if (res > 0)
			break ;
		sp->i++;
	}
	if (sp->is_simp_quote || sp->is_dbl_quote)
		return (free(sp),
			printf("Quote non finished!\n"), 1);
	return (free(sp), 0);
}

t_list	*ft_split_quote(char const *str, t_ht *env)
{
	t_list		*elements;
	t_list		*ret;

	if (!str)
		return (NULL);
	elements = NULL;
	if (create_strings_quote(str, &elements, env))
		return (NULL);
	ret = NULL;
	if (sq_replace_and_free(elements, &ret))
		return (ft_lstclear(&ret, &free_splitted_wc), ft_lstclear(&elements, &free_splitted), NULL);
	if (elements)
		ft_lstclear(&elements, &free_splitted);
	return (ret);
}
