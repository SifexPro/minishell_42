/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:48:15 by pepie             #+#    #+#             */
/*   Updated: 2025/01/22 15:33:28 by Sifex            ###   ########.fr       */
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
		{
			has_n = true;
		}
		else
		{
			text_display = true;
			ft_printf("%s", argv[i], argc);
		}
		if (i < argc - 1 && ((!has_n) || (has_n && i != 0)))////argc - 1 - hotfix (espace en trop à la fin sinon)
			ft_printf(" ");
		i++;
	}
	if (!has_n)
		ft_printf("\n");
	return (0);
}
