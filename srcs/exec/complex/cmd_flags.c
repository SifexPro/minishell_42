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
	flags->has_infile = false;
	flags->has_outfile = false;
	flags->pid = NULL;
	flags->fd_in = NULL;
	flags->fd_out = NULL;
	return (flags);
}

static t_flags *set_flags_cmd(t_flags *flags, t_list *splitted)
{
	int		i;

	i = 0;
	while (splitted)
	{
		if (i == 0 && flags->has_infile)
			flags->infile = ((t_exec *)splitted->content)->argv[0];
		else if (i == flags->cmd_count - 1 && flags->has_outfile)
			flags->outfile = ((t_exec *)splitted->content)->argv[0];
		flags->cmd[i] = splitted->content;
		splitted = splitted->next;
		i++;
	}
	flags->cmd[i] = NULL;
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

t_flags	*set_flags(t_list *splitted)
{
	t_flags	*flags;
	t_exec	*temp;
	t_list	*start;

	flags = init_flags();
	start = splitted;
	while (splitted)
	{
		temp = splitted->content;
		if (temp->token_next == PIPE || temp->token_next == REDIRECT_INPUT || temp->token_next == REDIRECT_OUTPUT)
			flags->pipe_count++;
		if (temp->token_next == REDIRECT_INPUT)
			flags->has_infile = true;
		else if (temp->token_next == REDIRECT_OUTPUT)
			flags->has_outfile = true;
		splitted = splitted->next;
	}
	flags->cmd_count = flags->pipe_count + 1;
	flags->cmd = (t_exec **)malloc(sizeof(t_exec *) * (flags->cmd_count + 1));
	//check malloc
	return (set_flags_cmd(flags, start));
}
