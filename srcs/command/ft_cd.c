/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:14:35 by pepie             #+#    #+#             */
/*   Updated: 2024/09/24 12:41:55 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(int argc, char **argv, t_ht *env)
{
	int	ret;

	if (argc > 1)
		return (ft_printf("cd: too many arguments\n"), 1);
	if (argc == 0)
	{
		ret = chdir(ht_search(env, "HOME"));
		if (ret == -1)
			return (ft_printf("cd: no such file or directory: %s\n",
					ht_search(env, "HOME")), 1);
		return (0);
	}
	ret = chdir(argv[0]);
	if (ret == -1)
		return (ft_printf("cd: no such file or directory: %s\n", argv[0]), 1);
	return (0);
}
