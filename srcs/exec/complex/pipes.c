/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:35:22 by Sifex             #+#    #+#             */
/*   Updated: 2025/01/16 15:35:22 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_pipe(t_flags *flags)
{
	int	i;
	int	pipe_fd[2];

	i = 0;
	if (!flags)
		return ;
	while (i < flags->total_redir)
	{
		if (pipe(pipe_fd) < 0)
			exit(1);////real exit
		flags->fd_in[i + 1] = pipe_fd[0];
		flags->fd_out[i] = pipe_fd[1];
		i++;
	}
}

void	close_pipe(t_flags *flags)
{
	int	i;

	i = 0;
	if (!flags)
		return ;
	while (i < flags->total_redir)
	{
		if (flags->fd_in && flags->fd_in[i] != -1)
		{
			close(flags->fd_in[i]);
			flags->fd_in[i] = -1;
		}
		if (flags->fd_out && flags->fd_out[i] != -1)
		{
			close(flags->fd_out[i]);
			flags->fd_out[i] = -1;
		}
		i++;
	}
}
