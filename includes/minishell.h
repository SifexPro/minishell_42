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
	int		index;
}			t_file;

typedef struct s_pipe
{
	int		infile_nb;
	t_file	**infile;
	int		outfile_nb;
	t_file	**outfile;
	t_exec	*cmd;
}			t_pipe;

typedef struct s_flags
{
	bool	multi_exec;
	pid_t	*pid;
	int		*fd_in;
	int		*fd_out;
	int		pipe_index;
	int		pipe_nb;
	t_pipe	**pipe;
	t_ht	*env;
	char	**envp;
	t_list	*splitted;
}			t_flags;

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
	int		prev_meta;
	char	*pretext;
}	t_split_sh;

typedef struct s_splitted
{
	bool	is_delimiter;
	void	*content;
	int		delimiter;
}	t_splitted;

typedef struct s_parsing
{
	bool		can_error;
	bool		has_started;
	t_list		*elements;
	t_exec		*last_neutral;
	t_exec		*tmp_exec;
	t_splitted	*tmp;
	t_list		**ret;
	int			delimiter;
}	t_pars;

/* main */
int		parse_cmd(char *input, t_ht *env, int last_status);
int		exit_prog(t_list **splitted, t_ht *env, int status);

/* prefix */

char	*get_prefix(int last_status);
int		handle_lst_status(t_ht *env, int last_status);

/* signals */
void	handle_signals_heredoc(int signo);
void	setup_term_signals(void);
void	setup_cmd_signals(void);
void	setup_heredoc_signals(void);
void	handle_sigint_noredisplay(int signo);
void	handle_sigint_null(int signo);

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
bool	has_plus(char *str);
bool	has_equal(char *str);

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
void	handle_sigint(int signo);
void	sigquit_handler(int signo);

/* split_quote_2 */
int		sq_replace_and_free(t_list *elements, t_list **ret, t_ht *env);
int		init_string_quote(t_split_sh *sp);
int		count_until_del(t_list *ret);
t_exec	*init_exec(void);
bool	create_argv(t_exec *tmp_exec, t_list *elements);

/* split_quote_3 */
void	handle_start(t_split_sh *sp, char *str, t_ht *env);
void	handle_pretext(t_list **elem, char *str, t_split_sh *sp, t_ht *env);
void	handle_pretext_env(t_list **elem, char *str, t_split_sh *sp, t_ht *env);
void	concat_pretext(char *str, t_split_sh *sp);
int		no_quote(char const *str, t_split_sh *sp, t_list **elem, t_ht *env);

/* split_quote_4 */
void	append_to_argv(t_pars *pars, t_exec *elem);
int		handle_delimiter(t_pars *pars, t_ht *env);
int		pipe_case(t_pars *pars, t_splitted *tmp);
int		norme_2(t_pars *pars, int delimiter);
int		norme_1(t_pars *pars, int delimiter);
int		handle_reformat_start(t_pars *pars, int delimiter);
int		check_pipe_error(t_pars *pars, t_splitted *tmp, int delimiter,
			t_ht *env);

/* split_quote_5 */
int		process_next_elem(t_pars *pars, int delimiter);
int		to_argv(t_pars *pars);
void	end_replace(t_pars *pars);
t_pars	*create_pars(t_list **ret, t_list *elements);

char	**custom_splitted(char *argv);

/* split_quote_7 */
int		pipe_case2(t_pars *pars, t_splitted *tmp);

/* split_quote_6 */
int		handle_no_next(t_pars *pars, int delimiter, t_ht *env);
void	concat_pretext(char *str, t_split_sh *sp);

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
int		handle_meta(char const *str, t_split_sh *sp, t_list **elem, t_ht *env);

/* parser/environement */
char	**ht_to_envp(t_ht *env);
void	clear_envp(char **env);

/* utils */
int		ft_strarr_len(char **input);

/* split_free */
void	free_splitted(void *splitted);
void	free_splitted_wc(void *splitted);
void	free_splitted_c(void *v);

/* exec/exec_utils */
int		check_file(char *cmd_path, char *file);
int		exit_with_clear(t_list **splitted, t_ht *env, t_flags *flags,
			long long last_status);

/* exec/cmd_path */
char	*get_path(char **envp);
char	*get_cmd_path(char *cmd, char *path);

/* exec/complex/cmd_flags */
t_flags	*set_flags(t_list *splitted);

/* exec/complex/cmd_flags_set_files */
int		set_files(int infile_count, int outfile_count, t_flags **flags,
			t_list **splitted);

/* exec/complex/cmd_flags_set_files_while */
int		set_files_while(t_flags **flags, t_list **splitted,
			int i_infile, int i_outfile);

/* exec/complex/cmd_flags_set_pipes */
int		set_pipes(t_flags **flags, t_list *splitted);

/* exec/complex/cmd_flags_free */
void	free_flags_pipe(t_flags *flags, int pipe_i);
void	free_flags_files(t_flags *flags, int i_infile, int i_outfile);
void	free_flags(t_flags *flags);

/* exec/complex/pipe */
int		open_pipe(t_flags *flags);
void	close_pipe(t_flags *flags);

/* exec/complex/exec_pipe */
void	child_exec(t_flags *flags, int i, t_ht *env);

/* exec/complex/exec_pipe_utils */
int		select_exec_pipe(int argc, char **argv, t_flags *flags, char **envp);
int		open_exit_pipe(t_flags *flags);

/* exec/complex/fork */
int		forking(t_flags *flags, t_list *splitted, t_ht *env);

/* exec/complex/edit_flags */
bool	edit_flags(t_flags **flags);

/* exec/complex/check_files */
int		check_exec(t_flags **flags);

/* exec/complex/open_files */
int		open_infile(int index, int index_file, t_flags *flags);
int		open_outfile(int index, int index_file, t_flags *flags);
int		open_heredoc(int index, int index_file, t_flags *flags);

/* exec/complex/exec_utils */
void	exec_error(char *error, char *cmd);
void	free_child(t_ht *env, t_list *splitted, t_flags *flags, char **envp);
void	exec_error_free(char *error, char *cmd, t_flags *flags);
void	exec_error_arg(char *error, char *arg, char *cmd);
void	file_error(char *error, char *file);

extern int	g_sig;

#endif