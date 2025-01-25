/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2025/01/13 18:46:22 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signals_edit(int signo)
{
	if (signo == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_signals_heredoc(int signo)
{
	char	test[2];

	if (signo == SIGQUIT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signo == SIGINT)
	{
		test[0] = EOF;
		test[1] = 0;
		write(1, test, 2);
	}
}

void	handle_signals_cmd(int signo)
{
	if (signo == SIGINT)
	{
		printf("\n");
		rl_redisplay();
	}
}
