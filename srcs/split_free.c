/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:25:19 by Sifex             #+#    #+#             */
/*   Updated: 2025/02/06 16:25:42 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_splitted(void *v)
{
	t_splitted	*tmp_splitted;

	tmp_splitted = v;
	if (tmp_splitted)
	{
		free(tmp_splitted);
	}
}

void	free_splitted_c(void *v)
{
	t_splitted	*tmp_splitted;

	tmp_splitted = v;
	if (tmp_splitted)
	{
		if (tmp_splitted->content)
		{
			free(tmp_splitted->content);
		}
		free(tmp_splitted);
	}
}

void	free_splitted_wc(void *v)
{
	t_exec	*tmp_exec;
	int		i;

	tmp_exec = v;
	i = 0;
	if (tmp_exec)
	{
		if (tmp_exec->argv)
		{
			while (tmp_exec->argv[i])
			{
				free(tmp_exec->argv[i]);
				i++;
			}
			free(tmp_exec->argv);
		}
		free(tmp_exec);
	}
}
