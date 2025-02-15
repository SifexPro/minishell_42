/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_flags_set_files_while.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:07:41 by Sifex             #+#    #+#             */
/*   Updated: 2025/02/04 20:07:41 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_files_outfile(t_flags **flags, t_list **splitted, int i_outfile,
	int index)
{
	(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile]
		= (t_file *)malloc(sizeof(t_file));
	if (!(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile])
		return (0);
	(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile]->file
		= ((t_exec *)(*splitted)->content)->argv[0];
	(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile]->is_infile
		= false;
	(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile]->is_outfile
		= true;
	(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile]->is_heredoc
		= false;
	(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile]->is_append
		= ((t_exec *)(*splitted)->content)->token_next == APPEND;
	(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile]->index
		= index;
	return (1);
}

static int	set_files_infile(t_flags **flags, t_list **splitted, int i_infile,
	int index)
{
	(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile]
		= (t_file *)malloc(sizeof(t_file));
	if (!(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile])
		return (0);
	(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile]->file
		= ((t_exec *)(*splitted)->content)->argv[0];
	(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile]->is_infile
		= true;
	(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile]->is_outfile
		= false;
	(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile]->is_heredoc
		= ((t_exec *)(*splitted)->content)->token_next == HEREDOC;
	(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile]->is_append
		= false;
	(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile]->index
		= index;
	return (1);
}

static int	set_files_while2(t_flags **flags, t_list **splitted,
	int i_infile, int i_outfile)
{
	if (((t_exec *)(*splitted)->content)->token_next == PIPE)
	{
		*splitted = (*splitted)->next;
		return (3);
	}
	else if (((t_exec *)(*splitted)->content)->token_next == REDIRECT_INPUT
		|| ((t_exec *)(*splitted)->content)->token_next == HEREDOC)
		return (1);
	else if (((t_exec *)(*splitted)->content)->token_next == REDIRECT_OUTPUT
		|| ((t_exec *)(*splitted)->content)->token_next == APPEND)
		return (2);
	return (0);
}

static void	handle_res(int res, int *i, int *i_infile, int *i_outfile)
{
	if (res == 1)
		(*i_infile)++;
	else if (res == 2)
		(*i_outfile)++;
	if (res == 1 || res == 2)
		(*i)++;
}

int	set_files_while(t_flags **flags, t_list **splitted,
	int i_infile, int i_outfile)
{
	int	i;
	int	res;

	i = 0;
	while (*splitted)
	{
		res = set_files_while2(flags, splitted, i_infile, i_outfile);
		if (res == 3)
			break ;
		else if (res == 1)
		{
			if (!set_files_infile(flags, splitted, i_infile, i))
				return (0);
		}
		else if (res == 2)
		{
			if (!set_files_outfile(flags, splitted, i_outfile, i))
				return (0);
		}
		handle_res(res, &i, &i_infile, &i_outfile);
		*splitted = (*splitted)->next;
	}
	return (1);
}
