/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cat.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:41:33 by pepie             #+#    #+#             */
/*   Updated: 2024/04/17 08:12:07 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CAT_H
# define FT_CAT_H

# include "minishell.h"

void	read_stdin(void);
void	read_file(char*name);
int     ft_cat(int argc, char **argv);

enum e_erros {
	NO_SUCH,
	IS_DIRECTORY
};

#endif