/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2025/01/14 15:11:18 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_program_exec(char *path, char **argv, char **envp)
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
		return (exit(1), 1);
	return (exit(0), 0);
}

int	run_program(char *path, char **argv, char **envp)
{
	pid_t	child;
	int		status;

	child = fork();
	if (!child)
		run_program_exec(path, argv, envp);
	waitpid(child, &status, 0);
	return (WEXITSTATUS(status));
}

int	select_exec(int argc, char **argv, t_ht *env, char **envp)
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
		return (run_program(argv[0], argv, envp));
}

void	handle_signals_cmd(int signo)
{
	if (signo == SIGINT)
	{
		printf("\n");
		rl_redisplay();
	}
}

int	parse_cmd(char *input, t_ht *env, char **envp)
{
	t_list	*splitted;
	t_flags	*flags;
	t_exec	*temp;
	int		res;

	res = 0;
	splitted = ft_split_quote(input, env);
	if (!splitted)
		return (0);
	free(input);
	flags = set_flags(splitted);
	if (signal(SIGINT, handle_signals_cmd) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
	if (signal(SIGQUIT, handle_signals_cmd) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");

	printf("HERE\n\npipe_count: %d\n", flags->pipe_count);////
	printf("cmd_count: %d\n", flags->cmd_count);////
	printf("infile: %s\n", flags->infile);////
	printf("cmd[0]: %s\n", flags->cmd[0]->argv[0]);////

	int i = 0;
	while (i < flags->cmd_count)
	{
		printf("cmd[%d]: %s\n", i, flags->cmd[i]->argv[0]);////
		i++;
	}
	if (flags->pipe_count || flags->has_infile || flags->has_outfile)
		forking(flags, env, envp);
	else
	{
		temp = splitted->content;
		if (ft_strcmp(temp->argv[0], "exit") == 0)
			return (exit_prog(&splitted, env));
		res = select_exec(temp->argc, temp->argv, env, envp);
	}
	return (res);
}
 