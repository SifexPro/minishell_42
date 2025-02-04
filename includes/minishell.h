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
# include <sys/stat.h>
# include <sys/time.h>////
# include <poll.h>////

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
	int			i;
	char		**argv;
	char		**envp;
	t_tokens	token_next;
}			t_exec;

typedef struct s_file
{
	char	*file;
	bool	is_infile;
	bool	is_outfile;
	bool	is_heredoc;
	bool	is_append;
}			t_file;

typedef struct s_pipe
{
	int		index;
	int		index_max;
	int		infile_index;
	int		infile_max;
	int		infile_nb;
	t_file	**infile;
	int		outfile_index;
	int		outfile_max;
	int		outfile_nb;
	t_file	**outfile;
	t_exec	*cmd;
}			t_pipe;

typedef struct s_flags
{
	bool	multi_exec;
	int		total_redir;
	pid_t	*pid;
	int		*fd_in;
	int		*fd_out;
	int		pipe_index;
	int		pipe_nb;//// ?
	bool	pipe_index_changed;
	t_pipe	**pipe;
	t_list	*splitted;//// remove ?
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
	bool	prev_meta;
	char	*pretext;
}	t_split_sh;

typedef struct s_splitted
{
	bool	is_delimiter;
	void	*content;
	int		delimiter;
}	t_splitted;

/* main */
int		parse_cmd(char *input, t_ht *env, char **envp, int last_status);
int		exit_prog(t_list **splitted, t_ht *env, int status);

/* prefix */
char	*get_prefix(int last_status);
int		handle_lst_status(t_ht *env, int last_status);

/* signals */
void	handle_signals_heredoc(int signo);
void	setup_term_signals(void);
void	setup_cmd_signals(void);
void	setup_heredoc_signals(void);

/* command/cd */
int		ft_cd(int argc, char **argv, t_ht *env);

/* command/pwd */
int		ft_pwd(int argc, char **argv);
char	*get_pwd(void);

/* command/echo */
int		ft_echo(int argc, char **argv);

/* command/unset */
int		ft_unset(int argc, char **argv, t_ht *env);

/* command/export */
int		ft_export(int argc, char **argv, t_ht *env, char **envp);

/* command/export_2 */
char	**get_export_splitted(char *argv);
bool	is_valid_export(char *str);

/* command/env */
int		ft_env(char **envp);

/* command/clear_env */
void	clear_env(char **envp);

/* command/exit */
long	ft_exit(int argc, char **argv, long long last_status);

/* split_quote */
t_list	*ft_split_quote(char const *str, t_ht *env);

/* split_quote_2 */
int		sq_replace_and_free(t_list *elements, t_list **ret, t_ht *env);
int		init_string_quote(t_split_sh *sp);

/* split_quote_3 */
void	handle_start(t_split_sh *sp, t_list **elem, char *str, t_ht *env);
void	handle_pretext(t_list **elem, char *str, t_split_sh *sp, t_ht *env);
void	concat_pretext(t_list **elem, char *str, t_split_sh *sp, t_ht *env);
int		no_quote(char const *str, t_split_sh *sp, t_list **elem, t_ht *env);

/* parsing */
void	fix_argc(t_list *ret);

/* parser/expansion */
void	register_env_var(t_ht *env, char **envp);
t_list	*create_str(char *str, bool is_simple_quote, t_ht *env);
char	*handle_expansion(char *str, t_ht *env);
char	*get_var_from_str(char *str, t_ht *env);
bool	is_valid_env(char *str);

/* parser/meta */
bool	is_meta(char c);
int		handle_meta(char const *str, t_split_sh *sp, t_list **elem);

/* parser/environement */
char	**ht_to_envp(t_ht *env);
void	clear_envp(char **env);

/* utils */
int		ft_strarr_len(char **input);

/* exec/exec */
int		run_program_exec(char *path, char **argv, char **envp);
int		select_exec(int argc, char **argv, t_ht *env, char **envp);
void	free_splitted(void *splitted);
void	free_splitted_wc(void *splitted);
void	free_splitted_c(void *v);

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
int		forking(t_flags *flags, t_ht *env, char **envp);

/* exec/complex/open_files */
int		open_infile(int index, t_flags *flags);
int		open_outfile(int index, t_flags *flags);
int		open_heredoc(int index, t_flags *flags);

/* exec/complex/exec_utils */
void	exec_error(char *error, char *cmd);
void	exec_error_arg(char *error, char *arg, char *cmd);
void	file_error(char *error, char *file);

extern int	g_pid;
extern bool	g_ctrl_c;

#endif