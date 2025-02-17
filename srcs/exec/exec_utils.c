/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:52:15 by Sifex             #+#    #+#             */
/*   Updated: 2025/02/06 18:23:19 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	has_slash(char *str)
{
	while (*str)
	{
		if (*str == '/')
			return (true);
		str++;
	}
	return (false);
}

int	check_file(char *cmd_path, char *file)
{
	struct stat	file_stat;
	int			fd;

	if (!((cmd_path && has_slash(cmd_path)) || (!cmd_path && has_slash(file)))
		|| (cmd_path && !ft_strncmp(cmd_path, "/usr/bin/", 9)))
	{
		if ((cmd_path && !has_slash(cmd_path))
			|| (!cmd_path && !has_slash(file)))
			return (exec_error("command not found", file), 127);
		return (0);
	}
	fd = open(file, O_RDONLY);
	if (fd == -1 && !access(file, F_OK))
		return (exec_error("Permission denied", file), 126);
	else if (fd == -1)
		return (exec_error("No such file or directory", file), 127);
	if (fstat(fd, &file_stat) == -1)
		return (perror("fstat"), close(fd), 0);
	if (!S_ISREG(file_stat.st_mode))
		return (exec_error("Is a directory", file), close(fd), 126);
	if (!(file_stat.st_mode & S_IXUSR))
		return (exec_error("Permission denied", file), close(fd), 126);
	return (close(fd), 0);
}

int	exit_with_clear(t_list **splitted, t_ht *env, t_flags *flags,
	long long last_status)
{
	long	exit_status;

	exit_status = ft_exit(((t_exec *)(*splitted)->content)->argc,
			((t_exec *)(*splitted)->content)->argv, last_status);
	if (((t_exec *)(*splitted)->content)->argc > 2 && exit_status == 1)
		return (1);
	if (flags)
		free_flags(flags);
	exit_prog(splitted, env, exit_status);
	return (exit_status);
}
