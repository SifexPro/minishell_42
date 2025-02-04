/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:18:12 by pepie             #+#    #+#             */
/*   Updated: 2024/12/03 15:54:49 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_env(char *str)
{
	int	i;

	i = 1;
	if (str[0] != '$')
		return (false);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (str[i] != 0)
		return (false);
	return (true);
}

t_list	*create_str(char *str, bool is_simple_quote, t_ht *env)
{
	t_splitted	*content;

	content = malloc(sizeof(t_splitted));
	if (!content)
		return (NULL);
	content->is_delimiter = false;
	if (!is_simple_quote)
	{
		content->content = handle_expansion(str, env);
	}
	else
		content->content = str;
	return (ft_lstnew(content));
}
