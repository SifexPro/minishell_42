/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2024/09/24 14:01:24 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_program(char *path, char **argv)
{
	int	i;

	i = 0;
	printf("run program %s\n", path);
	printf("argv:\n");
	while (argv[i])
	{
		printf("%s\n", argv[i]);
		i++;
	}
	return (0);
}

int	select_exec(char *prog, int argc, char **argv, t_ht *env)
{
	if (ft_strncmp(prog, "cd", 2) == 0)
		return (ft_cd(argc, argv, env));
	else if (ft_strncmp(prog, "pwd", 3) == 0)
		return (ft_pwd(argc, argv));
	else if (ft_strncmp(prog, "echo", 4) == 0)
		return (ft_echo(argc, argv));
	else
		return (run_program(prog, argv));
}

int	parse_cmd(char *input, t_ht *env)
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
	res = select_exec(splitted[0], argc, argv, env);
	if (ht_search(env, "nl"))
	{
		free(argv[0]);
		argv[0] = ht_search(env, "nl");
		ht_deletef(env, "nl");
		parse_cmd(argv[0], env);
	}
	ft_freesplit(splitted);
	free(splitted);
	return (res);
}
