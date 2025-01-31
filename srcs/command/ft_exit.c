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

int	ft_exit(int argc, char **argv, int status)
{
	int		exit_status;

	exit_status = 0;
	if (argc > 1)
		exit_status = ft_atoi(argv[1]);
	exit(exit_status);
	return (exit_status);
}