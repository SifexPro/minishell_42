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
	int	initial_infile_index;
	//printf("START\n");////
	initial_infile_index = (*flags)->pipe[(*flags)->pipe_index]->infile_index;
	(*flags)->pipe[(*flags)->pipe_index]->index++;
	//printf("\n[!] flags->pipe[pipe_index]->index = %d\n", (*flags)->pipe[(*flags)->pipe_index]->index);////
	//printf("[!] flags->pipe[pipe_index]->index_max = %d\n", (*flags)->pipe[(*flags)->pipe_index]->index_max);////

	if ((*flags)->pipe[(*flags)->pipe_index]->index >= (*flags)->pipe[(*flags)->pipe_index]->index_max)
	{
		//printf("ADDED PIPE INDEX\n");////
		//printf("[!] chnage pipe_index\n");////
		(*flags)->pipe_index += 1;
		(*flags)->pipe[(*flags)->pipe_index]->index = 0;
		//(*flags)->pipe[(*flags)->pipe_index]->infile_index = 0;
		//return (true);
		//(*flags)->pipe_index_changed = true;
	}

	//ls | < Makefile wc -l > test
	//printf("pipe_index: %d\n", (*flags)->pipe_index);////
	//printf("infile_index: %d\n", (*flags)->pipe[(*flags)->pipe_index]->infile_index);////
	//printf("outfile_index: %d\n", (*flags)->pipe[(*flags)->pipe_index]->outfile_index);////

	if ((*flags)->pipe[(*flags)->pipe_index]->outfile_nb > 0 && (*flags)->pipe[(*flags)->pipe_index]->outfile_index < (*flags)->pipe[(*flags)->pipe_index]->outfile_nb
		&& (
			(*flags)->pipe[(*flags)->pipe_index]->infile_index + 1 >= (*flags)->pipe[(*flags)->pipe_index]->infile_nb 
			//|| (*flags)->pipe[(*flags)->pipe_index]->infile_nb == 1 
			|| (*flags)->pipe[(*flags)->pipe_index]->infile_nb <= 1 
		)
		) 
	{
		if ((*flags)->pipe[(*flags)->pipe_index]->infile_max == -1)
		{
			//printf("1 - outfile_index++\n");////
			(*flags)->pipe[(*flags)->pipe_index]->outfile_index++;
		}
		else
		{
			//printf("1 - infile_index++\n");////
			(*flags)->pipe[(*flags)->pipe_index]->infile_index++;
		}
	}

	if ((*flags)->pipe[(*flags)->pipe_index]->infile_nb > 0 && (*flags)->pipe[(*flags)->pipe_index]->infile_index < (*flags)->pipe[(*flags)->pipe_index]->infile_nb
		&& (
			(*flags)->pipe[(*flags)->pipe_index]->outfile_index + 1 >= (*flags)->pipe[(*flags)->pipe_index]->outfile_nb
			|| (*flags)->pipe[(*flags)->pipe_index]->outfile_nb == 1
		)
		)
	{
		//printf("1 - infile_index++\n");////
		(*flags)->pipe[(*flags)->pipe_index]->infile_index++;
	}

	if ((*flags)->pipe[(*flags)->pipe_index]->outfile_index == -1 && (*flags)->pipe[(*flags)->pipe_index]->infile_index + 1 >= (*flags)->pipe[(*flags)->pipe_index]->infile_nb)
	{
		//printf("2 - outfile_index++\n");////
		(*flags)->pipe[(*flags)->pipe_index]->outfile_index++;
	}
		
	if ((*flags)->pipe[(*flags)->pipe_index]->infile_nb > 1 && (*flags)->pipe[(*flags)->pipe_index]->outfile_nb > 1)
	{
		if ((*flags)->pipe[(*flags)->pipe_index]->infile_index + 1 < (*flags)->pipe[(*flags)->pipe_index]->infile_nb)
		{
			//printf("2 - infile_index++\n");////
			(*flags)->pipe[(*flags)->pipe_index]->infile_index++;
		}
		if ((*flags)->pipe[(*flags)->pipe_index]->outfile_index + 1 < (*flags)->pipe[(*flags)->pipe_index]->outfile_nb
			&& (*flags)->pipe[(*flags)->pipe_index]->infile_index + 1 >= (*flags)->pipe[(*flags)->pipe_index]->infile_nb
			&& (*flags)->pipe[(*flags)->pipe_index]->outfile_index == -1)
		{
			//printf("3 - outfile_index++\n");////
			(*flags)->pipe[(*flags)->pipe_index]->outfile_index++;
		}

		if ((*flags)->pipe[(*flags)->pipe_index]->outfile_index + 1 >= (*flags)->pipe[(*flags)->pipe_index]->outfile_nb
			&& (*flags)->pipe[(*flags)->pipe_index]->infile_index >= (*flags)->pipe[(*flags)->pipe_index]->infile_nb)
		{
			//printf("4 - infile_index--\n");////
			(*flags)->pipe[(*flags)->pipe_index]->infile_index--;
		}
		/*if ((*flags)->pipe[(*flags)->pipe_index]->infile_max != -1 && (*flags)->pipe[(*flags)->pipe_index]->infile_index - 1 != -1)
		{
			//initial_infile_index
			printf("5 - infile_index = initial_infile_index\n");////
			(*flags)->pipe[(*flags)->pipe_index]->infile_index = initial_infile_index;
		}*/
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
	
	/*printf("pipe_index: %d\n", (*flags)->pipe_index);////
	printf("infile_index: %d\n", (*flags)->pipe[(*flags)->pipe_index]->infile_index);////
	printf("outfile_index: %d\n", (*flags)->pipe[(*flags)->pipe_index]->outfile_index);////
	printf("END\n\n");////*/
	//usleep(5000000);////
	return (true);
}

