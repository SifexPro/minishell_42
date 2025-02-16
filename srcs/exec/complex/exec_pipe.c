/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:53:15 by Sifex             #+#    #+#             */
/*   Updated: 2025/01/16 15:53:15 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_infile_pipe(t_flags *flags, int pipe_i, int infile_i, int i)
{
	if (!flags->pipe[pipe_i]->infile[infile_i]->is_heredoc)
	{
		if (!open_infile(i, infile_i, flags))
			return (close_pipe(flags), exit(1), 0);
	}
	else
	{
		if (!open_heredoc(i, infile_i, flags))
			return (close_pipe(flags), exit(1), 0);
	}
	dup2(flags->fd_in[i], 0);
	return (1);
}

static int	open_outfile_pipe(t_flags *flags, int outfile_i, int i)
{
	if (!open_outfile(i, outfile_i, flags))
		return (close_pipe(flags), exit(1), 0);
	dup2(flags->fd_out[i], 1);
	return (1);
}

bool	check_file_index(t_flags *flags, int index_check,
	int file_i, bool is_infile)
{
	int	file_nb;
	int	file_index;

	if (is_infile && file_i < flags->pipe[flags->pipe_index]->infile_nb)
	{	
		file_nb = flags->pipe[flags->pipe_index]->infile_nb;
		file_index = flags->pipe[flags->pipe_index]->infile[file_i]->index;
	}
	else if (!is_infile && file_i < flags->pipe[flags->pipe_index]->outfile_nb)
	{
		file_nb = flags->pipe[flags->pipe_index]->outfile_nb;
		file_index = flags->pipe[flags->pipe_index]->outfile[file_i]->index;
	}
	else
		return (false);
	if (file_i < file_nb && file_index == index_check)
		return (true);
	return (false);
}

static int	open_all_files(t_flags *flags, int pipe_index, int i)
{
	int	index_check;
	int	infile_i;
	int	outfile_i;

	index_check = 0;
	infile_i = 0;
	outfile_i = 0;
	while (index_check < (flags->pipe[pipe_index]->infile_nb
			+ flags->pipe[pipe_index]->outfile_nb))
	{
		if (check_file_index(flags, index_check, infile_i, true))
		{
			if (!open_infile_pipe(flags, pipe_index, infile_i, i))
				return (0);
			infile_i++;
		}
		else if (check_file_index(flags, index_check, outfile_i, false))
		{
			if (!open_outfile_pipe(flags, outfile_i, i))
				return (0);
			outfile_i++;
		}
		index_check++;
	}
	return (1);
}

void	child_exec(t_flags *flags, int i, t_ht *env)
{
	char	**envp_cpy;
	int		pipe_index;

	pipe_index = flags->pipe_index;
	if (!open_all_files(flags, pipe_index, i))
		return (close_pipe(flags), exit(1));
	if (pipe_index > 0)
		dup2(flags->fd_in[i], 0);
	if (pipe_index < flags->pipe_nb - 1)
		dup2(flags->fd_out[i], 1);
	close_pipe(flags);
	envp_cpy = ht_to_envp(env);
	if (flags->pipe[pipe_index]->cmd)
		exit(select_exec_pipe(flags->pipe[pipe_index]->cmd->argc,
				flags->pipe[pipe_index]->cmd->argv, env, envp_cpy));
	clear_env(envp_cpy);
	exit(0);
}
