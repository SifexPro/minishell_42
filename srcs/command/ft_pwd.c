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
	free(cwd);
	return (NULL);
}

int	ft_pwd(int argc, char **argv)
{
	char	*pwd;

	pwd = get_pwd();
	if (!pwd)
	{
		ft_putstr_fd("pwd: error retrieving current directory: getcwd:", 2);
		ft_putstr_fd(" cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		return (1);
	}
	printf("%s\n", pwd);
	(void)argc;
	(void)argv;
	return (0);
}
