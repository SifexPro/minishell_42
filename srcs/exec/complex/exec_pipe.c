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

static int open_infile_pipe(t_flags *flags, int pipe_index, int infile_index, int i)
{
	if (!flags->pipe[pipe_index]->infile[infile_index]->is_heredoc)
	{
		if (!open_infile(i, infile_index, flags))
			return (close_pipe(flags), exit(1), 0);
	}
	else
	{
		if (!open_heredoc(i, infile_index, flags))
			return (close_pipe(flags), exit(1), 0);
	}
	dup2(flags->fd_in[i], 0);
	return (1);
}

static int open_outfile_pipe(t_flags *flags, int pipe_index, int outfile_index, int i)
{
	if (!open_outfile(i, outfile_index, flags))
		return (close_pipe(flags), exit(1), 0);
	dup2(flags->fd_out[i], 1);
	return (1);
}

static int	open_all_files(t_flags *flags, int pipe_index, int i)
{
	int index_check;
	int	infile_index;
	int	outfile_index;

	index_check = 0;
	infile_index = 0;
	outfile_index = 0;
	while (index_check < (flags->pipe[flags->pipe_index]->infile_nb + flags->pipe[flags->pipe_index]->outfile_nb))
	{
		if (infile_index < flags->pipe[flags->pipe_index]->infile_nb && flags->pipe[flags->pipe_index]->infile[infile_index]->index == index_check)
		{
			if (!open_infile_pipe(flags, pipe_index, infile_index, i))
				return (0);
			infile_index++;
		}
		else if (outfile_index < flags->pipe[flags->pipe_index]->outfile_nb && flags->pipe[flags->pipe_index]->outfile[outfile_index]->index == index_check)
		{
			if (!open_outfile_pipe(flags, pipe_index, outfile_index, i))
				return (0);
			outfile_index++;
		}
		index_check++;
	}
	return (1);
}

void	child_exec(t_flags *flags, int i, t_ht *env, char **envp)
{
	char	**envp_cpy;
	int		infile_index;
	int		outfile_index;
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
		exit(select_exec_pipe(flags->pipe[pipe_index]->cmd->argc, flags->pipe[pipe_index]->cmd->argv, env, envp_cpy));
	clear_env(envp_cpy);
	exit(0);
}
