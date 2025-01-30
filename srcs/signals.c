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

void	handle_signals_term(int signo)
{
	ft_printf("signo = %d\n", g_pid);
	if (signo == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", 2);
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
		ft_printf("signo = %d\n", g_pid);
		g_pid = -1;
		write(1, "\n", 4);
		//kill(g_pid, SIGINT);
	}
}

void	handle_signals_cmd(int signo)
{
	if (signo == SIGINT)
	{
		ft_printf("signo = %d\n", g_pid);
	}
}

void	setup_cmd_signals(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
	if (signal(SIGQUIT, handle_signals_cmd) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
}

void	setup_heredoc_signals(void)
{
	if (signal(SIGINT, handle_signals_heredoc) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
	if (signal(SIGQUIT, handle_signals_heredoc) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
}

void	setup_term_signals(void)
{
	if (signal(SIGINT, handle_signals_term) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
	if (signal(SIGQUIT, handle_signals_term) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
}
