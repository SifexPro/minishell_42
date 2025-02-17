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
	if (!sp->is_simp_quote && !sp->is_dbl_quote)
	{
		handle_start(sp, (char *)str, env);
		sp->is_dbl_quote = true;
	}
	else if (sp->is_dbl_quote)
	{
		if (str[sp->i + 1] == ' ' || str[sp->i + 1] == 0)
		{
			if (sp->pretext)
				handle_pretext_env(elem, (char *)str, sp, env);
			else
				ft_lstadd_back(elem, create_str(
						ft_strndup((char *)(&str[sp->quote_start + 1]),
							sp->i - sp->quote_start - 1), false, env));
		}
		else
			concat_pretext(handle_expansion(ft_strndup((char *)(&str[sp->quote_start + 1]),
					sp->i - sp->quote_start - 1), env), sp);
		sp->str_start = sp->i + 1;
		sp->quote_start = 0;
		sp->is_dbl_quote = false;
	}
	sp->prev_meta = false;
}

void	simple_quote(char const *str, t_split_sh *sp, t_list **elem, t_ht *env)
{
	if (!sp->is_simp_quote && !sp->is_dbl_quote)
	{
		handle_start(sp, (char *)str, env);
		sp->is_simp_quote = true;
	}
	else if (sp->is_simp_quote)
	{
		if (str[sp->i + 1] == ' ' || str[sp->i + 1] == 0)
		{
			if (sp->pretext)
				handle_pretext(elem, (char *)str, sp, env);
			else
				ft_lstadd_back(elem, create_str(
						ft_strndup((char *)(&str[sp->quote_start + 1]),
							sp->i - sp->quote_start - 1), true, env));
		}
		else
			concat_pretext(ft_strndup((char *)(&str[sp->quote_start + 1]),
					sp->i - sp->quote_start - 1), sp);
		sp->str_start = sp->i + 1;
		sp->is_simp_quote = false;
		sp->quote_start = 0;
	}
	sp->prev_meta = false;
}

int	loop_char(char const *str, t_split_sh *sp, t_list **elem, t_ht *env)
{
	int	resp;

	if (str[sp->i] == ' ' && sp->str_start == sp->i)
		sp->str_start++;
	else if (is_meta(str[sp->i]) && !sp->is_dbl_quote && !sp->is_simp_quote)
		return (handle_meta(str, sp, elem, env));
	else if (str[sp->i] == '\"')
		double_quote(str, sp, elem, env);
	else if (str[sp->i] == '\'')
		simple_quote(str, sp, elem, env);
	else if ((str[sp->i] == ' ' && sp->quote_start == 0)
		|| (str[sp->i + 1] == 0)
		|| (is_meta(str[sp->i]) && !sp->is_dbl_quote && !sp->is_simp_quote))
	{
		resp = no_quote(str, sp, elem, env);
		if (resp != 0)
			return (resp);
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
	while (str[sp->i] || (str[sp->i - 1] && str[sp->i]))
	{
		res = loop_char(str, sp, elem, env);
		if (res == -2)
		{
			ht_deletef(env, "?");
			ht_insert(env, "?", "1");
			return (free(sp), 1);
		}
		if (res > 0)
			break ;
		sp->i++;
	}
	if (sp->pretext)
		free(sp->pretext);
	if (sp->is_simp_quote || sp->is_dbl_quote)
		return (free(sp), printf("Quote non finished!\n"), 1);
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
		return (ft_lstclear(&elements, &free_splitted_c), NULL);
	ret = NULL;
	if (sq_replace_and_free(elements, &ret, env))
	{
		return (ft_lstclear(&elements, &free_splitted_c),
			ft_lstclear(&ret, &free_splitted_wc), NULL);
	}
	ft_lstclear(&elements, &free_splitted);
	fix_argc(ret);
	return (ret);
}
