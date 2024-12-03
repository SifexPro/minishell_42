/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 23:57:11 by pepie             #+#    #+#             */
/*   Updated: 2024/12/03 15:44:07 by pepie            ###   ########.fr       */
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
# include <signal.h>

# include <sys/wait.h>

# ifdef LINUX
#  include <linux/limits.h>
# endif
# ifndef PATH_MAX
#  define PATH_MAX 1024
# endif

typedef enum e_tokens
{
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	HEREDOC,
	APPEND,
	PIPE
}	t_tokens;

typedef struct s_str_input
{
	char	*str;
	bool	is_double_quote;
	bool	is_simple_quote;
}	t_str_input;

typedef struct s_split_sh
{
	int		i;
	int		str_start;
	int		quote_start;
	bool	is_simp_quote;
	bool	is_dbl_quote;
}	t_split_sh;

/* main */
int		parse_cmd(char *input, t_ht *env, char **envp);
int		exit_prog(char **splitted, t_ht *env);

/* command/cd */
int		ft_cd(int argc, char **argv, t_ht *env);

/* command/pwd */
int		ft_pwd(int argc, char **argv);
char	*get_pwd(void);

/* command/echo */
int		ft_echo(int argc, char **argv);

/* split_quote */
char	**ft_split_quote(char const *str, t_ht *env);

/* split_quote_2 */
int		sq_replace_and_free(t_list *elements, char **ret);
int		init_string_quote(t_split_sh *sp);

/* parser/expansion */
void	register_env_var(t_ht *env, char **envp);
t_list	*create_str(char *str, bool is_simple_quote, t_ht *env);

/* parser/meta */
bool	is_meta(char c);
int		handle_meta(char const *str, t_split_sh *sp, t_list **elem, t_ht *env);

/* utils */
int		ft_strarr_len(char **input);

/* exec/cmd_path */
char	*get_path(char **envp);
char	*get_cmd_path(char *cmd, char *path);

#endif