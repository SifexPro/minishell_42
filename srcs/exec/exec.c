/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2024/12/03 15:30:07 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_program_exec(char *path, char **argv, char **envp)
{
	int		i;
	char	*cmd_path;

	i = 0;
	if (!path)
		return (exit(1), 1); // to print
	cmd_path = get_cmd_path(path, get_path(envp));
	if (!cmd_path)
		return (exit(127), 127); // to print
	else if (access(cmd_path, X_OK))
		return (exit(126), 126); // to print
	else if (execve(cmd_path, argv, envp) < 0)
		return (printf("here"), exit(1), 1);
	/*while (argv[i])
	{
		printf("%s\n", argv[i]);
		i++;
	}*/
	return (0);
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

int	select_exec(char *prog, int argc, char **argv, t_ht *env, char **envp)
{
	if (ft_strncmp(prog, "cd", 2) == 0)
		return (ft_cd(argc, argv, env));
	else if (ft_strncmp(prog, "pwd", 3) == 0)
		return (ft_pwd(argc, argv));
	else if (ft_strncmp(prog, "echo", 4) == 0)
		return (ft_echo(argc, argv));
	else
		return (run_program(prog, argv, envp));
}

int	parse_cmd(char *input, t_ht *env, char **envp)
{
	char	**splitted;
	char	**argv;
	int		argc;
	int		res;

	splitted = ft_split_quote(input, env);
	if (!splitted)
		return (0);
	free(input);
	argv = splitted;
	argv++;
	argc = ft_strarr_len(argv);
	if (ft_strcmp(splitted[0], "exit") == 0)
		return (exit_prog(splitted, env));
	res = select_exec(splitted[0], argc, argv, env, envp);
	if (ht_search(env, "nl"))
	{
		free(argv[0]);
		argv[0] = ht_search(env, "nl");
		ht_deletef(env, "nl");
		parse_cmd(argv[0], env, envp);
	}
	ft_freesplit(splitted);
	free(splitted);
	return (res);
}
