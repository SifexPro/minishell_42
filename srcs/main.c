/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2024/05/10 13:14:40 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_signals(int signo) {
  if (signo == SIGINT) {
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
  }
}

char    *get_prefix(void)
{
    char    *pwd;
    char    **splitted;
    char    *tmp;
    char    *resp;

    pwd = get_pwd();
    splitted = ft_split(pwd, '/');
    tmp = ft_strjoin("\e[1;35m[CUSTOM] \e[1;33m", splitted[ft_strarr_len(splitted) - 1]);
    resp = ft_strjoin(tmp, " > \e[0;37m");
    free(tmp);
    free_split(splitted);
    free(pwd);
    return (resp);
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
    else if (ft_strncmp(splitted[0], "exit", 4) == 0)
        exit(0);
    free_split(splitted);
    free(input);
}

int main(void)
{
    char	*buffer;

    if (signal(SIGINT, handle_signals) == SIG_ERR) {
        printf("failed to register interrupts with kernel\n");
    }
    buffer = readline(get_prefix());
    while (buffer != NULL)
    {
        if (buffer[0] != 0) {
            add_history(buffer);
            parse_cmd(buffer);
        }
        buffer = readline(get_prefix());
    }
}