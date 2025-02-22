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

char	**custom_splitted(char *argv)
{
	char	**split_argv;
	int		i;

	i = 0;
	split_argv = malloc(sizeof(char *) * 3);
	if (!split_argv)
		return (NULL);
	while (argv[i] && argv[i] != '=')
		i++;
	split_argv[0] = ft_strndup(argv, i);
	split_argv[1] = ft_strdup(&argv[i + 1]);
	split_argv[2] = NULL;
	return (split_argv);
}

char	**get_export_splitted(char *argv)
{
	char	**split_argv;

	split_argv = custom_splitted(argv);
	if (!split_argv)
		return (NULL);
	if (!split_argv[0])
		return (ft_freesplit(split_argv), free(split_argv), NULL);
	if (!is_valid_export(split_argv[0]))
	{
		return (ft_freesplit(split_argv), free(split_argv), NULL);
	}
	return (split_argv);
}

bool	is_valid_export(char *str)
{
	if (!ft_isalpha(str[0]))
		return (false);
	if (!is_valid_env(str))
		return (false);
	return (true);
}
