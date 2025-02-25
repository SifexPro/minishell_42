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

static int	run_program_exec(char *path, char **argv, char **envp,
	t_flags *flags)
{
	char	*cmd_path;
	int		check;

	if (!ft_strcmp(path, ""))
		return (exec_error_free("command not found", NULL, flags), exit(127),
			127);
	cmd_path = get_cmd_path(path, get_path(envp));
	check = check_file(cmd_path, path);
	if (check)
		return (free_child(flags->env, flags->splitted, flags, envp),
			exit(check), 0);
	if (!cmd_path)
		return (exec_error_free("command not found", argv[0], flags),
			exit(127), 127);
	else if (access(cmd_path, X_OK)
		&& (ft_strncmp(cmd_path, ".", 1) || ft_strncmp(cmd_path, "/", 1)))
		return (exec_error_free("command not found", path, flags),
			exit(127), 127);
	else if (access(cmd_path, X_OK))
		return (exec_error_free("Permission denied", path, flags),
			exit(126), 126);
	else if (execve(cmd_path, argv, envp) < 0)
		return (exec_error_free("failed to exec command", argv[0], flags),
			exit(1), 1);
	return (exit(0), 0);
}

static int	run_program(char *path, char **argv, char **envp, t_flags *flags)
{
	pid_t	child;
	int		status;

	child = fork();
	if (child < 0)
		return (1);
	if (!child)
		run_program_exec(path, argv, envp, flags);
	waitpid(child, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (130);
	clear_env(envp);
	flags->env = NULL;
	flags->envp = NULL;
	return (WEXITSTATUS(status));
}

static int	select_exec(int argc, char **argv, t_ht *env, t_flags *flags)
{
	char	**envp;

	envp = ht_to_envp(env);
	if (!envp)
		return (1);
	flags->env = env;
	flags->envp = envp;
	if (!ft_strcmp(argv[0], "cd"))
		return (clear_env(envp), ft_cd(argc, argv, env));
	else if (!ft_strcmp(argv[0], "pwd"))
		return (clear_env(envp), ft_pwd(argc, argv));
	else if (!ft_strcmp(argv[0], "echo"))
		return (clear_env(envp), ft_echo(argc, argv));
	else if (!ft_strcmp(argv[0], "env"))
		return (ft_env(envp), clear_env(envp), 0);
	else if (!ft_strcmp(argv[0], "unset"))
		return (clear_env(envp), ft_unset(argc, argv, env));
	else if (!ft_strcmp(argv[0], "export"))
		return (ft_export(argc, argv, env, envp));
	else
		return (run_program(argv[0], argv, envp, flags));
}

static int	parsec_cmd_exec(t_flags	*flags, t_list *splitted, t_ht *env,
		int last_status)
{
	t_exec	*temp;
	int		res;

	if (flags->multi_exec)
		res = forking(flags, splitted, env);
	else
	{
		temp = splitted->content;
		if (ft_strcmp(temp->argv[0], "exit") == 0)
			return (exit_with_clear(&splitted, env, flags, last_status));
		flags->splitted = splitted;
		res = select_exec(temp->argc, temp->argv, env, flags);
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
