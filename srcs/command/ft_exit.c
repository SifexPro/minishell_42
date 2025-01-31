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

static long long	check_args(int argc, char **argv, long long last_status)
{
	int	i;

	i = -1;
	printf("argc = %d\n", argc);
	if (argc > 1)
	{
		while (argv[1][++i])
			if (!ft_isdigit(argv[1][i]))
				return (exec_error("numeric argument required", "exit"), 2);
		if (argc > 2)
			return (exec_error("too many arguments", "exit"), -1);
		printf("argv[1] = %s\n", argv[1]);
		return (ft_atoi(argv[1]));
	}
	return (last_status);
}

int	ft_exit(int argc, char **argv, long long last_status)
{
	long long	exit_status;

	printf("argc = %d\n", argc);
	printf("argv[0] = %s\n", argv[0]);
	printf("argv[1] = %s\n", argv[1]);
	if (last_status != -1)
		ft_putstr_fd("exit\n", 2);
	else
		last_status = 0;
	exit_status = check_args(argc, argv, last_status);
	return (exit_status);
}
