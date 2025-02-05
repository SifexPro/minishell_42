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

static void	set_files_outfile(t_flags **flags, t_list **splitted, int i_outfile)
{
	(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile]
		= (t_file *)malloc(sizeof(t_file));
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
}

static void	set_files_infile(t_flags **flags, t_list **splitted, int i_infile)
{
	(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile]
		= (t_file *)malloc(sizeof(t_file));
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
}

static void	set_files2_while(t_flags **flags, t_list **splitted,
	int i_infile, int i_outfile)
{
	while (*splitted)
	{
		if (((t_exec *)(*splitted)->content)->token_next == PIPE)
		{
			*splitted = (*splitted)->next;
			break ;
		}
		else if (((t_exec *)(*splitted)->content)->token_next == REDIRECT_INPUT
			|| ((t_exec *)(*splitted)->content)->token_next == HEREDOC)
		{
			set_files_infile(flags, splitted, i_infile);
			i_infile++;
		}
		else if (((t_exec *)(*splitted)->content)->token_next == REDIRECT_OUTPUT
			|| ((t_exec *)(*splitted)->content)->token_next == APPEND)
		{
			set_files_outfile(flags, splitted, i_outfile);
			i_outfile++;
		}
		*splitted = (*splitted)->next;
	}
}

static void	set_files2(int infile_count, int outfile_count,
	t_flags **flags, t_list **splitted)
{
	(*flags)->pipe[(*flags)->pipe_index]->infile_nb = infile_count;
	(*flags)->pipe[(*flags)->pipe_index]->outfile_nb = outfile_count;
	(*flags)->pipe[(*flags)->pipe_index]->infile
		= (t_file **)malloc(sizeof(t_file *) * infile_count + 1);
	(*flags)->pipe[(*flags)->pipe_index]->outfile
		= (t_file **)malloc(sizeof(t_file *) * outfile_count + 1);
	set_files2_while(flags, splitted, 0, 0);
}

void	set_files(int infile_count, int outfile_count,
	t_flags **flags, t_list **splitted)
{
	int		i_infile;
	int		i_outfile;
	t_file	**file;
	t_list	*start;

	i_infile = 0;
	i_outfile = 0;
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
		else if (((t_exec *)(*splitted)->content)->token_next == -1)
			(*flags)->pipe[(*flags)->pipe_index]->cmd
				= (t_exec *)(*splitted)->content;
		*splitted = (*splitted)->next;
	}
	*splitted = start;
	set_files2(infile_count, outfile_count, flags, splitted);
}
