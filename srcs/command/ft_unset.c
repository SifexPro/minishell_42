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

int ft_unset(int argc, char **argv, t_ht *env)
{
	int		i;
	char	*key;

	if (argc == 1)
	{
		printf("unset: no arguments\n");
		return (1);
	}

	i = 1;
	while (argv[i])
	{
		key = ft_strdup(argv[i]);
		ft_strtoupper(key);
		ht_delete(env, key);
		free(key);
		i++;
	}
	return (0);
}