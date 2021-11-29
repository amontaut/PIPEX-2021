/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_stuff.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 20:41:29 by amontaut          #+#    #+#             */
/*   Updated: 2021/11/24 20:41:39 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	relative_path(char *cmd, char **mycmdargs, char **envp)
{
	int		err;
	char	*finalcmd;

	finalcmd = ft_strjoin("./", cmd);
	err = execve(finalcmd, mycmdargs, envp);
	free(finalcmd);
	return (err);
}

int	path_from_shell(char *shell_path, char **mycmdargs, char **envp)
{
	char	*finalcmd;
	int		i;
	char	*precmd1;
	char	**mypaths;
	int		err;

	mypaths = ft_split(shell_path, ':');
	i = 0;
	while (mypaths[i])
	{
		precmd1 = ft_strjoin(mypaths[i], "/");
		finalcmd = ft_strjoin(precmd1, mycmdargs[0]);
		err = execve(finalcmd, mycmdargs, envp);
		free(mypaths[i]);
		i++;
		free(precmd1);
		free(finalcmd);
	}
	free(mypaths);
	return (err);
}

int	child(char *cmd, char **envp, char *shell_path)
{
	char	**mycmdargs;
	int		err;
	int		len;
	int		i;

	len = ft_strlen(cmd) - 1;
	mycmdargs = ft_split(cmd, ' ');
	if (cmd[0] == '/')
		err = execve(mycmdargs[0], mycmdargs, envp);
	else if (ft_strchr(mycmdargs[0], '/') != NULL && mycmdargs[0][len] != '/')
		err = relative_path(mycmdargs[0], mycmdargs, envp);
	else
		err = path_from_shell(shell_path, mycmdargs, envp);
	i = -1;
	while (mycmdargs[++i])
		free(mycmdargs[i]);
	if (err == -1)
	{
		free(mycmdargs);
		if (errno != 2)
			ft_error(cmd, shell_path);
		else
			ft_error(cmd, shell_path);
	}
	exit(EXIT_FAILURE);
}

void	child_one(char **av, char **envp, char *shell_path, int *t)
{
	int	f1;

	f1 = open(av[1], O_RDONLY);
	if (f1 < 0)
		ft_error(av[1], shell_path);
	dup2(f1, STDIN_FILENO);
	close(f1);
	dup2(t[1], STDOUT_FILENO);
	close(t[1]);
	close(t[0]);
	child(av[2], envp, shell_path);
	close(f1);
}

void	child_two(char **av, char **envp, char *shell_path, int *t)
{
	int	f2;

	f2 = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (f2 < 0)
		ft_error(av[4], shell_path);
	dup2(t[0], STDIN_FILENO);
	close(t[0]);
	close(t[1]);
	dup2(f2, STDOUT_FILENO);
	close(f2);
	child(av[3], envp, shell_path);
	close(f2);
}
