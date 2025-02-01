/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:14:35 by pepie             #+#    #+#             */
/*   Updated: 2024/05/24 11:10:08 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_pwd(void)
{
	char	*cwd;

	cwd = malloc(sizeof(char) * PATH_MAX);
	if (!cwd)
		return (NULL);
	if (getcwd(cwd, sizeof(char) * PATH_MAX) != NULL)
		return (cwd);
	else
		return (NULL);
}

int	ft_pwd(int argc, char **argv)
{
	char	*pwd;

	pwd = get_pwd();
	if (!pwd)
	{
		printf("pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
		return (1);
	}
	printf("%s\n", pwd);
	(void)argc;
	(void)argv;
	return (0);
}
