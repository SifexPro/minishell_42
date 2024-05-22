/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:18:44 by pepie             #+#    #+#             */
/*   Updated: 2024/05/22 16:23:48 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strarr_len(char **input)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	return (i);
}

void	free_split(char **splitted)
{
	int	i;

	i = 0;
	while (splitted[i] != NULL)
	{
		ft_printf("%d\n", i);
		free(splitted[i]);
		i++;
	}
	ft_printf("NON\n");
}
