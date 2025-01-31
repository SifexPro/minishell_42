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
	
	//if (path == NULL)////
	//	return (exec_error("failed to exec command", NULL), exit(1), 1);////
	cmd_path = get_cmd_path(path, get_path(envp));
	if (!cmd_path)
		return (exec_error("command not found", argv[0]), exit(127), 127);
	else if (access(cmd_path, X_OK))
		return (exec_error("permission denied", argv[0]), exit(126), 126);////check
	else if (execve(cmd_path, argv, envp) < 0)
		return (exec_error("failed to exec command", argv[0]), exit(1), 1);////check
	return (exit(0), 0);
}

int	run_program(char *path, char **argv, char **envp)
{
	pid_t	child;
	int		status;

	child = fork();
	if (child < 0)
		return (1);////real exit
	g_pid = child;
	if (!child)
		run_program_exec(path, argv, envp);
	waitpid(child, &status, 0);
	g_pid = 0;
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
	else if (!ft_strncmp(argv[0], "env", 3))
		return (ft_env(envp));
	else if (!ft_strncmp(argv[0], "unset", 5))
		return (ft_unset(argc, argv, env));
	else if (!ft_strncmp(argv[0], "export", 6))
		return (ft_export(argc, argv, env, envp));
	else
		return (run_program(argv[0], argv, envp));
}

void	free_splitted(void *v)
{
	t_list		*splitted;
	t_splitted	*tmp_splitted;

	tmp_splitted = v;
	if (tmp_splitted)
	{
		free(tmp_splitted);
	}
}
void	free_splitted_wc(void *v)
{
	t_list	*splitted;
	t_exec	*tmp_exec;
	int		i;

	tmp_exec = v;
	i = 0;
	if (tmp_exec && tmp_exec->argv)
	{
		while (tmp_exec->argv[i])
		{
			free(tmp_exec->argv[i]);
			i++;
		}
		free(tmp_exec->argv);
		free(tmp_exec);
	}
}

int	exit_with_clear(t_list **splitted, t_ht *env, t_flags *flags, long long last_status)
{
	int			i;
	t_exec		*temp;
	long long	exit_status;

	i = -1;
	temp = (*splitted)->content;
	exit_status = last_status;
	ft_putstr_fd("exit\n", 2);
	if (temp->argc == 2)
	{
		while (temp->argv[1][++i])
			if (!ft_isdigit(temp->argv[1][i]))
				exec_error("numeric argument required", "exit");
		exit_status = ft_atoi(temp->argv[1]);
	}
	else if (temp->argc > 2)
		return (exec_error("too many arguments", "exit"), 1);
	free_flags(flags);
	ft_lstclear(splitted, &free_splitted_wc);
	exit_prog(splitted, env, exit_status);
	return (exit_status);
}

int	parse_cmd(char *input, t_ht *env, char **envp, int last_status)

{
	t_list	*splitted;
	t_flags	*flags;
	t_exec	*temp;
	int		res;
	char	**envp_cpy;

	res = 0;
	splitted = ft_split_quote(input, env);
	if (!splitted)
		return (-1);
	free(input);

	////
	t_list	*temp_list = splitted;
	while (splitted)
	{
		ft_printf("splitted != NULL\n");////
		printf("((t_exec *)splitted->content)->argc: %d\n", ((t_exec *)splitted->content)->argc);////
		printf("((t_exec *)splitted->content)->argv[0]: %s\n", ((t_exec *)splitted->content)->argv[0]);////
		printf("((t_exec *)splitted->content)->argv[1]: %s\n", ((t_exec *)splitted->content)->argv[1]);////
		printf("((t_exec *)splitted->content)->token_next: %d\n", ((t_exec *)splitted->content)->token_next);////
		splitted = splitted->next;
	}
	splitted = temp_list;
	////

	//// < Makefile cat | wc -l> test
	flags = set_flags(splitted);

	printf("HERE\n\npipe_count: %d\n", flags->pipe_count);////
	printf("cmd_count: %d\n", flags->cmd_count);////
	printf("has_infile: %d\n", flags->has_infile);////
	printf("has_outfile: %d\n", flags->has_outfile);////
	printf("has_heredoc: %d\n", flags->has_heredoc);////
	printf("has_append: %d\n", flags->has_append);////
	printf("infile: %s\n", flags->infile);////
	printf("outfile: %s\n", flags->outfile);////
	printf("heredoc: %s\n", flags->heredoc);////
	int i = 0;
	while (i < flags->cmd_count)
	{
		printf("cmd[%d]: %s\n", i, flags->cmd[i]->argv[0]);////
		i++;
	}

	if (flags->pipe_count || flags->has_infile || flags->has_outfile || flags->has_heredoc) {
		res = forking(flags, env, envp);
	}
	else
	{
		temp = splitted->content;
		printf("res: %d\n", last_status);////
		if (ft_strcmp(temp->argv[0], "exit") == 0)
			return (exit_with_clear(&splitted, env, flags, last_status));
		envp_cpy = ht_to_envp(env);
		res = select_exec(temp->argc, temp->argv, env, envp_cpy);
		clear_env(envp_cpy);
	}
	ft_lstclear(&splitted, &free_splitted_wc);
	free_flags(flags);
	return (res);
}
 