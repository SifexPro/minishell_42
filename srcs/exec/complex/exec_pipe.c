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

static void	open_some(t_flags *flags, int pipe_i, int infile_i, int outfile_i)
{
	if (infile_i > flags->pipe[pipe_i]->infile_max
		|| outfile_i >= flags->pipe[pipe_i]->outfile_nb || outfile_i == -1)
		return (close_pipe(flags), exit(1));
	while (flags->pipe[pipe_i]->infile[infile_i - 1]->index
		> flags->pipe[pipe_i]->outfile[outfile_i]->index
		&& flags->pipe[pipe_i]->outfile_index
		< flags->pipe[pipe_i]->outfile_nb)
	{
		flags->pipe[pipe_i]->outfile_index = outfile_i;
		if (!open_outfile(0, flags))
			break ;
		outfile_i++;
	}
	return (close_pipe(flags), exit(1));
}

static void	check_files(t_flags *flags, int infile_i, int outfile_i)
{
	int	pipe_index;

	pipe_index = flags->pipe_index;
	if (flags->pipe[pipe_index]->infile_max != -1
		&& infile_i >= flags->pipe[pipe_index]->infile_max)
		open_some(flags, pipe_index, infile_i, outfile_i);
	if (flags->pipe[pipe_index]->infile_max != -1
		&& flags->pipe[pipe_index]->infile_nb == 1
		&& flags->pipe[pipe_index]->outfile_nb == 1
		&& infile_i == 0 && outfile_i == 0
		&& flags->pipe[pipe_index]->infile[infile_i]->index
		> flags->pipe[pipe_index]->outfile[outfile_i]->index)
	{
		open_outfile(0, flags);
		return (close_pipe(flags), exit(1));
	}
	else if (flags->pipe[pipe_index]->outfile_max != -1
		&& outfile_i >= flags->pipe[pipe_index]->outfile_max)
		return (close_pipe(flags), exit(1));
	else if (flags->pipe[pipe_index]->infile_stop != -1
		&& flags->pipe[pipe_index]->index
		> flags->pipe[pipe_index]->infile_stop)
		return (close_pipe(flags), exit(1));
}

static void	handle_files(t_flags *flags, int infile_i, int outfile_i, int i)
{
	int	pipe_index;

	pipe_index = flags->pipe_index;
	check_files(flags, infile_i, outfile_i);
	if (infile_i != -1 && infile_i < flags->pipe[pipe_index]->infile_nb)
	{
		if (!flags->pipe[pipe_index]->infile[infile_i]->is_heredoc)
		{
			if (!open_infile(i, flags))
				return (close_pipe(flags), exit(1));
		}
		else
		{
			if (!open_heredoc(i, flags))
				return (close_pipe(flags), exit(1));
		}
		dup2(flags->fd_in[i], 0);
	}
	if (outfile_i != -1 && outfile_i < flags->pipe[pipe_index]->outfile_nb)
	{
		if (!open_outfile(i, flags))
			return (close_pipe(flags), exit(1));
		dup2(flags->fd_out[i], 1);
	}
}

void	child_exec(t_flags *flags, int i, t_ht *env, char **envp)
{
	char	**envp_cpy;
	int		infile_index;
	int		outfile_index;
	int		pipe_index;

	pipe_index = flags->pipe_index;
	infile_index = flags->pipe[pipe_index]->infile_index;
	outfile_index = flags->pipe[pipe_index]->outfile_index;
	handle_files(flags, infile_index, outfile_index, i);
	if (pipe_index > 0)
		dup2(flags->fd_in[i], 0);
	if (pipe_index < flags->pipe_nb - 1)
		dup2(flags->fd_out[i], 1);
	close_pipe(flags);
	envp_cpy = ht_to_envp(env);
	if (infile_index >= flags->pipe[pipe_index]->infile_nb - 1)
		if (outfile_index >= flags->pipe[pipe_index]->outfile_nb - 1
			&& flags->pipe[pipe_index]->cmd)
			exit(select_exec_pipe(flags->pipe[pipe_index]->cmd->argc,
					flags->pipe[pipe_index]->cmd->argv, env, envp_cpy));
	clear_env(envp_cpy);
	exit(0);
}
