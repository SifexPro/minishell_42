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

static int	check_dot(char *file)
{
	int	i;

	i = 0;
	while (file[i] == '.')
		i++;
	if (file[i] == '/' && i > 0)
		return (i - 1);
	return (0);
}

int	check_file(char *cmd_path, char *file)
{
	struct stat	file_stat;
	int			fd;
	int			file_len;
	int			cmd_path_len;

	file_len = ft_strlen(file) - 1;
	cmd_path_len = ft_strlen(cmd_path) - 1;
	if (!((cmd_path && !ft_strncmp(&cmd_path[check_dot(cmd_path)], "./", 2)
				|| !cmd_path && !ft_strncmp(&file[check_dot(file)], "./", 2))
			|| (!ft_strncmp(file, "/", 1))
			|| (cmd_path && !ft_strncmp(&cmd_path[cmd_path_len], "/", 1)
				|| !cmd_path && !ft_strncmp(&file[file_len], "/", 1))))
		return (0);
	fd = open(file, O_RDONLY);
	if (fd == -1 && !access(file, F_OK))
		return (exec_error("Permission denied", file), close(fd), 126);
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
