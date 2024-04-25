/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:18:44 by pepie             #+#    #+#             */
/*   Updated: 2024/04/17 15:38:03 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_strarr_len(char **input)
{
    int i;

    i = 0;
    while(input[i])
        i++;
    return (i);
}


void    free_split(char **splitted)
{
    int i;

    i = 0;
    while (splitted[i])
    {
        free(splitted[i]);
        i++;
    }
    free(splitted);
}