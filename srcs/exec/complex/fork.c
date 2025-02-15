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

static int	wait_child(int pipe_nb, pid_t *pid)
{
	int	i;
	int	status;

	i = 0;
	while (i < pipe_nb)
	{
		waitpid(pid[i], &status, 0);
		i++;
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (130);
	return (WEXITSTATUS(status));
}

static void	exit_fork(t_flags *flags, t_list **splitted, t_ht *env)
{
	free_flags(flags);
	exit_prog(splitted, env, 1);
}

bool	edit_flags(t_flags **flags)
{
	(*flags)->pipe_index++;
	return (true);
}

int	forking(t_flags *flags, t_list *splitted, t_ht *env, char **envp)
{
	int	i;

	i = 0;
	if (!open_pipe(flags))
		return (exit_fork(flags, &splitted, env), 1);
	while (i < flags->pipe_nb)
	{
		if (edit_flags(&flags))
			flags->pid[i] = fork();
		if (flags->pid[i] < 0)
			return (exit_fork(flags, &splitted, env), 1);
		else if (flags->pid[i] == 0)
			child_exec(flags, i, env, envp);
		i++;
	}
	close_pipe(flags);
	return (wait_child(flags->pipe_nb, flags->pid));
}
