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
		return (ft_exit(argc, argv, -1));
	else
		return (run_program_exec_pipe(argv[0], argv, envp));
}

////
void read_from_fd(int fd, char *text, bool print) {
    char buffer[1024];
    ssize_t bytesRead;
    
    struct pollfd pfd = { .fd = fd, .events = POLLIN };

    ft_putstr_fd("\n[READ_FROM_FD] ", 2);
	ft_putendl_fd(text, 2);////
    // Vérifier si des données sont disponibles
    int ret = poll(&pfd, 1, 0); // Timeout = 0 → ne bloque pas
    if (ret == 0) {
        ft_putstr_fd("fd is null\n", 2);
        return ;
    } else if (ret == -1) {
        perror("poll");
        return;
    }
	else
	{
        ft_putstr_fd("fd is not null\n", 2);
	}
	

	if (print)
	{
		// Lire uniquement si `poll()` dit qu'il y a des données
		while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0 && ret != 0) {
			buffer[bytesRead] = '\0';
			ft_putstr_fd("Child received: ", 2);
			ft_putstr_fd(buffer, 2);
		}
	}
    ft_putstr_fd("[READ_FROM_FD] ", 2);
	ft_putendl_fd(text, 2);////
}
////

void	child_exec(t_flags *flags, int i, t_ht *env, char **envp)
{
	char	**envp_cpy;
	int		infile_index;
	int		outfile_index;
	int		pipe_index;

	//ft_printf("here\n");////
	//ft_putstr_fd("\n[CHILD_EXEC]\n", 2);////
	pipe_index = flags->pipe_index;
	infile_index = flags->pipe[pipe_index]->infile_index;
	outfile_index = flags->pipe[pipe_index]->outfile_index;
	/*printf("flags->pipe[pipe_index]->index = %d\n", flags->pipe[pipe_index]->index);////
	printf("flags->pipe[pipe_index]->index_max = %d\n", flags->pipe[pipe_index]->index_max);////
	printf("pipe_index = %d\n", pipe_index);////*/
	//printf("flags->pipe_nb = %d\n", flags->pipe_nb);////
	//printf("i = %d\n", i);////
	//printf("i = %d\nflags->pipe_index = %d\nflags->pipe[flags->pipe_index]->infile_index = %d\nflags->pipe[flags->pipe_index]->infile_nb = %d\nflags->pipe[flags->pipe_index]->outfile_index = %d\nflags->pipe[flags->pipe_index]->outfile_nb = %d\n", i, flags->pipe_index, flags->pipe[flags->pipe_index]->infile_index, flags->pipe[flags->pipe_index]->infile_nb, flags->pipe[flags->pipe_index]->outfile_index, flags->pipe[flags->pipe_index]->outfile_nb);////

	/*ft_putstr_fd("condition : ", 2);////
	//ft_putnbr_fd(infile_index < flags->pipe[pipe_index]->infile_nb, 2);////
	ft_putnbr_fd(infile_index != -1 && infile_index < flags->pipe[pipe_index]->infile_nb 
	&& outfile_index + 1 >= flags->pipe[pipe_index]->outfile_nb, 2);////
	ft_putstr_fd("\n", 2);////*/
	if (infile_index != -1 && infile_index < flags->pipe[pipe_index]->infile_nb)	
//&& outfile_index + 1 >= flags->pipe[pipe_index]->outfile_nb)
	{
		//ft_putstr_fd("infile\n", 2);////
		if (!flags->pipe[pipe_index]->infile[infile_index]->is_heredoc)
		{
			if (!open_infile(i, flags))
				return (close_pipe(flags), exit(1));////real exit
		}
		else
		{
			if (!open_heredoc(i, flags))
				return (close_pipe(flags), exit(1));////real exit
		}
		dup2(flags->fd_in[i], 0);
		//ft_putendl_fd("OPENFILE: dup2(flags->fd_in[i], 0);", 2);////
	}
	if (outfile_index != -1 && outfile_index < flags->pipe[pipe_index]->outfile_nb)
	{
		//if (flags->pipe[pipe_index]->outfile_nb == 1 || flags->pipe[pipe_index]->outfile_nb > 1 && infile_index >= flags->pipe[pipe_index]->infile_nb - 1)
		//{
			//ft_putstr_fd("outfile\n", 2);////
			if (!open_outfile(i, flags))
				return (close_pipe(flags), exit(1));////real exit
			dup2(flags->fd_out[i], 1);
			//ft_putendl_fd("OPENOUTFILE: dup2(flags->fd_out[i], 1);", 2);////
		//}
	}


	//if (flags->pipe[pipe_index]->index > 0)
	//ft_putstr_fd("IS : pipe_index > 0\n", 2);////
	//ft_putnbr_fd(pipe_index > 0, 2);////
	//ft_putstr_fd("\n", 2);////
	if (pipe_index > 0)
	{
		//ft_putendl_fd("dup2(flags->fd_in[i], 0);", 2);////
		dup2(flags->fd_in[i], 0);
	}
	/*ft_putstr_fd("IS : pipe_index < flags->pipe_nb - 1\n", 2);////
	ft_putnbr_fd(pipe_index < flags->pipe_nb - 1, 2);////
	ft_putstr_fd("\n", 2);////*/
	//if (flags->pipe[pipe_index]->index < flags->pipe[pipe_index]->index_max)
	if (pipe_index < flags->pipe_nb - 1)
	{
		//ft_putendl_fd("dup2(flags->fd_out[i], 1);", 2);////
		dup2(flags->fd_out[i], 1);
	}

	////
	//read_from_fd(flags->fd_in[i], "fd_in", false);////
	//read_from_fd(flags->fd_out[i], "fd_out", false);////
	////

	close_pipe(flags);

	//if (!flags->pipe[pipe_index]->cmd)
	//	ft_putstr_fd("no cmd\n", 2);////

	envp_cpy = ht_to_envp(env);

	
	if (infile_index >= flags->pipe[pipe_index]->infile_nb - 1)
	{
		//&& flags->pipe[pipe_index]->outfile_nb <= 1
		if (outfile_index >= flags->pipe[pipe_index]->outfile_nb - 1)
		{
			/*ft_putendl_fd("[EXEC]\n", 2);////
			ft_putnbr_fd(flags->pipe[pipe_index]->cmd->argc, 2);////
			ft_putstr_fd("\n", 2);////
			ft_putendl_fd(flags->pipe[pipe_index]->cmd->argv[0], 2);////*/
			exit(select_exec_pipe(flags->pipe[pipe_index]->cmd->argc, flags->pipe[pipe_index]->cmd->argv, env, envp_cpy));
		}
	}
	/*else if (infile_index >= flags->pipe[pipe_index]->infile_nb - 1 && flags->pipe[pipe_index]->outfile_nb > 1)
	{
		ft_putendl_fd("[EXEC]\n", 2);////
		ft_putnbr_fd(flags->pipe[pipe_index]->cmd->argc, 2);////
		ft_putstr_fd("\n", 2);////
		ft_putendl_fd(flags->pipe[pipe_index]->cmd->argv[0], 2);////
		exit(select_exec_pipe(flags->pipe[pipe_index]->cmd->argc, flags->pipe[pipe_index]->cmd->argv, env, envp_cpy));
	}*/
	//ft_putendl_fd("[NO EXEC]\n", 2);////
	exit(0);
	clear_env(envp_cpy);////useless (after exit)
}

// check if has "file < command"
// check if has "command > file"
// check if has "command | command" //
// check if has combo of the above
// do the heredoc thing  

//// exit -> "real_exit"