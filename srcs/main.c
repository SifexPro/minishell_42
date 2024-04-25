/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2024/04/18 07:31:51 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void show_text(void)
{
    char    *pwd;
    char    **splitted;

    pwd = get_pwd();
    splitted = ft_split(pwd, '/');
    ft_printf("[CUSTOM] %s>", splitted[ft_strarr_len(splitted) - 1]);
    free_split(splitted);
    free(pwd);
}



char    **parse_str_quote(char *input)
{
    char    **resp;

    resp = ft_split_quote(input);
    return (resp);
}

void    parse_cmd(char *input)
{
    char    **splitted;
    char    **argv;
    int     argc;

    splitted = parse_str_quote(input);
    argv = splitted;
    argv++;
    argc = ft_strarr_len(argv);
    if (ft_strncmp(splitted[0], "cd", 2) == 0)
        ft_cd(argc, argv);
    else if (ft_strncmp(splitted[0], "pwd", 3) == 0)
        ft_pwd(argc, argv);
    else if (ft_strncmp(splitted[0], "echo", 4) == 0)
        ft_echo(argc, argv);
    free_split(splitted);
    free(input);
}

int main(void)
{
    char	*buffer;

    show_text();
    buffer = get_next_line(STDIN_FILENO);
    while (buffer != NULL)
    {
        buffer[ft_strlen(buffer) - 1] = 0;
        parse_cmd(buffer);
        show_text();
        buffer = get_next_line(STDIN_FILENO);
    }
}