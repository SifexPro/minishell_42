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

static int	run_program_exec(char *path, char **argv, char **envp)
{
	char	*cmd_path;
	int		check;

	if (!ft_strcmp(path, ""))
		return (exec_error("command not found", NULL), exit(127), 127);
	cmd_path = get_cmd_path(path, get_path(envp));
	check = check_file(cmd_path, path);
	if (check)
		return (free(cmd_path), exit(check), 0);
	if (!cmd_path)
		return (exec_error("command not found", argv[0]), exit(127), 127);
	else if (access(cmd_path, X_OK)
		&& (ft_strncmp(cmd_path, ".", 1) || ft_strncmp(cmd_path, "/", 1)))
		return (exec_error("command not found", path),
			free(cmd_path), exit(127), 127);
	else if (access(cmd_path, X_OK))
		return (exec_error("Permission denied", path),
			free(cmd_path), exit(126), 126);
	else if (execve(cmd_path, argv, envp) < 0)
		return (exec_error("failed to exec command", argv[0]),
			free(cmd_path), exit(1), 1);
	return (exit(0), 0);
}

static int	run_program(char *path, char **argv, char **envp)
{
	pid_t	child;
	int		status;

	child = fork();
	if (child < 0)
		return (1);
	if (!child)
		run_program_exec(path, argv, envp);
	waitpid(child, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (130);
	return (WEXITSTATUS(status));
}

static int	select_exec(int argc, char **argv, t_ht *env, char **envp)
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
	else
		return (run_program(argv[0], argv, envp));
}

static int	parsec_cmd_exec(t_flags	*flags, t_list *splitted, t_ht *env,
		int last_status)
{
	t_exec	*temp;
	char	**envp_cpy;
	int		res;

	if (flags->multi_exec)
		res = forking(flags, splitted, env);
	else
	{
		temp = splitted->content;
		if (ft_strcmp(temp->argv[0], "exit") == 0)
			return (exit_with_clear(&splitted, env, flags, last_status));
		envp_cpy = ht_to_envp(env);
		if (!envp_cpy)
			return (1);
		res = select_exec(temp->argc, temp->argv, env, envp_cpy);
		clear_env(envp_cpy);
	}
	return (res);
}

int	parse_cmd(char *input, t_ht *env, int last_status)
{
	t_list	*splitted;
	t_flags	*flags;
	int		res;

	splitted = ft_split_quote(input, env);
	free(input);
	if (!splitted)
		return (-1);
	flags = set_flags(splitted);
	if (!flags)
		return (exit_with_clear(&splitted, env, NULL, -1));
	res = parsec_cmd_exec(flags, splitted, env, last_status);
	free_flags(flags);
	ft_lstclear(&splitted, &free_splitted_wc);
	return (res);
}
