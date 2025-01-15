/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:27:00 by anaudibe          #+#    #+#             */
/*   Updated: 2025/01/14 15:09:15 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

///////////////////////////////////////////
int	run_program_exec_tmp(char *path, char **argv, char **envp)
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

int	run_program_tmp(char *path, char **argv, char **envp)
{
	return (run_program_exec_tmp(path, argv, envp));
}

int	select_exec_tmp(int argc, char **argv, t_ht *env, char **envp)
{
	/*if (!ft_strncmp(argv[0], "cd", 2))
		return (ft_cd(argc, argv, env));
	else if (!ft_strncmp(argv[0], "pwd", 3))
		return (ft_pwd(argc, argv));
	else if (!ft_strncmp(argv[0], "echo", 4))
		return (ft_echo(argc, argv));*/
	//else if (!ft_strncmp(argv[0], "cat", 3))
	//	return (ft_cat(argc, argv));
	//else
		return (run_program_tmp(argv[0], argv, envp));
}
///////////////////////////////////////////

void	open_pipe(t_flags *flags)
{
	int	i;
	int pipe_fd[2];

	i = 0;
	while (i < flags->pipe_count)
	{
		if (pipe(pipe_fd) < 0)
			exit(1);///exit if pipe is not successful
		flags->fd_in[i + 1] = pipe_fd[0];
		flags->fd_out[i] = pipe_fd[1];
		i++;
	}
}

void	close_pipe(t_flags *flags)
{
	int	i;

	i = 0;
	//checks
	while (i < flags->pipe_count)
	{
		close(flags->fd_in[i + 1]);
		close(flags->fd_out[i]);
		/*if (flags->fd_in && flags->fd_in[i] != -1)
		{
			close(flags->fd_in[i]);
			flags->fd_in[i] = -1;
		}
		if (flags->fd_out && flags->fd_out[i] != -1)
		{
			close(flags->fd_out[i]);
			flags->fd_out[i] = -1;
		}*/
		i++;
	}
}

void	child_exec(t_flags *flags, int i, t_ht *env, char **envp)
{
	if (i > 0)
		dup2(flags->fd_in[i], 0);
	if (i < flags->cmd_count - 1)
		dup2(flags->fd_out[i], 1);
	close_pipe(flags);
	exit(select_exec_tmp(flags->cmd[i]->argc, flags->cmd[i]->argv, env, envp));
}

void	forking(t_flags *flags, t_ht *env, char **envp)
{
	int		i;

	i = 0;
	open_pipe(flags);
	while (i < flags->cmd_count)
	{
		flags->pid[i] = fork();
		//if (ft_strcmp(temp->argv[0], "exit") == 0) // exit
		//	return (exit_prog(&splitted, env));
		if (flags->pid[i] < 0)
			exit(1);///exit if fork is not successful
		else if (flags->pid[i] == 0)
			child_exec(flags, i, env, envp);
		//printf("select_exec_tmp: -%d-", select_exec_tmp(flags->cmd[i]->argc, flags->cmd[i]->argv, temp));
		i++;
	}

	///////////////////////////////////////////
	i = 0;
	close_pipe(flags);
	int status;
	while (i < flags->cmd_count)
	{
		waitpid(flags->pid[i], &status, 0);
		i++;
	}
	printf("status : %d\n", WEXITSTATUS(status));
	///////////////////////////////////////////
}

// check if has "file < command"
// check if has "command > file"
// check if has "command | command" //
// check if has combo of the above
// do the heredoc thing  