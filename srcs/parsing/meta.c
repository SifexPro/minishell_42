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

int	handle_meta(char const *str, t_split_sh *sp, t_list **elem)
{
	t_splitted	*content;

	content = malloc(sizeof(t_splitted));
	if (!content)
		return (0);
	content->is_delimiter = true;
	if (strncmp((char *)(&str[sp->str_start]), "<<", 2) == 0)
	{
		content->delimiter = HEREDOC;
		sp->str_start = sp->i + 2;
	}
	else if (strncmp((char *)(&str[sp->str_start]), ">>", 2) == 0)
	{
		content->delimiter = APPEND;
		sp->str_start = sp->i + 2;
	}
	else if (strncmp((char *)(&str[sp->str_start]), "|", 1) == 0)
	{
		content->delimiter = PIPE;
		sp->str_start = sp->i + 1;
	}
	else if (strncmp((char *)(&str[sp->str_start]), "<", 1) == 0)
	{
		content->delimiter = REDIRECT_INPUT;
		sp->str_start = sp->i + 1;
	}
	else if (strncmp((char *)(&str[sp->str_start]), ">", 1) == 0)
	{
		content->delimiter = REDIRECT_OUTPUT;
		sp->str_start = sp->i + 1;
	}
	else
	{
		sp->str_start = sp->i + 1;
		return (0);
	}
	ft_lstadd_back(elem, ft_lstnew(content));
	while (ft_str_is_whitespace(str[sp->str_start]))
		sp->str_start++;
	sp->i = sp->str_start;
	return (2);
}
