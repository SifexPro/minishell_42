/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 10:16:13 by pepie             #+#    #+#             */
/*   Updated: 2024/05/24 11:09:45 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_cat.h"

void	error(char*name, enum e_erros err)
{
	write(2, "ft_cat: ", 8);
	write(2, name, ft_strlen(name));
	if (err == IS_DIRECTORY)
		write(2, ": Is a directory\n", 17);
	else if (err == NO_SUCH)
		write(2, ": No such file or directory\n", 28);
}

int	verify_dir(char*name)
{
	int		file;

	file = open(name, O_DIRECTORY);
	if (file < 0)
	{
		return (0);
	}
	return (1);
}

void	read_file(char*name)
{
	int		file;
	char	chunk;

	if (verify_dir(name))
		return (error(name, IS_DIRECTORY));
	file = open(name, O_RDONLY);
	if (file == -1)
		return (error(name, NO_SUCH));
	while (read(file, &chunk, 1) > 0)
		write(1, &chunk, 1);
	close(file);
}

void	read_stdin(void)
{
	char	buffer[1024];

	while (read(0, buffer, 1024) != 0)
		write(1, &buffer, 1);
}
