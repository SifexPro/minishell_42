/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:20:05 by Sifex             #+#    #+#             */
/*   Updated: 2025/01/30 17:21:18 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_sign(char c)
{
	return (c == '-' || c == '+');
}

long	ft_exit(int argc, char **argv, long long last_status)
{
	int			i;
	bool 		has_sign;

	i = -1;
	if (last_status != -1)
		ft_putstr_fd("exit\n", 2);
	else
		last_status = 0;
	if (argc > 1)
	{
		has_sign = is_sign(argv[1][0]);
		if (has_sign)
			i = 0;
		while (argv[1][++i])
			if (!ft_isdigit(argv[1][i]))
				return (exec_error("numeric argument required", "exit"), 2);
		if (ft_strcmp(ft_ltoa(ft_atol(argv[1])), &argv[1][has_sign]) &&
			ft_strcmp(ft_ltoa(ft_atol(argv[1])), argv[1]))
			return (exec_error("numeric argument required", "exit"), 2);
		if (argc > 2)
			return (exec_error("too many arguments", "exit"), 1);
		return (ft_atol(argv[1]));
	}
	return (last_status);
}
