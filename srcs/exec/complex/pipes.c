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

int	open_pipe(t_flags *flags)
{
	int	i;
	int	pipe_fd[2];

	i = 0;
	if (!flags)
		return (0);
	while (i < flags->pipe_nb)
	{
		if (pipe(pipe_fd) < 0)
			return (0);
		flags->fd_in[i + 1] = pipe_fd[0];
		flags->fd_out[i] = pipe_fd[1];
		i++;
	}
	return (1);
}

void	close_pipe(t_flags *flags)
{
	int	i;

	i = 0;
	if (!flags)
		return ;
	while (i < flags->pipe_nb)
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
