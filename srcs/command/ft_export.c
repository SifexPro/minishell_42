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

int	process_line(char *argv, t_ht *env)
{
	char	**split_argv;

	split_argv = ft_split(argv, '=');
	if (!split_argv)
		return (1);
	if (!split_argv[0])
		return (ft_freesplit(split_argv), free(split_argv), 1);
	if (!ft_isalpha(split_argv[0][0]))
		return (ft_freesplit(split_argv), free(split_argv), 2);
	ht_deletef(env, split_argv[0]);
	if (!split_argv[1])
		return (ft_freesplit(split_argv), free(split_argv), 0);
	ht_insert(env, split_argv[0], ft_strdup(split_argv[1]));
	ft_freesplit(split_argv);
	free(split_argv);
	return (0);
}

int	ft_export(int argc, char **argv, t_ht *env, char **envp)
{
	int		i;
	
	i = 1;
	ft_printf("export %d\n", argc);
	if (argc == 1)
	{
		ft_env(envp);
		return (0);
	}
	while (argv[i])
	{
		if (process_line(argv[i], env) > 0)
		{
			printf("bash: export: `%s': not a valid identifier\n", argv[i]);
		}
		i++;
	}
	return (0);

}

