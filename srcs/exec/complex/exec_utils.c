/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:15:38 by Sifex             #+#    #+#             */
/*   Updated: 2025/01/21 16:34:04 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_error(char *error, char *cmd)
{
	ft_putstr_fd("bash: ", 2);
	if (cmd)
		ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
}

void	free_child(t_ht *env, t_list *splitted, t_flags *flags, char **envp)
{
	if (splitted)
		ft_lstclear(&splitted, &free_splitted_wc);
	if (envp)
		clear_env(envp);
	if (env)
		hashtable_destroy(env);
	if (flags)
		free_flags(flags);
}

void	exec_error_free(char *error, char *cmd, t_flags *flags)
{
	ft_putstr_fd("bash: ", 2);
	if (cmd)
		ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
	free_child(flags->env, flags->splitted, flags, flags->envp);
}

void	exec_error_arg(char *error, char *arg, char *cmd)
{
	ft_putstr_fd("bash: ", 2);
	if (!cmd)
		ft_putstr_fd(" ", 2);
	else
		ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (!arg)
		ft_putstr_fd(" ", 2);
	else
		ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
}

void	file_error(char *error, char *file)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
}
