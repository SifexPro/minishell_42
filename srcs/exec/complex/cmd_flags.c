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

static t_flags	*init_flags(void)
{
	t_flags	*flags;

	flags = malloc(sizeof(t_flags));
	if (!flags)
		return (0);// put the real exit
	flags->cmd_count = 0;
	flags->pipe_count = 0;
	flags->infile = NULL;
	flags->outfile = NULL;
	flags->heredoc = NULL;
	flags->has_infile = false;
	flags->has_outfile = false;
	flags->has_heredoc = false;
	flags->pid = NULL;
	flags->fd_in = NULL;
	flags->fd_out = NULL;
	return (flags);
}

static t_flags *set_flags_cmd(t_flags *flags, t_list *splitted)
{
	int		i;

	i = -1;
	//printf("\n\nset_flags_cmd\n");////
	while (splitted)
	{
		//printf("((t_exec *)splitted->content)->argv[0]: %s\n", ((t_exec *)splitted->content)->argv[0]);////
		//if ((flags->has_infile || flags->has_heredoc) && i == flags->cmd_count)
		//	break;
		/*if (((t_exec *)splitted->content)->token_next != REDIRECT_INPUT && ((t_exec *)splitted->content)->token_next != HEREDOC && ((t_exec *)splitted->content)->token_next != REDIRECT_OUTPUT && ((t_exec *)splitted->content)->token_next != APPEND)
		{	
			printf("i: %d\n", i);////
			printf("((t_exec *)splitted->content)->argv[0]: %s\n", ((t_exec *)splitted->content)->argv[0]);////
			flags->cmd[++i] = splitted->content;
		}*/
		flags->cmd[++i] = splitted->content;
		splitted = splitted->next;
	}
	flags->cmd[++i] = NULL;
	if (flags->pipe_count > 0)//// rework for files in and out
	{
		flags->pid = ft_calloc(flags->cmd_count + 1, sizeof(pid_t));////
		flags->fd_in = ft_calloc((flags->cmd_count + 2), sizeof(int));//// prévoir un fd_in[0] pour le cas où il y a un fichier en entrée
		flags->fd_in = ft_memset(flags->fd_in, -1, (sizeof(int) * flags->cmd_count + 1));
		flags->fd_out = ft_calloc((flags->cmd_count + 2), sizeof(int));////
		flags->fd_out = ft_memset(flags->fd_out, -1, (sizeof(int) * flags->cmd_count + 1));
	}
	return (flags);
}

static t_flags *set_flags_files(t_flags *flags, t_list *splitted)
{
	t_list	*start;
	t_list	*temp;

	start = splitted;
	//printf("\n\nset_flags_files\n");////
	if (flags->has_infile)
	{
		flags->infile = ((t_exec *)splitted->content)->argv[0];
		//printf("flags->infile: %s\n", flags->infile);////
		start = start->next;
	}
	else if (flags->has_heredoc)
	{
		flags->heredoc = ((t_exec *)splitted->content)->argv[0];
		//printf("flags->heredoc: %s\n", flags->heredoc);////
		start = start->next;
	}
	//printf("((t_exec *)start->content)->argv[0]: %s\n", ((t_exec *)start->content)->argv[0]);////
	if (flags->has_outfile)
	{
		while (splitted->next)
			splitted = splitted->next;
		//printf("((t_exec *)splitted->content)->argv[0]: %s\n", ((t_exec *)splitted->content)->argv[0]);////
		flags->outfile = ((t_exec *)splitted->content)->argv[0];
		//printf("flags->outfile: %s\n", flags->outfile);////
	}
	return (set_flags_cmd(flags, start));
}

t_flags	*set_flags(t_list *splitted)
{
	t_flags	*flags;
	t_exec	*temp;
	t_list	*start;
	int		i;

	i = 0;
	flags = init_flags();
	start = splitted;
	while (splitted)
	{
		temp = splitted->content;
		printf("temp->argv[0]: %s\n", temp->argv[0]);////
		printf("temp->argv[1]: %s\n", temp->argv[1]);////
		printf("temp->token_next: %d\n", temp->token_next);////
		if (temp->token_next == PIPE || temp->token_next == REDIRECT_INPUT || temp->token_next == REDIRECT_OUTPUT || temp->token_next == HEREDOC || temp->token_next == APPEND)
			flags->pipe_count++;
		if (temp->token_next == REDIRECT_OUTPUT || temp->token_next == APPEND)
			flags->has_outfile = true;
		if (temp->token_next == REDIRECT_INPUT && i == 0)
			flags->has_infile = true;
		else if (temp->token_next == HEREDOC) 
			flags->has_heredoc = true;
		else if (temp->token_next == APPEND)
			flags->has_append = true;
		splitted = splitted->next;
		i++;
	}
	flags->cmd_count = flags->pipe_count - flags->has_infile - flags->has_outfile - flags->has_heredoc + 1;
	flags->cmd = (t_exec **)malloc(sizeof(t_exec *) * (flags->cmd_count + 1));
	//check malloc
	return (set_flags_files(flags, start));
}

void	free_flags(t_flags *flags)
{
	free(flags->cmd);
	free(flags->fd_in);
	free(flags->fd_out);
	free(flags->pid);
	free(flags);
}
