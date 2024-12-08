/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:27:00 by anaudibe          #+#    #+#             */
/*   Updated: 2024/12/09 00:50:51 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

///////////////////////////////////////////
int	run_program_exec_tmp(char *path, char **argv, char **envp)
{
	char	*cmd_path;

	if (!path)
		return (exit(1), 1); // to print
	cmd_path = get_cmd_path(path, get_path(envp));
	if (!cmd_path)
		return (exit(127), 127); // to print
	else if (access(cmd_path, X_OK))
		return (exit(126), 126); // to print
	else if (execve(cmd_path, argv, envp) < 0)
		return (printf("here"), exit(1), 1);
	return (exit(6), 6);
}

int	run_program_tmp(char *path, char **argv, char **envp)
{
	return (run_program_exec_tmp(path, argv, envp));
}

int	select_exec_tmp(int argc, char **argv, t_ht *env, char **envp)
{
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
		flags->fd_in[i] = pipe_fd[0];//i + 1
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
		if (flags->fd_in && flags->fd_in[i] != -1)
		{
			close(flags->fd_in[i]);
			flags->fd_in[i] = -1;
		}
		if (flags->fd_out && flags->fd_out[i] != -1)
		{
			close(flags->fd_out[i]);
			flags->fd_out[i] = -1;
		}	
		i++;
	}
}

void	child_exec(t_flags *flags, int i, t_ht *env)
{
	/*if (i > 0) {
        dup2(flags->fd_in[i - 1], 0);
    }
    if (i < flags->cmd_count - 1) {
        dup2(flags->fd_out[i], 1);
    }*/
    
    close_pipe(flags);
	//exit(select_exec_tmp(flags->cmd[i]->argc, flags->cmd[i]->argv, env, flags->cmd[i]->envp));
	//execve(flags->cmd[i]->argv[0], flags->cmd[i]->argv, flags->cmd[i]->envp);
	//select_exec_tmp(flags->cmd[i]->argc, flags->cmd[i]->argv, env, flags->cmd[i]->envp);
	exit(13);
}
// if (i == 0)
	// open file in if has
	//dup2(flags->fd_in[i], 0);
		
	// open file out if has
	//dup2(flags->fd_out[i], 1);
	
	//close(flags->fd_in[i]);
	//close(flags->fd_out[i]);
	
	//close_pipe(flags);
	//printf("%d -> flags->fd_in[%d]: %d\n", i, i, flags->fd_in[i]);

void	forking(t_flags *flags, t_ht *env)
{
	int		i;

	i = 0;
	open_pipe(flags);
	while (i < flags->cmd_count)
	{
		flags->pid[i] = fork();
		if (flags->pid[i] < 0)
			exit(1);///exit if fork is not successful
		else if (flags->pid[i] == 0)
			child_exec(flags, i, env);
		//if (ft_strcmp(temp->argv[0], "exit") == 0)
		//	return (exit_prog(&splitted, env));
		//printf("temp->argv[0]); %s\n", temp->argv[0]);
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
	printf("status : %d\n", status);
	printf("status : %d\n", WEXITSTATUS(status));
	///////////////////////////////////////////
}

// check if has "file < command"
// check if has "command > file"
// check if has "command | command"
// check if has combo of the above
// do the heredoc thing  