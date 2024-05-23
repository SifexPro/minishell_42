/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2024/05/23 04:03:54 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signals(int signo)
{
	if (signo == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
	{
	}
}

char	*get_prefix(void)
{
	char	*pwd;
	char	**splitted;
	char	*tmp;
	char	*resp;

	pwd = get_pwd();
	splitted = ft_split(pwd, '/');
	tmp = ft_strjoin("\e[1;35m[CUSTOM] \e[1;33m",
			splitted[ft_strarr_len(splitted) - 1]);
	resp = ft_strjoin(tmp, " > \e[0;37m");
	free(tmp);
	ft_freesplit(splitted);
	free(pwd);
	return (resp);
}

int run_program(char *path, char **argv)
{
    int i;

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

int	parse_cmd(char *input, t_ht *env)
{
	char	**splitted;
	char	**argv;
	int		argc;

	splitted = ft_split_quote(input, env);
	if (!splitted)
		return -1;
	free(input);
	argv = splitted;
	argv++;
	argc = ft_strarr_len(argv);
	if (ft_strncmp(splitted[0], "cd", 2) == 0)
		return (ft_cd(argc, argv, env));
	else if (ft_strncmp(splitted[0], "pwd", 3) == 0)
		return (ft_pwd(argc, argv));
	else if (ft_strncmp(splitted[0], "echo", 4) == 0)
		return (ft_echo(argc, argv));
	else if (ft_strncmp(splitted[0], "exit", 5) == 0)
		return (exit(0), 0);
    else
        return (run_program(splitted[0], argv));
}

int	main(void)
{
	char	*buffer;
	t_ht	*env;
    int     last_status;

	if (signal(SIGINT, handle_signals) == SIG_ERR)
	{
		printf("failed to register interrupts with kernel\n");
	}
	if (signal(SIGQUIT, handle_signals) == SIG_ERR)
	{
		printf("failed to register interrupts with kernel\n");
	}
	env = hashtable_create(64);
	if (!env)
		return (printf("failed to malloc!"), 1);
	hashtable_insert(env, "HOME", "/Users/polepie");
	buffer = readline(get_prefix());
	while (buffer != NULL)
	{
		if (buffer[0] != 0)
		{
			add_history(buffer);
			last_status = parse_cmd(buffer, env);
            hashtable_insert(env, "?", ft_uitoa(last_status));
		}
		buffer = readline(get_prefix());
	}
}
