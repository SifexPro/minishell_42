/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 17:01:27 by Sifex             #+#    #+#             */
/*   Updated: 2024/02/09 17:12:28 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			printf("FOND");
			return (envp[i] + 5);
		}
		i++;
	}
	return (NULL);
}

static char	*get_cmd_path_res(char **paths, char *cmd_path)
{
	free_array(paths);
	return (cmd_path);
}

static char	*get_cmd_path_while(char **paths, char *cmd_path, char *cmd, int i)
{
	char	*cmd_path_first;

	while (paths[i])
	{
		cmd_path_first = ft_strjoin(paths[i], "/");
		if (!cmd_path_first)
			return (get_cmd_path_res(paths, NULL));
		cmd_path = ft_strjoin(cmd_path_first, cmd);
		free(cmd_path_first);
		if (!cmd_path)
			return (get_cmd_path_res(paths, NULL));
		if (access(cmd_path, F_OK) == 0)
			return (get_cmd_path_res(paths, cmd_path));
		free(cmd_path);
		i++;
	}
	return (get_cmd_path_res(paths, NULL));
}

char	*get_cmd_path(char *cmd, char *path)
{
	char	**paths;
	char	*cmd_path;
	int		i;

	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	if (!access(cmd, F_OK))
		return (get_cmd_path_res(paths, cmd));
	i = 0;
	cmd_path = NULL;
	return (get_cmd_path_while(paths, cmd_path, cmd, i));
}
