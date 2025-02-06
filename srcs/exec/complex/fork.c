/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:56:14 by Sifex             #+#    #+#             */
/*   Updated: 2025/01/16 15:56:14 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_child(int total_redir, pid_t *pid)
{
	int	i;
	int	status;

	i = 0;
	while (i < total_redir)
	{
		waitpid(pid[i], &status, 0);
		i++;
	}
	return (WEXITSTATUS(status));
}

static void	exit_fork(t_flags *flags, t_list **splitted, t_ht *env)
{
	free_flags(flags);
	exit_prog(splitted, env, 1);
}

int	forking(t_flags *flags, t_list *splitted, t_ht *env, char **envp)
{
	int	i;

	i = 0;
	if (!open_pipe(flags))
		return (exit_fork(flags, &splitted, env), 1);
	if (!check_exec(&flags))
		return (exit_fork(flags, &splitted, env), 1);
	while (i < flags->total_redir)
	{
		if (edit_flags(&flags))
			flags->pid[i] = fork();
		g_pid = flags->pid[i];
		if (flags->pid[i] < 0)
			return (exit_fork(flags, &splitted, env), 1);
		else if (flags->pid[i] == 0)
			child_exec(flags, i, env, envp);
		i++;
	}
	close_pipe(flags);
	return (wait_child(flags->total_redir, flags->pid));
}
