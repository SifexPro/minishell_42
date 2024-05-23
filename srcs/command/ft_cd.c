/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:14:35 by pepie             #+#    #+#             */
/*   Updated: 2024/05/23 03:52:30 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(int argc, char **argv)
{
	int	ret;

	if (argc != 1)
		return (ft_printf("cd: too many arguments\n"), 1);
	ret = chdir(argv[0]);
	if (ret == -1)
		return (ft_printf("cd: no such file or directory: %s\n", argv[0]), 1);
	return (0);
}
