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
	printf("START\n");////
	(*flags)->pipe[(*flags)->pipe_index]->index++;
	//printf("\n[!] flags->pipe[pipe_index]->index = %d\n", (*flags)->pipe[(*flags)->pipe_index]->index);////
	//printf("[!] flags->pipe[pipe_index]->index_max = %d\n", (*flags)->pipe[(*flags)->pipe_index]->index_max);////

	if ((*flags)->pipe[(*flags)->pipe_index]->index >= (*flags)->pipe[(*flags)->pipe_index]->index_max)
	{
		printf("ADDED PIPE INDEX\n");////
		//printf("[!] chnage pipe_index\n");////
		(*flags)->pipe_index += 1;
		(*flags)->pipe[(*flags)->pipe_index]->index = 0;
		//(*flags)->pipe[(*flags)->pipe_index]->infile_index = 0;
		//return (true);
		//(*flags)->pipe_index_changed = true;
	}

	//ls | < Makefile wc -l > test
	printf("pipe_index: %d\n", (*flags)->pipe_index);////
	printf("infile_index: %d\n", (*flags)->pipe[(*flags)->pipe_index]->infile_index);////
	printf("outfile_index: %d\n", (*flags)->pipe[(*flags)->pipe_index]->outfile_index);////

	if ((*flags)->pipe[(*flags)->pipe_index]->outfile_nb > 0 && (*flags)->pipe[(*flags)->pipe_index]->outfile_index < (*flags)->pipe[(*flags)->pipe_index]->outfile_nb
		&& (
			(*flags)->pipe[(*flags)->pipe_index]->infile_index + 1 >= (*flags)->pipe[(*flags)->pipe_index]->infile_nb 
			//|| (*flags)->pipe[(*flags)->pipe_index]->infile_nb == 1 
			|| (*flags)->pipe[(*flags)->pipe_index]->infile_nb <= 1 
		)
		) 
	{
		printf("1 - outfile_index++\n");
		(*flags)->pipe[(*flags)->pipe_index]->outfile_index++;
	}

	if ((*flags)->pipe[(*flags)->pipe_index]->infile_nb > 0 && (*flags)->pipe[(*flags)->pipe_index]->infile_index < (*flags)->pipe[(*flags)->pipe_index]->infile_nb
		&& (
			(*flags)->pipe[(*flags)->pipe_index]->outfile_index + 1 >= (*flags)->pipe[(*flags)->pipe_index]->outfile_nb
			|| (*flags)->pipe[(*flags)->pipe_index]->outfile_nb == 1
		)
		)
	{
		printf("1 - infile_index++\n");
		(*flags)->pipe[(*flags)->pipe_index]->infile_index++;
	}

	if ((*flags)->pipe[(*flags)->pipe_index]->outfile_index == -1 && (*flags)->pipe[(*flags)->pipe_index]->infile_index + 1 >= (*flags)->pipe[(*flags)->pipe_index]->infile_nb)
	{
		printf("2 - outfile_index++\n");
		(*flags)->pipe[(*flags)->pipe_index]->outfile_index++;
	}
		
	if ((*flags)->pipe[(*flags)->pipe_index]->infile_nb > 1 && (*flags)->pipe[(*flags)->pipe_index]->outfile_nb > 1)
	{
		if ((*flags)->pipe[(*flags)->pipe_index]->infile_index + 1 < (*flags)->pipe[(*flags)->pipe_index]->infile_nb)
		{
			printf("2 - infile_index++\n");
			(*flags)->pipe[(*flags)->pipe_index]->infile_index++;
		}
		if ((*flags)->pipe[(*flags)->pipe_index]->outfile_index + 1 < (*flags)->pipe[(*flags)->pipe_index]->outfile_nb
			&& (*flags)->pipe[(*flags)->pipe_index]->infile_index + 1 >= (*flags)->pipe[(*flags)->pipe_index]->infile_nb)
			(*flags)->pipe[(*flags)->pipe_index]->outfile_index++;

		if ((*flags)->pipe[(*flags)->pipe_index]->outfile_index + 1 >= (*flags)->pipe[(*flags)->pipe_index]->outfile_nb
			&& (*flags)->pipe[(*flags)->pipe_index]->infile_index >= (*flags)->pipe[(*flags)->pipe_index]->infile_nb)
			(*flags)->pipe[(*flags)->pipe_index]->infile_index--;
		//&& (*flags)->pipe[(*flags)->pipe_index]->outfile_index == -1)
	}

	//printf("[!] flags->pipe[pipe_index]->infile_index = %d\n", (*flags)->pipe[(*flags)->pipe_index]->infile_index);////
	//printf("[!] flags->pipe[pipe_index]->infile_nb = %d\n", (*flags)->pipe[(*flags)->pipe_index]->infile_nb);////

	/*if ((*flags)->pipe[(*flags)->pipe_index]->index >= (*flags)->pipe[(*flags)->pipe_index]->index_max)
	{
		//printf("[!] chnage pipe_index\n");////
		(*flags)->pipe_index += 1;
		(*flags)->pipe[(*flags)->pipe_index]->index = 0;
		(*flags)->pipe[(*flags)->pipe_index]->infile_index = 0;

		//(*flags)->pipe_index_changed = true;
	}*/
	
	printf("pipe_index: %d\n", (*flags)->pipe_index);////
	printf("infile_index: %d\n", (*flags)->pipe[(*flags)->pipe_index]->infile_index);////
	printf("outfile_index: %d\n", (*flags)->pipe[(*flags)->pipe_index]->outfile_index);////
	printf("END\n\n");////
	usleep(500000);////
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
	//printf("flags->total_redir: %d\n", flags->total_redir);////
	while (i < flags->total_redir)
	{
		if (edit_flags(&flags, i))
		{
			//printf("-flags->pipe_index = %d\n", flags->pipe_index);////
			usleep(100000);////
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
	//printf("pipe_index: %d\n", flags->pipe_index);////
	return (WEXITSTATUS(status));
}

// handle the exit command
	//if (ft_strcmp(temp->argv[0], "exit") == 0) // exit
		//	return (exit_prog(&splitted, env));