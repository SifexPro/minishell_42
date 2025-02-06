/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:39:01 by Sifex             #+#    #+#             */
/*   Updated: 2025/02/04 19:39:01 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_infile(t_flags **flags, int pipe_i)
{
	int			j;

	j = 0;
	while (j < (*flags)->pipe[pipe_i]->infile_nb)
	{
		if (access((*flags)->pipe[pipe_i]->infile[j]->file, F_OK | R_OK) != 0)
		{
			if ((*flags)->pipe[pipe_i]->outfile_nb > 1
				&& (*flags)->pipe[pipe_i]->infile_nb > 1)
				(*flags)->pipe[pipe_i]->infile_max = j + 1;
			else
				(*flags)->pipe[pipe_i]->infile_max = j + 1;
			break ;
		}
		j++;
	}
}

static int	open_outfile_fd(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	return (fd);
}

static void	check_outfile(t_flags **flags, int pipe_i)
{
	int			j;
	int			fd;
	struct stat	file_stat;

	j = 0;
	while (j < (*flags)->pipe[pipe_i]->outfile_nb)
	{
		if (access((*flags)->pipe[pipe_i]->outfile[j]->file, F_OK) == 0)
		{
			fd = open_outfile_fd((*flags)->pipe[pipe_i]->outfile[j]->file);
			if (fstat(fd, &file_stat) == -1)
			{
				perror("fstat");
				close(fd);
				return ;
			}
			close(fd);
			if (!(file_stat.st_mode & S_IWOTH))
			{
				(*flags)->pipe[pipe_i]->outfile_max = j + 1;
				break ;
			}
		}
		j++;
	}
}

void	check_exec(t_flags **flags)
{
	int		i;

	i = 0;
	while (i < (*flags)->pipe_nb)
	{
		check_infile(flags, i);
		check_outfile(flags, i);
		i++;
	}
}
////gerer exit