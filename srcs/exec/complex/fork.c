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

static bool	edit_flags(t_flags **flags, int i)
{
	if ((*flags)->pipe_index_changed)
	{
		(*flags)->pipe_index_changed = false;
		return (true);
	}
		
	if ((*flags)->pipe[(*flags)->pipe_index]->infile_index != -1)
		(*flags)->pipe[(*flags)->pipe_index]->infile_index++;

	if ((*flags)->pipe[(*flags)->pipe_index]->infile_index == (*flags)->pipe[(*flags)->pipe_index]->infile_nb)
		return ((*flags)->pipe[(*flags)->pipe_index]->infile_index = -1, true);

	if ((*flags)->pipe[(*flags)->pipe_index]->outfile_index != -1 && 
		(*flags)->pipe[(*flags)->pipe_index]->infile_index == -1)
		(*flags)->pipe[(*flags)->pipe_index]->outfile_index++;

	if ((*flags)->pipe[(*flags)->pipe_index]->outfile_index == (*flags)->pipe[(*flags)->pipe_index]->outfile_nb && 
		(*flags)->pipe[(*flags)->pipe_index]->infile_index == -1)
		(*flags)->pipe[(*flags)->pipe_index]->outfile_index = -1;

	if ((*flags)->pipe[(*flags)->pipe_index]->infile_index == -1 && (*flags)->pipe[(*flags)->pipe_index]->outfile_index == -1)
	{
		(*flags)->pipe_index += 1;
		(*flags)->pipe_index_changed = true;
	}
	
	return (true);
}

int	forking(t_flags *flags, t_ht *env, char **envp)
{
	int	i;
	int	status;

	i = 0;
	//if (flags->has_heredoc)////in child
	//	open_heredoc(flags);
	open_pipe(flags);
	while (i < flags->total_redir)
	{
		if (edit_flags(&flags, i))
		{
			printf("flags->pipe_index = %d\n", flags->pipe_index);////
			flags->pid[i] = fork();
		}
		g_pid = flags->pid[i];
		if (flags->pid[i] < 0)
			return(ft_printf("fork failed\n"), exit(1), 1);///exit if fork is not successful REAL EXIT
		else if (flags->pid[i] == 0)
			child_exec(flags, i, env, envp);
		i++;
	}
	i = 0;
	close_pipe(flags);
	while (i < flags->total_redir)
	{
		waitpid(flags->pid[i], &status, 0);
		g_pid = 0;
		i++;
	}
	printf("pipe_index: %d\n", flags->pipe_index);////
	return (WEXITSTATUS(status));
}

// handle the exit command
	//if (ft_strcmp(temp->argv[0], "exit") == 0) // exit
		//	return (exit_prog(&splitted, env));