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

void	handle_pretext(t_list **elem, char *str, t_split_sh *sp, t_ht *env)
{
	char	*txt;
	char	*end;

	end = ft_strndup((char *)(&str[sp->quote_start + 1]),
			sp->i - sp->quote_start - 1);
	txt = ft_strjoin(sp->pretext, end);
	free(sp->pretext);
	sp->pretext = NULL;
	free(end);
	ft_lstadd_back(elem, create_str(txt, true, env));
}

void	concat_pretext(t_list **elem, char *str, t_split_sh *sp, t_ht *env)
{
	char	*txt;

	txt = ft_strjoin(sp->pretext, str);
	free(str);
	if (sp->pretext)
		free(sp->pretext);
	sp->pretext = txt;
}

void	handle_start(t_split_sh *sp, t_list **elem, char *str, t_ht *env)
{
	if (sp->i != sp->str_start || sp->pretext)
		concat_pretext(elem, ft_strndup((char *)(&str[sp->str_start]),
				sp->i - sp->str_start), sp, env);
	else
		sp->pretext = NULL;
	sp->quote_start = sp->i;
}

int	no_quote(char const *str, t_split_sh *sp, t_list **elem, t_ht *env)
{
	char	*s;
	int		j;

	if (str[sp->str_start] == 0)
		return (1);
	if (str[sp->i + 1] == 0 && str[sp->i] != ' ')
		sp->i++;
	s = ft_strndup((char *)(&str[sp->str_start]), sp->i - sp->str_start);
	if (is_valid_env(s) && !get_var_from_str(&s[1], env))
	{
		if (sp->prev_meta)
		{
			printf("bash: %s: ambiguous redirect\n", s);
			ht_deletef(env, "?");
			ht_insert(env, "?", ft_strdup("1"));
			return (free(sp), -2);
		}
	}
	else 
		ft_lstadd_back(elem, create_str(ft_strjoin(sp->pretext, s), false, env));
	if (sp->pretext)
		free(sp->pretext);
	sp->pretext = NULL;
	free(s);
	sp->str_start = sp->i + 1;
	sp->prev_meta = false;
	if (str[sp->i] == 0)
		return (1);
	return (0);
}

void	append_to_argv(t_pars *pars)
{
	char	**old_argv;
	int		i;
	int		old_argc;

	old_argv = pars->tmp_exec->argv;
	old_argc = pars->tmp_exec->argc;
	if (!old_argv)
		return ;
	pars->tmp_exec->argv = malloc(sizeof(char *) * (old_argc + 1));
	if (!pars->tmp_exec->argv)
		return ;
	pars->tmp_exec->argc = old_argc + 1;
	i = 0;
	while (old_argv && old_argv[i] && i < pars->tmp_exec->i)
	{
		pars->tmp_exec->argv[i] = old_argv[i];
		i++;
	}
	pars->tmp_exec->argv[i] = ((t_splitted *)pars->elements->content)->content;
	pars->tmp_exec->argv[i + 1] = NULL;
	pars->tmp_exec->i++;
	free(old_argv);
	pars->elements = pars->elements->next;
}
