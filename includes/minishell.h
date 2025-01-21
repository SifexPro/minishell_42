/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 23:57:11 by pepie             #+#    #+#             */
/*   Updated: 2025/01/14 15:08:13 by Sifex            ###   ########.fr       */
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

typedef struct s_exec
{
	int			argc;
	char		**argv;
	char		**envp;
	t_tokens	token_next;
}			t_exec;

typedef struct s_flags
{
	int		cmd_count;
	int		pipe_count;
	char	*infile;
	char	*outfile;
	char	*heredoc;
	bool	has_infile;
	bool 	has_outfile;
	bool	has_append;
	bool	has_heredoc;
	pid_t	*pid;
	int		*fd_in;
	int		*fd_out;
	t_exec 	**cmd;
}			t_flags;
// flag for if has file in start & end

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

typedef struct s_splitted
{
	bool	is_delimiter;
	void	*content;
	int		delimiter;
}	t_splitted;

/* main */
int		parse_cmd(char *input, t_ht *env, char **envp);
int		exit_prog(t_list **splitted, t_ht *env);

/* signals */
void	handle_signals_edit(int signo);
void	handle_signals_cmd(int signo);
void	handle_signals_heredoc(int signo);

/* command/cd */
int		ft_cd(int argc, char **argv, t_ht *env);

/* command/pwd */
int		ft_pwd(int argc, char **argv);
char	*get_pwd(void);

/* command/echo */
int		ft_echo(int argc, char **argv);

/* command/cat */
int		ft_cat(int argc, char **argv);

/* split_quote */
t_list	*ft_split_quote(char const *str, t_ht *env);

/* split_quote_2 */
int		sq_replace_and_free(t_list *elements, t_list **ret);
int		init_string_quote(t_split_sh *sp);

/* parser/expansion */
void	register_env_var(t_ht *env, char **envp);
t_list	*create_str(char *str, bool is_simple_quote, t_ht *env);

/* parser/meta */
bool	is_meta(char c);
int		handle_meta(char const *str, t_split_sh *sp, t_list **elem);

/* utils */
int		ft_strarr_len(char **input);

/* exec/exec */
int		run_program_exec(char *path, char **argv, char **envp);
int		select_exec(int argc, char **argv, t_ht *env, char **envp);
void	free_splitted(void *splitted);
void	free_splitted_wc(void *splitted);

/* exec/cmd_path */
char	*get_path(char **envp);
char	*get_cmd_path(char *cmd, char *path);

/* exec/complex/cmd_flags */
t_flags	*set_flags(t_list *splitted);
void	free_flags(t_flags *flags);

/* exec/complex/pipe */
void	open_pipe(t_flags *flags);
void	close_pipe(t_flags *flags);

/* exec/complex/exec_pipe */
void	child_exec(t_flags *flags, int i, t_ht *env, char **envp);

/* exec/complex/fork */
void	forking(t_flags *flags, t_ht *env, char **envp);

/* exec/complex/open_files */
int		open_infile(t_flags *flags);
int		open_outfile(t_flags *flags);
void	open_heredoc(t_flags *flags);

#endif