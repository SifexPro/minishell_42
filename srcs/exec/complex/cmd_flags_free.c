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

	i = -1;
	while (++i < pipe_i)
	{
		free(flags->pipe[i]->infile);
		free(flags->pipe[i]->outfile);
		free(flags->pipe[i]);
	}
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
