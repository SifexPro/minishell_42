/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_flags_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 22:07:15 by Sifex             #+#    #+#             */
/*   Updated: 2025/02/05 22:07:15 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_flags_pipe(t_flags *flags, int pipe_i)
{
	int	i;
	int	j;

	i = -1;
	while (++i < pipe_i)
	{
		j = -1;
		while (++j < flags->pipe[i]->infile_nb)
			free(flags->pipe[i]->infile[j]);
		free(flags->pipe[i]->infile);
		j = -1;
		while (++j < flags->pipe[i]->outfile_nb)
			free(flags->pipe[i]->outfile[j]);
		free(flags->pipe[i]->outfile);
		free(flags->pipe[i]);
	}
}

void	free_flags_files(t_flags *flags, int i_infile, int i_outfile)
{
	int	i;

	i = 0;
	while (i < i_infile)
	{
		free(flags->pipe[flags->pipe_index]->infile[i]);
		i++;
	}
	free(flags->pipe[flags->pipe_index]->infile);
	i = 0;
	while (i < i_outfile)
	{
		free(flags->pipe[flags->pipe_index]->outfile[i]);
		i++;
	}
	free(flags->pipe[flags->pipe_index]->outfile);
}

void	free_flags(t_flags *flags)
{
	int	i;

	free_flags_pipe(flags, flags->pipe_nb);
	free(flags->pipe);
	free(flags->pid);
	free(flags->fd_in);
	free(flags->fd_out);
	free(flags);
}
