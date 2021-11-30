/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_stuff.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 20:41:29 by amontaut          #+#    #+#             */
/*   Updated: 2021/11/30 19:29:44 by amontaut         ###   ########.fr       */
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

/*
** In the varaible PATH, there are several possibilities (seperated by : in 
** the var PATH) than could work for our command. We try them all with a loop.
** We need to add a / at the end so that it works (join). 
*/
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

/*
** We execute our command (for example ls -la) with the fct execve. 
** First arg = first part of the command we put in arg when we launched 
** pipex (so ls) OR the var PATH; 2nd arg = char ** with all parts of 
** the command (so ls -la) and last arg = env varibles. 
** if the command starts with / we launch it as it is, if there is a ./
** we launch it as an executable (relative path), otherwise we launch 
** it normally with the var PATH. 
*/
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

/*
** We open the file of the command line, change the stdin from the
** standard entry to f1 and change the stdout from the standard exit to
** the pipe t[1]
*/
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

/*
** We open the file of the command line, change the stdin from the
** standard entry to the pipe f[0] and change the stdout from the 
** standard exit to the pipe t[1]
*/
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
