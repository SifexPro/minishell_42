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
		return (0);//// put the real exit
	flags->multi_exec = false;
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

static t_flags	*set_flags3(t_flags *flags)
{
	if (flags->total_redir > 0)
	{
		flags->pid = ft_calloc(flags->total_redir + 1, sizeof(pid_t));
		if (!flags->pid)
			return (free_flags_pipe(flags, flags->pipe_nb), NULL);
		flags->fd_in = ft_calloc((flags->total_redir + 2), sizeof(int));
		if (!flags->fd_in)
			return (free_flags_pipe(flags, flags->pipe_nb), free(flags->pid), NULL);
		flags->fd_in = ft_memset(flags->fd_in, -1,
				(sizeof(int) * flags->total_redir + 1));
		flags->fd_out = ft_calloc((flags->total_redir + 2), sizeof(int));
		if (!flags->fd_out)
			return (free_flags_pipe(flags, flags->pipe_nb), free(flags->pid), free(flags->fd_in), NULL);
		flags->fd_out = ft_memset(flags->fd_out, -1,
				(sizeof(int) * flags->total_redir + 1));
	}
	return (flags);
}

static t_flags	*set_flags2(t_flags *flags)
{
	int	i;

	i = 0;
	flags->total_redir += flags->pipe_nb;
	if (flags->total_redir > 1)
		flags->multi_exec = true;
	while (i < flags->pipe_nb)
	{
		if (flags->pipe[i]->infile_nb > 0)
		{
			flags->multi_exec = true;
			flags->total_redir += flags->pipe[i]->infile_nb - 1;
		}
		if (flags->pipe[i]->outfile_nb > 0)
		{
			flags->multi_exec = true;
			flags->total_redir += flags->pipe[i]->outfile_nb - 1;
		}
		i++;
	}
	return (flags);
}

t_flags	*set_flags(t_list *splitted)
{
	t_flags	*flags;
	t_exec	*temp;
	t_list	*start;

	flags = init_flags(splitted);
	if (!flags)
		return (NULL);
	start = splitted;
	while (splitted)
	{
		temp = splitted->content;
		if (temp->token_next == PIPE)
			flags->pipe_nb++;
		splitted = splitted->next;
	}
	if (!set_pipes(&flags, start))
		return (free(flags), NULL);
	flags = set_flags2(flags);
	if (!flags->total_redir)
		return (flags);
	return (set_flags3(flags));
}
////Proteger les mallocs
////si malloc fail, free tout ce qui a été malloc avant