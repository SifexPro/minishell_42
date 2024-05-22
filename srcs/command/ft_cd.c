/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:14:35 by pepie             #+#    #+#             */
/*   Updated: 2024/05/22 13:45:21 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(int argc, char **argv)
{
	int	ret;

	ft_printf("%d", argc);
	if (argc != 1)
		return (ft_printf("cd: too many arguments\n"), 1);
	ret = chdir(argv[0]);
	if (ret == -1)
		ft_printf("cd: no such file or directory: %s\n", argv[0]);
	return (0);
}
