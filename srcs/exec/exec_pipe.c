/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:27:00 by anaudibe          #+#    #+#             */
/*   Updated: 2024/12/06 16:04:36 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_pipe(t_list *idk)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (idk)
	{
		printf("idk->content = %s\n", input->content);
		idk = idk->next;
	}
	return (res);
}

//void    run_program_pipe()
// Base code
// check if has "file < command"
// check if has "command > file"
// check if has "command | command"
// check if has combo of the above
// do the heredoc thing  
void    run_program_pipe(char *path, char **argv, char **envp)
{
    pid_t	pid[2];
    int		pipe_fd[2];
    int		status;
    int		i;

    i = 0;
    while (argv[i])
    {
        if (pipe(pipe_fd) < 0)
            exit(1);
        pid[i] = fork();
        if (pid[i] < 0)
            exit(1);
        if (pid[i] == 0)
        {
            if (i == 0)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], 1);
                close(pipe_fd[1]);
                execve(path, argv, envp);
            }
            else
            {
                close(pipe_fd[1]);
                dup2(pipe_fd[0], 0);
                close(pipe_fd[0]);
                execve(path, argv, envp);
            }
        }
        i++;
    }
}

