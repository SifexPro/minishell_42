/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 23:57:11 by pepie             #+#    #+#             */
/*   Updated: 2024/05/10 12:41:01 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <libgen.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# ifdef LINUX
#  include <linux/limits.h>
# else 
#  define PATH_MAX 1024
# endif

typedef struct s_str_input {
    char    *str;
    bool    is_double_quote;
    bool    is_simple_quote;
} t_str_input;

/* command/cd */
int     ft_cd(int argc, char **argv);

/* command/pwd */
int     ft_pwd(int argc, char **argv);
char    *get_pwd(void);

/* command/echo */
int     ft_echo(int argc, char **argv);

/* split_quote */
char    **ft_split_quote(char const *str);

/* utils */
int     ft_strarr_len(char **input);
void    free_split(char **splitted);

#endif