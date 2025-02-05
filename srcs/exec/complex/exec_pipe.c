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

	if (path == NULL)
		return (clear_env(envp),
			exec_error("failed to exec command", NULL), 1); ////
	cmd_path = get_cmd_path(path, get_path(envp));
	if (!cmd_path)
		return (clear_env(envp),
			exec_error("command not found", argv[0]), 127);
	else if (access(cmd_path, X_OK))
		return (clear_env(envp),
			free(cmd_path), exec_error("permission denied", argv[0]), 126); ////free(cmd_path) ? - check
	else if (execve(cmd_path, argv, envp) < 0)
		return (clear_env(envp),
			exec_error("failed to exec command", argv[0]), 1); ////check
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

static void	check_files(t_flags *flags, int infile_i, int outfile_i)
{
	int	pipe_index;

	pipe_index = flags->pipe_index;
	if (flags->pipe[pipe_index]->infile_max != -1
		&& infile_i >= flags->pipe[pipe_index]->infile_max)
	{
		while (flags->pipe[pipe_index]->infile[infile_i - 1]->index
			> flags->pipe[pipe_index]->outfile[outfile_i]->index
			&& flags->pipe[pipe_index]->outfile_index
			< flags->pipe[pipe_index]->outfile_nb)
		{
			if (!open_outfile(0, flags))
				break ;
			flags->pipe[pipe_index]->outfile_index++;
		}
		return (close_pipe(flags), exit(1));////real exit
	}
	else if (flags->pipe[pipe_index]->outfile_max != -1
		&& outfile_i >= flags->pipe[pipe_index]->outfile_max)
		return (close_pipe(flags), exit(1));////real exit
}

static void	handle_files(t_flags *flags, int infile_i, int outfile_i, int i)
{
	int	pipe_index;

	pipe_index = flags->pipe_index;
	check_files(flags, infile_i, outfile_i);
	if (infile_i != -1 && infile_i < flags->pipe[pipe_index]->infile_nb)
	{
		if (!flags->pipe[pipe_index]->infile[infile_i]->is_heredoc)
			if (!open_infile(i, flags))
				return (close_pipe(flags), exit(1));////real exit
		else
			if (!open_heredoc(i, flags))
				return (close_pipe(flags), exit(1));////real exit
		dup2(flags->fd_in[i], 0);
	}
	if (outfile_i != -1 && outfile_i < flags->pipe[pipe_index]->outfile_nb)
	{
		if (!open_outfile(i, flags))
			return (close_pipe(flags), exit(1));////real exit
		dup2(flags->fd_out[i], 1);
	}
}

void	child_exec(t_flags *flags, int i, t_ht *env, char **envp)
{
	char	**envp_cpy;
	int		infile_index;
	int		outfile_index;
	int		pipe_index;

	pipe_index = flags->pipe_index;
	infile_index = flags->pipe[pipe_index]->infile_index;
	outfile_index = flags->pipe[pipe_index]->outfile_index;
	handle_files(flags, infile_index, outfile_index, i);
	if (pipe_index > 0)
		dup2(flags->fd_in[i], 0);
	if (pipe_index < flags->pipe_nb - 1)
		dup2(flags->fd_out[i], 1);
	close_pipe(flags);
	envp_cpy = ht_to_envp(env);
	if (infile_index >= flags->pipe[pipe_index]->infile_nb - 1)
		if (outfile_index >= flags->pipe[pipe_index]->outfile_nb - 1)
			exit(select_exec_pipe(flags->pipe[pipe_index]->cmd->argc,
					flags->pipe[pipe_index]->cmd->argv, env, envp_cpy));
	clear_env(envp_cpy);
	exit(0);
}
//// exit -> "real_exit"