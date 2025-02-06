/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_flags_set_pipes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:33:51 by Sifex             #+#    #+#             */
/*   Updated: 2025/02/04 20:33:51 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_pipes_while(t_flags **flags, t_list *splitted, int i)
{
	while (i < (*flags)->pipe_nb)
	{
		(*flags)->pipe_index = i;
		(*flags)->pipe[i] = (t_pipe *)malloc(sizeof(t_pipe));
		if (!(*flags)->pipe[i])
			return (free_flags_pipe((*flags), i), -1);
		(*flags)->pipe[i]->index = -1;
		(*flags)->pipe[i]->index_max = 1;
		(*flags)->pipe[i]->infile_index = -1;
		(*flags)->pipe[i]->infile_max = -1;
		(*flags)->pipe[i]->infile_nb = 0;
		(*flags)->pipe[i]->infile = NULL;
		(*flags)->pipe[i]->outfile_index = -1;
		(*flags)->pipe[i]->outfile_max = -1;
		(*flags)->pipe[i]->outfile_nb = 0;
		(*flags)->pipe[i]->outfile = NULL;
		(*flags)->pipe[i]->cmd = NULL;
		if (!set_files(0, 0, flags, &splitted))
			return (free_flags_pipe((*flags), i), -1);
		if ((*flags)->pipe[i]->infile_nb > 0)
			(*flags)->pipe[i]->index_max += (*flags)->pipe[i]->infile_nb - 1;
		if ((*flags)->pipe[i]->outfile_nb > 0)
			(*flags)->pipe[i]->index_max += (*flags)->pipe[i]->outfile_nb - 1;
		i++;
	}
	return (i);
}

int	set_pipes(t_flags **flags, t_list *splitted)
{
	int		i;

	//printf("pipe_nb: %d\n", (*flags)->pipe_nb);////
	(*flags)->pipe
		= (t_pipe **)malloc(sizeof(t_pipe *) * ((*flags)->pipe_nb + 1));
	if (!(*flags)->pipe)
		return (0);
	i = set_pipes_while(flags, splitted, 0);
	//printf("pipe_nb: %d\n", (*flags)->pipe_nb);////
	//printf("i: %d\n", i);////
	if (i < 0)
		return (0);
	(*flags)->pipe[i] = NULL;
	(*flags)->pipe_index = 0;
	return (1);
}
