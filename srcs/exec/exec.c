/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2024/12/06 16:38:50 by Sifex            ###   ########.fr       */
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
		return (printf("here"), exit(1), 1);
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
	if (ft_strncmp(argv[0], "cd", 2) == 0)
		return (ft_cd(argc, argv, env));
	else if (ft_strncmp(argv[0], "pwd", 3) == 0)
		return (ft_pwd(argc, argv));
	else if (ft_strncmp(argv[0], "echo", 4) == 0)
		return (ft_echo(argc, argv));
	else
		return (run_program(argv[0], argv, envp));
}

int	parse_cmd(char *input, t_ht *env, char **envp)
{
	t_list	*splitted;
	t_exec	*temp;
	int		res;

	splitted = ft_split_quote(input, env);
	if (!splitted)
		return (0);
	free(input);
	while (splitted)
	{
		temp = splitted->content;
		//has_pipe(NULL);
		printf("=====================\n");
		printf("1 argc %d\n", temp->argc);
		printf("1 argv1: %s\n", temp->argv[0]);
		printf("1 tokens: %d\n", temp->token_next); 
		splitted = splitted->next;
		if (ft_strcmp(temp->argv[0], "exit") == 0)
			return (exit_prog(&splitted, env));
		res = select_exec(temp->argc, temp->argv, env, envp);
	}
	return (res);
}
