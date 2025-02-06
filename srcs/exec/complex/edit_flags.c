/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_flags.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:18:30 by Sifex             #+#    #+#             */
/*   Updated: 2025/02/04 19:18:30 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	edit_flags4(t_flags **flags)
{
	if ((*flags)->pipe[(*flags)->pipe_index]->index == 0
		&& (*flags)->pipe[(*flags)->pipe_index]->infile_max != -1
		&& (*flags)->pipe[(*flags)->pipe_index]->infile_index == -1)
		(*flags)->pipe[(*flags)->pipe_index]->infile_index = 0;
	if ((*flags)->pipe[(*flags)->pipe_index]->infile_max != -1
		&& (*flags)->pipe[(*flags)->pipe_index]->infile_stop == -1
		&& (*flags)->pipe[(*flags)->pipe_index]->infile_index
		>= (*flags)->pipe[(*flags)->pipe_index]->infile_max - 1)
	{
		(*flags)->pipe[(*flags)->pipe_index]->infile_stop
			= (*flags)->pipe[(*flags)->pipe_index]->index;
	}
	/*if ((*flags)->pipe[(*flags)->pipe_index]->infile_stop != -1
		&& (*flags)->pipe[(*flags)->pipe_index]->index
		>= (*flags)->pipe[(*flags)->pipe_index]->infile_stop)
	{
		(*flags)->pipe[(*flags)->pipe_index]->infile_index = (*flags)->pipe[(*flags)->pipe_index]->infile_max - 1;
	}*/
}

static void	edit_flags3(t_flags **flags)
{
	if ((*flags)->pipe[(*flags)->pipe_index]->infile_index + 1
		< (*flags)->pipe[(*flags)->pipe_index]->infile_nb)
		(*flags)->pipe[(*flags)->pipe_index]->infile_index++;
	if ((*flags)->pipe[(*flags)->pipe_index]->outfile_index + 1
		< (*flags)->pipe[(*flags)->pipe_index]->outfile_nb
		&& (*flags)->pipe[(*flags)->pipe_index]->infile_index + 1
		>= (*flags)->pipe[(*flags)->pipe_index]->infile_nb
		&& (*flags)->pipe[(*flags)->pipe_index]->outfile_index == -1)
		(*flags)->pipe[(*flags)->pipe_index]->outfile_index++;
	if ((*flags)->pipe[(*flags)->pipe_index]->outfile_index + 1
		>= (*flags)->pipe[(*flags)->pipe_index]->outfile_nb
		&& (*flags)->pipe[(*flags)->pipe_index]->infile_index
		>= (*flags)->pipe[(*flags)->pipe_index]->infile_nb)
		(*flags)->pipe[(*flags)->pipe_index]->infile_index--;
}

static void	edit_flags2(t_flags **flags)
{
	if ((*flags)->pipe[(*flags)->pipe_index]->infile_nb > 0
		&& (*flags)->pipe[(*flags)->pipe_index]->infile_index
		< (*flags)->pipe[(*flags)->pipe_index]->infile_nb
		&& ((*flags)->pipe[(*flags)->pipe_index]->outfile_index + 1
			>= (*flags)->pipe[(*flags)->pipe_index]->outfile_nb
			|| (*flags)->pipe[(*flags)->pipe_index]->outfile_nb == 1))
		(*flags)->pipe[(*flags)->pipe_index]->infile_index++;
	if ((*flags)->pipe[(*flags)->pipe_index]->outfile_index == -1
		&& (*flags)->pipe[(*flags)->pipe_index]->infile_index + 1
		>= (*flags)->pipe[(*flags)->pipe_index]->infile_nb)
		(*flags)->pipe[(*flags)->pipe_index]->outfile_index++;
	if ((*flags)->pipe[(*flags)->pipe_index]->infile_nb > 1
		&& (*flags)->pipe[(*flags)->pipe_index]->outfile_nb > 1)
		edit_flags3(flags);
	edit_flags4(flags);
}

bool	edit_flags(t_flags **flags)
{
	(*flags)->pipe[(*flags)->pipe_index]->index++;
	if ((*flags)->pipe[(*flags)->pipe_index]->index
		>= (*flags)->pipe[(*flags)->pipe_index]->index_max)
	{
		(*flags)->pipe_index += 1;
		(*flags)->pipe[(*flags)->pipe_index]->index = 0;
	}
	if ((*flags)->pipe[(*flags)->pipe_index]->outfile_nb > 0
		&& (*flags)->pipe[(*flags)->pipe_index]->outfile_index
		< (*flags)->pipe[(*flags)->pipe_index]->outfile_nb
		&& (
			(*flags)->pipe[(*flags)->pipe_index]->infile_index + 1
			>= (*flags)->pipe[(*flags)->pipe_index]->infile_nb
			|| (*flags)->pipe[(*flags)->pipe_index]->infile_nb <= 1))
	{
		if ((*flags)->pipe[(*flags)->pipe_index]->infile_max == -1)
			(*flags)->pipe[(*flags)->pipe_index]->outfile_index++;
		else if ((*flags)->pipe[(*flags)->pipe_index]->infile_max == 1)//// belek
			(*flags)->pipe[(*flags)->pipe_index]->infile_index = -1;
		else
			(*flags)->pipe[(*flags)->pipe_index]->infile_index++;
	}
	edit_flags2(flags);
	return (true);
}
