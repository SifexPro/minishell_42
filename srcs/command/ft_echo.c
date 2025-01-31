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

bool	is_good_arg(char *str)
{
	int		i;

	i = 1;
	if (str[0] != '-')
		return (false);
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

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
		if (is_good_arg(argv[i]) && !text_display)
			has_n = true;
		else
		{
			text_display = true;
			ft_printf("%s", argv[i], argc);
		}
		if (i < argc - 1 && ((!has_n) || (has_n && i != 0)))
			ft_printf(" ");
		i++;
	}
	if (!has_n)
		ft_printf("\n");
	return (0);
}
