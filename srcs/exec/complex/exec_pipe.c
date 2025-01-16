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

	if (!path)
		return (1); // to print
	cmd_path = get_cmd_path(path, get_path(envp));
	if (!cmd_path)
		return (127); // to print
	else if (access(cmd_path, X_OK))
		return (126); // to print
	else if (execve(cmd_path, argv, envp) < 0)
		return (1);
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
	//else if (!ft_strncmp(argv[0], "cat", 3))
	//	return (ft_cat(argc, argv));
	else
		return (run_program_exec_pipe(argv[0], argv, envp));
}

void	child_exec(t_flags *flags, int i, t_ht *env, char **envp)
{
	if (i > 0)
		dup2(flags->fd_in[i], 0);
	if (i < flags->cmd_count - 1)
		dup2(flags->fd_out[i], 1);
	close_pipe(flags);
	exit(select_exec_pipe(flags->cmd[i]->argc, flags->cmd[i]->argv, env, envp));
}



// check if has "file < command"
// check if has "command > file"
// check if has "command | command" //
// check if has combo of the above
// do the heredoc thing  