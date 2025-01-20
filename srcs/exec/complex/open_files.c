/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:01:34 by Sifex             #+#    #+#             */
/*   Updated: 2025/01/16 16:01:34 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_infile(t_flags *flags)
{
	flags->fd_in[0] = open(flags->infile, O_RDONLY);
	if (flags->fd_in[0] < 0)
		return (1);////real exit
	if (access(flags->infile, R_OK) < 0)
		return (1);
	return (0);
}

int open_outfile(t_flags *flags)
{
	flags->fd_out[flags->cmd_count - 1] = open(flags->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0667);//// 0667~
	if (flags->fd_out[flags->cmd_count - 1] < 0)
		return (1);////real exit
	return (0);
}
