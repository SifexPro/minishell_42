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
	ft_printf("term signal\n");
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

	ft_printf("heredoc signal\n");
	if (signo == SIGQUIT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signo == SIGINT)
	{
	}
}

void	handle_signals_cmd(int signo)
{
	ft_printf("cmd signal\n");
	if (signo == SIGINT && g_pid != -1)
	{
		kill(g_pid, SIGQUIT);
	}
}

void	handle_sigint(int sig_num)
{
	if (sig_num == SIGINT)
	{
		printf("sigint %d\n", g_pid);
		write(1, "\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signal_ctrl_c(void)
{
	struct sigaction	ctrl_c;

	ctrl_c.sa_handler = handle_sigint;
	ctrl_c.sa_flags = SA_RESTART;
	sigemptyset(&ctrl_c.sa_mask);
	sigaction(SIGINT, &ctrl_c, NULL);
}

void	signal_ctrl_backslash(void)
{
	struct sigaction	ctrl_back_slash;

	ctrl_back_slash.sa_handler = SIG_IGN;
	ctrl_back_slash.sa_flags = SA_RESTART;
	sigemptyset(&ctrl_back_slash.sa_mask);
	sigaction(SIGQUIT, &ctrl_back_slash, NULL);
}

void	setup_cmd_signals(void)
{
	ft_printf("setup cmd signals\n");
	/* if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
	if (signal(SIGQUIT, handle_signals_cmd) == SIG_ERR)
		printf("failed to register interrupts with kernel\n"); */
	signal_ctrl_backslash();
	signal_ctrl_c();
}

void	setup_heredoc_signals(void)
{
	/* ft_printf("setup heredoc signals\n");
	if (signal(SIGINT, handle_signals_heredoc) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
	if (signal(SIGQUIT, handle_signals_heredoc) == SIG_ERR)
		printf("failed to register interrupts with kernel\n"); */
}

void	setup_term_signals(void)
{
	/* ft_printf("setup term signals\n");
	if (signal(SIGINT, handle_signals_term) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
	if (signal(SIGQUIT, handle_signals_term) == SIG_ERR)
		printf("failed to register interrupts with kernel\n"); */
}
