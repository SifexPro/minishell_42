/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2024/12/03 15:20:37 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signals(int signo)
{
	if (signo == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

char	*get_prefix(int last_status)
{
	char	*pwd;
	char	**splitted;
	char	*tmp;
	char	*status_str;

	pwd = get_pwd();
	status_str = ft_itoa(last_status);
	if (last_status == 0)
		tmp = ft_strjoin("\e[1;32m", status_str);
	else
		tmp = ft_strjoin("\e[1;31m", status_str);
	tmp = ft_strjoin_free(tmp, " \e[1;35m[CUSTOM] \e[1;33m");
	splitted = ft_split(pwd, '/');
	tmp = ft_strjoin_free(tmp,
			splitted[ft_strarr_len(splitted) - 1]);
	tmp = ft_strjoin_free(tmp, " > \e[0;37m");
	ft_freesplit(splitted);
	free(splitted);
	free(pwd);
	free(status_str);
	return (tmp);
}

int	exit_prog(char **splitted, t_ht *env)
{
	ft_freesplit(splitted);
	free(splitted);
	hashtable_destroy(env);
	exit(0);
}

int	process_input(char *buffer, char *prefix, t_ht *env, char **envp)
{
	char	*last_status_str;
	int		last_status;

	while (buffer != NULL)
	{
		if (buffer[0] != 0)
		{
			add_history(buffer);
			last_status = parse_cmd(buffer, env, envp);
			last_status_str = ft_uitoa(last_status);
			ht_deletef(env, "?");
			ht_insert(env, "?", last_status_str);
		}
		prefix = get_prefix(last_status);
		buffer = readline(prefix);
		free(prefix);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	*buffer;
	char	*prefix;
	t_ht	*env;

	(void)argc;
	(void)argv;
	printf("%d\n", getpid());
	if (signal(SIGINT, handle_signals) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
	if (signal(SIGQUIT, handle_signals) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
	env = hashtable_create(100);
	if (!env)
		return (printf("failed to malloc!"), 1);
	register_env_var(env, envp);
	prefix = get_prefix(0);
	buffer = readline(prefix);
	free(prefix);
	process_input(buffer, prefix, env, envp);
}
