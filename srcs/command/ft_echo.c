/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:48:15 by pepie             #+#    #+#             */
/*   Updated: 2024/12/03 17:40:41 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(int argc, char **argv)
{
	int		i;
	bool	has_n;
	bool	text_display;

	i = 0;
	has_n = false;
	text_display = false;
	argv++;
	argc--;
	while (argv[i] && i < argc)
	{
		if (ft_strncmp(argv[i], "-n", 2) == 0 && !text_display)
			has_n = true;
		else
		{
			text_display = true;
			ft_printf("%s", argv[i], argc);
		}
		i++;
		if (i < argc)
			ft_printf(" ");
	}
	if (!has_n)
		ft_printf("\n");
	return (0);
}
