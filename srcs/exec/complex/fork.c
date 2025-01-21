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

void	forking(t_flags *flags, t_ht *env, char **envp)
{
	int	i;
	int	status;
	
	ft_printf("Forking\n");

	i = 0;
	if (flags->has_heredoc)
		open_heredoc(flags);
	open_pipe(flags);
	while (i < flags->cmd_count)
	{
		flags->pid[i] = fork();
		if (flags->pid[i] < 0)
			exit(1);///exit if fork is not successful
		else if (flags->pid[i] == 0)
			child_exec(flags, i, env, envp);
		i++;
	}
	i = 0;
	close_pipe(flags);
	while (i < flags->cmd_count)
	{
		waitpid(flags->pid[i], &status, 0);
		i++;
	}
	printf("status : %d\n", WEXITSTATUS(status));
}

// handle the exit command
	//if (ft_strcmp(temp->argv[0], "exit") == 0) // exit
		//	return (exit_prog(&splitted, env));