/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:53:15 by Sifex             #+#    #+#             */
/*   Updated: 2025/01/16 15:53:15 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	run_program_exec_pipe(char *path, char **argv, char **envp)
{
	char	*cmd_path;

	if (path == NULL)////
		return (exec_error("failed to exec command", NULL), 1);////
	cmd_path = get_cmd_path(path, get_path(envp));
	if (!cmd_path)
		return (exec_error("command not found", argv[0]), 127);
	else if (access(cmd_path, X_OK))
		return (free(cmd_path), exec_error("permission denied", argv[0]), 126);////free(cmd_path) ? - check
	else if (execve(cmd_path, argv, envp) < 0)
		return (exec_error("failed to exec command", argv[0]), 1); ////check
	return (0);
}

static int	select_exec_pipe(int argc, char **argv, t_ht *env, char **envp)
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
		return (exit(ft_atoi(argv[1])), 0);
	else
		return (run_program_exec_pipe(argv[0], argv, envp));
}

void	child_exec(t_flags *flags, int i, t_ht *env, char **envp)
{
	char **envp_cpy;

	if (i == 0 && flags->infile)
	{
		if (!open_infile(flags))
			return close_pipe(flags), exit(1);////real exit
		dup2(flags->fd_in[i], 0);
	}
	else if (i == 0 && flags->has_heredoc)
		dup2(flags->fd_in[i], 0);
	if (i == flags->cmd_count - 1 && flags->outfile)
	{
		if (!open_outfile(flags))
			return close_pipe(flags), exit(1);////real exit
		dup2(flags->fd_out[i], 1);
	}
	if (i > 0)
		dup2(flags->fd_in[i], 0);
	if (i < flags->cmd_count - 1)
		dup2(flags->fd_out[i], 1);
	close_pipe(flags);
	envp_cpy = ht_to_envp(env);
	exit(select_exec_pipe(flags->cmd[i]->argc, flags->cmd[i]->argv, env, envp_cpy));
	clear_env(envp_cpy);
}



// check if has "file < command"
// check if has "command > file"
// check if has "command | command" //
// check if has combo of the above
// do the heredoc thing  

//// exit -> "real_exit"