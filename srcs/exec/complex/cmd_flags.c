/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_flags.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:15:38 by Sifex             #+#    #+#             */
/*   Updated: 2025/01/16 15:15:38 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_flags	*init_flags(t_list *splitted)
{
	t_flags	*flags;

	flags = malloc(sizeof(t_flags));
	if (!flags)
		return (0);// put the real exit
	flags->total_redir = 0;
	flags->pid = NULL;
	flags->fd_in = NULL;
	flags->fd_out = NULL;
	flags->pipe_index = 0;
	flags->pipe_nb = 1;
	flags->pipe_index_changed = true;
	flags->pipe = NULL;
	flags->splitted = splitted;
	return (flags);
}

static void	set_files(int infile_count, int outfile_count, t_flags **flags, t_list **splitted)
{
	int		i_infile;
	int		i_outfile;
	t_file	**file;
	t_list	*start;

	i_infile = 0;
	i_outfile = 0;
	start = *splitted;
	ft_printf("\n[IN] set_files\n");////
	ft_printf("pipe_index: %d\n", (*flags)->pipe_index);////
	while (*splitted)
	{
		if (((t_exec *)(*splitted)->content)->token_next == PIPE)
			break;
		else if (((t_exec *)(*splitted)->content)->token_next == REDIRECT_INPUT
			|| ((t_exec *)(*splitted)->content)->token_next == HEREDOC)
			infile_count++;
		else if (((t_exec *)(*splitted)->content)->token_next == REDIRECT_OUTPUT
			|| ((t_exec *)(*splitted)->content)->token_next == APPEND)
			outfile_count++;
		else if (((t_exec *)(*splitted)->content)->token_next == -1)
			(*flags)->pipe[(*flags)->pipe_index]->cmd = (t_exec *)(*splitted)->content;
		*splitted = (*splitted)->next;
	}
	*splitted = start;

	ft_printf("infile_count: %d\n", infile_count);////
	ft_printf("outfile_count: %d\n", outfile_count);////

	ft_printf("\n[before] set\n");////
	ft_printf("infile_nb: %d\n", (*flags)->pipe[(*flags)->pipe_index]->infile_nb);////
	ft_printf("outfile_nb: %d\n", (*flags)->pipe[(*flags)->pipe_index]->outfile_nb);////
	ft_printf("infile_index: %d\n", (*flags)->pipe[(*flags)->pipe_index]->infile_index);////
	ft_printf("outfile_index: %d\n", (*flags)->pipe[(*flags)->pipe_index]->outfile_index);////

	(*flags)->pipe[(*flags)->pipe_index]->infile_nb = infile_count;
	(*flags)->pipe[(*flags)->pipe_index]->outfile_nb = outfile_count;
	if (infile_count > 0)
		(*flags)->pipe[(*flags)->pipe_index]->infile_index = 0;
	if (outfile_count > 0)
		(*flags)->pipe[(*flags)->pipe_index]->outfile_index = 0;

	ft_printf("\n[after] set\n");////
	ft_printf("infile_nb: %d\n", (*flags)->pipe[(*flags)->pipe_index]->infile_nb);////
	ft_printf("outfile_nb: %d\n", (*flags)->pipe[(*flags)->pipe_index]->outfile_nb);////
	ft_printf("infile_index: %d\n", (*flags)->pipe[(*flags)->pipe_index]->infile_index);////
	ft_printf("outfile_index: %d\n", (*flags)->pipe[(*flags)->pipe_index]->outfile_index);////
	
	(*flags)->pipe[(*flags)->pipe_index]->infile = (t_file **)malloc(sizeof(t_file *) * infile_count + 1);
	(*flags)->pipe[(*flags)->pipe_index]->outfile = (t_file **)malloc(sizeof(t_file *) * outfile_count + 1);

	while (*splitted)
	{
		printf("((t_exec *)(*splitted)->content)->token_next: %d\n", ((t_exec *)(*splitted)->content)->token_next);////
		if (((t_exec *)(*splitted)->content)->token_next == PIPE)
		{
			*splitted = (*splitted)->next;
			break;
		}
		else if (((t_exec *)(*splitted)->content)->token_next == REDIRECT_INPUT 
			|| ((t_exec *)(*splitted)->content)->token_next == HEREDOC)
		{
			(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile] = (t_file *)malloc(sizeof(t_file));
			(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile]->file = ((t_exec *)(*splitted)->content)->argv[0];
			(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile]->is_infile = true;
			(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile]->is_outfile = false;
			(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile]->is_heredoc = ((t_exec *)(*splitted)->content)->token_next == HEREDOC;
			(*flags)->pipe[(*flags)->pipe_index]->infile[i_infile]->is_append = false;
		
			i_infile++;
		}
		else if (((t_exec *)(*splitted)->content)->token_next == REDIRECT_OUTPUT 
			|| ((t_exec *)(*splitted)->content)->token_next == APPEND)
		{
			(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile] = (t_file *)malloc(sizeof(t_file));
			(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile]->file = ((t_exec *)(*splitted)->content)->argv[0];
			(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile]->is_infile = false;
			(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile]->is_outfile = true;
			(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile]->is_heredoc = false;
			(*flags)->pipe[(*flags)->pipe_index]->outfile[i_outfile]->is_append = ((t_exec *)(*splitted)->content)->token_next == APPEND;
		
			i_outfile++;
		}
		*splitted = (*splitted)->next;
	}
}

static void	set_pipes(t_flags **flags, t_list *splitted)
{
	int		i;
	int		j;

	i = 0;
	(*flags)->pipe = (t_pipe **)malloc(sizeof(t_pipe *) * (*flags)->pipe_nb + 1);
	while (i < (*flags)->pipe_nb)
	{
		(*flags)->pipe_index = i;
		(*flags)->pipe[i] = (t_pipe *)malloc(sizeof(t_pipe));
		(*flags)->pipe[i]->infile_index = -1;
		(*flags)->pipe[i]->infile_nb = 0;
		(*flags)->pipe[i]->infile = NULL;
		(*flags)->pipe[i]->outfile_index = -1;
		(*flags)->pipe[i]->outfile_nb = 0;
		(*flags)->pipe[i]->outfile = NULL;
		(*flags)->pipe[i]->cmd = NULL;

		set_files(0, 0, flags, &splitted);
		printf("[AFTER] set_files\n");////
		
		printf("\n[INFILE]\n");///
		printf("infile_index: %d\n", (*flags)->pipe[i]->infile_index);////
		printf("infile_nb: %d\n", (*flags)->pipe[i]->infile_nb);////
		printf("[IN] infile\n");////
		for (int f = 0; f < (*flags)->pipe[i]->infile_nb; f++)
		{
			printf("infile[%d]: %s\n", f, (*flags)->pipe[i]->infile[f]->file);////
			printf("is_infile: %d\n", (*flags)->pipe[i]->infile[f]->is_infile);////
			printf("is_outfile: %d\n", (*flags)->pipe[i]->infile[f]->is_outfile);////
			printf("is_heredoc: %d\n", (*flags)->pipe[i]->infile[f]->is_heredoc);////
			printf("is_append: %d\n", (*flags)->pipe[i]->infile[f]->is_append);////
		}
		printf("[OUT] infile\n");////

		printf("\n[OUTFILE]\n");////
		printf("outfile_index: %d\n", (*flags)->pipe[i]->outfile_index);////
		printf("outfile_nb: %d\n", (*flags)->pipe[i]->outfile_nb);////
		printf("[IN] outfile\n");////
		for (int f = 0; f < (*flags)->pipe[i]->outfile_nb; f++)
		{
			printf("outfile[%d]: %s\n", f, (*flags)->pipe[i]->outfile[f]->file);////
			printf("is_infile: %d\n", (*flags)->pipe[i]->outfile[f]->is_infile);////
			printf("is_outfile: %d\n", (*flags)->pipe[i]->outfile[f]->is_outfile);////
			printf("is_heredoc: %d\n", (*flags)->pipe[i]->outfile[f]->is_heredoc);////
			printf("is_append: %d\n", (*flags)->pipe[i]->outfile[f]->is_append);////
		}
		printf("[OUT] outfile\n");////

		if ((*flags)->pipe[i]->cmd)
			printf("\n[COMMAND] cmd: %s\n", (*flags)->pipe[i]->cmd->argv[0]);////
		else
			printf("\n[COMMAND] cmd: NULL\n");////
		
		//exit(0);////
		i++;
	}
	(*flags)->pipe[i] = NULL;

	(*flags)->pipe_index = 0;
}

t_flags	*set_flags(t_list *splitted)
{
	t_flags	*flags;
	t_exec	*temp;
	t_list	*start;
	int		i;

	i = 0;
	flags = init_flags(splitted);
	if (!flags)
		return (0);// put the real exit
	start = splitted;
	while (splitted)
	{
		temp = splitted->content;
		//printf("temp->argv[0]: %s\n", temp->argv[0]);////
		//printf("temp->argv[1]: %s\n", temp->argv[1]);////
		//printf("temp->token_next: %d\n", temp->token_next);////
		if (temp->token_next == PIPE || temp->token_next == REDIRECT_INPUT || temp->token_next == REDIRECT_OUTPUT || temp->token_next == HEREDOC || temp->token_next == APPEND)
			flags->total_redir++;
		if (temp->token_next == PIPE)
			flags->pipe_nb++;
		splitted = splitted->next;
		i++;
	}
	if (!flags->total_redir)
		return (flags);
	set_pipes(&flags, start);
	if (flags->total_redir > 0)
	{
		flags->pid = ft_calloc(flags->total_redir + 1, sizeof(pid_t));
		flags->fd_in = ft_calloc((flags->total_redir + 2), sizeof(int));
		flags->fd_in = ft_memset(flags->fd_in, -1, (sizeof(int) * flags->total_redir + 1));
		flags->fd_out = ft_calloc((flags->total_redir + 2), sizeof(int));
		flags->fd_out = ft_memset(flags->fd_out, -1, (sizeof(int) * flags->total_redir + 1));
	}

	////flags->cmd_count = flags->pipe_count - flags->has_infile - flags->has_outfile - flags->has_heredoc + 1;
	///flags->cmd = (t_exec **)malloc(sizeof(t_exec *) * (flags->cmd_count + 1));
	//check malloc
	return (flags);
	////return (set_flags_files(flags, start));
}

void	free_flags(t_flags *flags)
{
	/*free(flags->cmd);
	free(flags->fd_in);
	free(flags->fd_out);
	free(flags->pid);
	free(flags);*/
}

////Proteger les mallocs
////si malloc fail, free tout ce qui a été malloc avant