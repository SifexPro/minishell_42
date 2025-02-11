/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:52:44 by Sifex             #+#    #+#             */
/*   Updated: 2025/02/06 18:52:58 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	run_program_exec_pipe(char *path, char **argv, char **envp)
{
	char	*cmd_path;
	int		check;

	if (path == NULL)
		return (clear_env(envp),
			exec_error("Failed to exec command", NULL), 1);
	cmd_path = get_cmd_path(path, get_path(envp));
	check = 0;
	if ((!cmd_path && !ft_strncmp(path, "./", 2))
		|| (cmd_path && !ft_strncmp(cmd_path, "./", 2)))
		check = check_file(path);
	if (check)
		return (free(cmd_path), check);
	if (!cmd_path)
		return (clear_env(envp), exec_error("Command not found", argv[0]), 127);
	else if (access(cmd_path, X_OK) && ft_strncmp(cmd_path, "./", 2))
		return (exec_error("command not found", path), free(cmd_path),
			exit(127), 127);
	else if (access(cmd_path, X_OK))
		return (clear_env(envp),
			exec_error("Permission denied", argv[0]), free(cmd_path), 126);
	else if (execve(cmd_path, argv, envp) < 0)
		return (clear_env(envp),
			exec_error("Failed to exec command", argv[0]), free(cmd_path), 1);
	return (0);
}

int	select_exec_pipe(int argc, char **argv, t_ht *env, char **envp)
{
	if (!ft_strncmp(argv[0], "cd", 2))
		return (ft_cd(argc, argv, env));
	else if (!ft_strncmp(argv[0], "pwd", 3))
		return (ft_pwd(argc, argv));
	else if (!ft_strncmp(argv[0], "echo", 4))
		return (ft_echo(argc, argv));
	else if (!ft_strncmp(argv[0], "env", 3))
		return (ft_env(envp));
	else if (!ft_strncmp(argv[0], "unset", 5))
		return (ft_unset(argc, argv, env));
	else if (!ft_strncmp(argv[0], "export", 6))
		return (ft_export(argc, argv, env, envp));
	else if (!ft_strcmp(argv[0], "exit"))
		return (ft_exit(argc, argv, -1));
	else
		return (run_program_exec_pipe(argv[0], argv, envp));
}
