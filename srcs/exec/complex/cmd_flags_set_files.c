/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_flags2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:07:41 by Sifex             #+#    #+#             */
/*   Updated: 2025/02/04 20:07:41 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_files2(int infile_count, int outfile_count,
	t_flags **flags, t_list **splitted)
{
	(*flags)->pipe[(*flags)->pipe_index]->infile_nb = infile_count;
	(*flags)->pipe[(*flags)->pipe_index]->outfile_nb = outfile_count;
	(*flags)->pipe[(*flags)->pipe_index]->infile
		= (t_file **)malloc(sizeof(t_file *) * infile_count + 1);
	if (!(*flags)->pipe[(*flags)->pipe_index]->infile)
		return (0);
	(*flags)->pipe[(*flags)->pipe_index]->outfile
		= (t_file **)malloc(sizeof(t_file *) * outfile_count + 1);
	if (!(*flags)->pipe[(*flags)->pipe_index]->outfile)
		return (free((*flags)->pipe[(*flags)->pipe_index]->infile), 0);
	return (set_files_while(flags, splitted, 0, 0));
}

int	set_files(int infile_count, int outfile_count,
	t_flags **flags, t_list **splitted)
{
	t_list	*start;

	start = *splitted;
	while (*splitted)
	{
		if (((t_exec *)(*splitted)->content)->token_next == PIPE)
			break ;
		else if (((t_exec *)(*splitted)->content)->token_next == REDIRECT_INPUT
			|| ((t_exec *)(*splitted)->content)->token_next == HEREDOC)
			infile_count++;
		else if (((t_exec *)(*splitted)->content)->token_next == REDIRECT_OUTPUT
			|| ((t_exec *)(*splitted)->content)->token_next == APPEND)
			outfile_count++;
		else if ((int)((t_exec *)(*splitted)->content)->token_next == -1)
			(*flags)->pipe[(*flags)->pipe_index]->cmd
				= (t_exec *)(*splitted)->content;
		*splitted = (*splitted)->next;
	}
	*splitted = start;
	return (set_files2(infile_count, outfile_count, flags, splitted));
}
