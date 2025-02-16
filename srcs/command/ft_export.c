/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:14:35 by pepie             #+#    #+#             */
/*   Updated: 2024/05/24 11:10:08 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_plus(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (str[i - 1] == '+');
}

bool	is_valid_identifier(char *str)
{
	int		i;
	bool	plus;

	i = 0;
	plus = has_plus(str);
	while (str[i])
	{
		if (!ft_isalpha(str[i]) && str[i] != '_' && !plus)
			return (false);
		i++;
	}
	return (true);
}

int	process_line(char *argv, t_ht *env)
{
	char	**split_argv;
	char	*prev;
	char	*env_key;
	char	*new_value;

	split_argv = get_export_splitted(argv);
	if (!split_argv)
		return (1);
	prev = NULL;
	env_key = split_argv[0];
	if (has_plus(split_argv[0]))
	{
		env_key[ft_strlen(env_key) - 1] = '\0';
		prev = ht_search(env, env_key);
		if (prev)
		{
			prev = ft_strdup(prev);
			if (!prev)
				return (ft_freesplit(split_argv), free(split_argv), 1);
		}
	}
	ht_deletef(env, split_argv[0]);
	if (!split_argv[1])
		return (free(prev), ft_freesplit(split_argv), free(split_argv), 0);
	if (prev)
	{
		new_value = ft_strjoin(prev, split_argv[1]);
		free(prev);
		if (!new_value)
			return (ft_freesplit(split_argv), free(split_argv), 1);
		if (!ht_insert(env, split_argv[0], new_value))
			return (free(new_value), ft_freesplit(split_argv), free(split_argv), 1);
	}
	else
	{
		new_value = ft_strdup(split_argv[1]);
		if (!new_value)
			return (ft_freesplit(split_argv), free(split_argv), 1);
		if (!ht_insert(env, split_argv[0], new_value))
			return (free(new_value), ft_freesplit(split_argv), free(split_argv), 1);
	}
	return (ft_freesplit(split_argv), free(split_argv), 0);
}

void	show_export(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("export %s\n", envp[i]);
		i++;
	}
}

int	ft_export(int argc, char **argv, t_ht *env, char **envp)
{
	int		i;
	bool	has_error;

	i = 1;
	has_error = false;
	if (argc == 1)
	{
		show_export(envp);
		return (0);
	}
	while (argv[i])
	{
		if (process_line(argv[i], env) > 0)
		{
			has_error = true;
			ft_putstr_fd("bash: export: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("\': not a valid identifier\n", 2);
		}
		i++;
	}
	if (has_error)
		return (1);
	return (0);
}
