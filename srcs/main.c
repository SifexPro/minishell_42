/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2025/01/13 18:46:22 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_sig;////?
bool	g_ctrl_c;

int	exit_prog(t_list **splitted, t_ht *env, int status)
{
	ft_lstclear(splitted, &free_splitted_wc);
	hashtable_destroy(env);
	exit(status);
}

int	process_input(char *buffer, char *prefix, t_ht *env, char **envp)
{
	HIST_ENTRY	*le;
	char		*last_status_str;
	int			last_status;

	last_status = 0;
	ht_insert(env, "?", ft_strdup("0"));
	while (buffer != NULL)
	{
		if (buffer[0] != 0)
		{
			le = history_get(history_length);
			if (!le || (le && ft_strcmp(le->line, buffer) != 0))
				add_history(buffer);
			setup_cmd_signals();
			last_status = parse_cmd(buffer, env, envp, last_status);
			setup_term_signals();
			last_status = handle_lst_status(env, last_status);
		}
		prefix = get_prefix(last_status);
		buffer = readline(prefix);
		if (prefix)
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
	g_ctrl_c = false;
	g_sig = -1;
	env = hashtable_create(100);
	if (!env)
		return (printf("failed to malloc!"), 1);
	register_env_var(env, envp);
	prefix = get_prefix(0);
	setup_term_signals();
	buffer = readline(prefix);
	free(prefix);
	process_input(buffer, prefix, env, envp);
}
