/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:53:35 by polepie           #+#    #+#             */
/*   Updated: 2024/12/06 17:00:47 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_meta(char c)
{
	if (c == '&')
		return (true);
	if (c == '|')
		return (true);
	if (c == '<')
		return (true);
	if (c == '>')
		return (true);
	if (c == ';')
		return (true);
	return (false);
}

bool	handle_meta_2(
	char const *str,
	t_split_sh *sp,
	t_list **elem,
	t_splitted *content
)
{
	if (strncmp((char *)(&str[sp->i]), "|", 1) == 0)
	{
		content->delimiter = PIPE;
		sp->str_start = sp->i + 1;
	}
	else if (strncmp((char *)(&str[sp->i]), "<", 1) == 0)
	{
		content->delimiter = REDIRECT_INPUT;
		sp->str_start = sp->i + 1;
	}
	else if (strncmp((char *)(&str[sp->i]), ">", 1) == 0)
	{
		content->delimiter = REDIRECT_OUTPUT;
		sp->str_start = sp->i + 1;
	}
	else
	{
		sp->str_start = sp->i + 1;
		return (true);
	}
	return (false);
}

bool	handle_meta_3(
	char const *str,
	t_split_sh *sp,
	t_list **elem,
	t_splitted *content
)
{
	if (strncmp((char *)(&str[sp->i]), "<<", 2) == 0)
	{
		content->delimiter = HEREDOC;
		sp->str_start = sp->i + 2;
		return (false);
	}
	else if (strncmp((char *)(&str[sp->i]), ">>", 2) == 0)
	{
		content->delimiter = APPEND;
		sp->str_start = sp->i + 2;
		return (false);
	}
	return (handle_meta_2(str, sp, elem, content));
}

int	handle_meta(char const *str, t_split_sh *sp, t_list **elem, t_ht *env)
{
	t_splitted	*content;
	int			resp;

	if (sp->i != sp->str_start)
	{
		resp = no_quote(str, sp, elem, env);
		if (resp != 0)
			return (resp);
	}
	content = malloc(sizeof(t_splitted));
	if (!content)
		return (1);
	content->content = NULL;
	content->is_delimiter = true;
	content->delimiter = -1;
	if (handle_meta_3(str, sp, elem, content))
		return (1);
	ft_lstadd_back(elem, ft_lstnew(content));
	while (ft_str_is_whitespace(str[sp->str_start]))
		sp->str_start++;
	sp->i = sp->str_start - 1;
	sp->prev_meta = true;
	return (0);
}