static void check_exec(t_flags **flags)
{
	int			i;
	int			j;
	int			fd;
	struct stat	fileStat;

	i = 0;
	//ft_printf("check_exec\n");
	while (i < (*flags)->pipe_nb)
	{
		j = 0;
		//ft_printf("in pipe[%d]\n", i);
		while (j < (*flags)->pipe[i]->infile_nb)
		{
			//ft_printf("infile[%d]: %s\n", j, (*flags)->pipe[i]->infile[j]->file);
			if (access((*flags)->pipe[i]->infile[j]->file, F_OK | R_OK) != 0)
			{
				if ((*flags)->pipe[i]->outfile_nb > 1 && (*flags)->pipe[i]->infile_nb > 1)
					(*flags)->pipe[i]->infile_max = j + 1;
				else
					(*flags)->pipe[i]->infile_max = j + 1;
				break ;
			}
			j++;
		}
		j = 0;
		//printf("outfile_nb: %d\n", (*flags)->pipe[i]->outfile_nb);
		while (j < (*flags)->pipe[i]->outfile_nb)
		{
			//ft_printf("outfile[%d]\n", j);
			fd = open((*flags)->pipe[i]->outfile[j]->file, O_RDONLY);
			if (fd == -1)
			{
				perror("open");
				return ;
			}
			if (fstat(fd, &fileStat) == -1)
			{
				perror("fstat");
				close(fd);
       	 		return ;
			}
			close(fd);
			if (!(fileStat.st_mode & S_IWOTH))
			{
				(*flags)->pipe[i]->outfile_max = j + 1;
				break ;
			}
			j++;
		}
		i++;
	}
}

int	forking(t_flags *flags, t_ht *env, char **envp)
{
	int	i;
	int	status;

	i = 0;
	//if (flags->has_heredoc)////in child
	//	open_heredoc(flags);
	open_pipe(flags);
	//ft_printf("started\n");
	check_exec(&flags);
	//ft_printf("finished\n");
	//printf("flags->total_redir: %d\n", flags->total_redir);////
	while (i < flags->total_redir)
	{
		if (edit_flags(&flags, i))
		{
			//printf("-flags->pipe_index = %d\n", flags->pipe_index);////
			//usleep(100000);////
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