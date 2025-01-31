/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:14:35 by pepie             #+#    #+#             */
/*   Updated: 2025/01/31 17:52:22 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(int argc, char **argv, t_ht *env)
{
	int	ret;

	if (argc > 2)
		return (ft_printf("cd: too many arguments\n"), 1);
	if (argc == 1)
	{
		ret = chdir(ht_search(env, "HOME"));
		if (ret == -1)
			return (ft_printf("cd: No such file or directory: %s\n",
					ht_search(env, "HOME")), 1);
		return (0);
	}
	ret = chdir(argv[1]);
	if (ret == -1)
		return (ft_printf("cd: No such file or directory: %s\n", argv[1]), 1);
	return (0);
}
