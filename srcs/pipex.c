/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 20:29:43 by amontaut          #+#    #+#             */
/*   Updated: 2021/11/24 20:33:58 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_error(char *str, char *shell_path)
{
	free(shell_path);
	write(STDERR_FILENO, "bash: ", 6);
	write(STDERR_FILENO, str, ft_strlen(str));
	perror(" ");
	exit(EXIT_FAILURE);
}

char	*count_malloc(char **envp, int *len, char *shell_path)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (ft_strstr(envp[i], "PATH") != NULL)
	{
		while (ft_strstr(envp[i], "PATH")[j] != '/')
			j++;
		(*len) = ft_strlen(ft_strstr(envp[i], "PATH")) - j;
		shell_path = malloc((*len) * (sizeof(char *)) + 1);
		if (shell_path == NULL)
			exit(EXIT_FAILURE);
	}
	return (shell_path);
}

char	*assign_malloc(char **envp, int len, char *shell_path)
{
	int	i;
	int	k;
	int	j;

	i = 0;
	j = 0;
	k = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (ft_strstr(envp[i], "PATH") != NULL)
	{
		while (ft_strstr(envp[i], "PATH")[j] != '/')
			j++;
		len = len + 1;
		while (j <= len)
		{
			shell_path[k] = ft_strstr(envp[i], "PATH")[j];
			k++;
			j++;
		}
		shell_path[k] = '\0';
	}
	return (shell_path);
}

void	launch_child(int *t, char **av, char **envp, char *shell_path)
{
	pid_t	child1;
	pid_t	child2;
	int		status;

	child2 = 0;
	child1 = fork();
	if (child1 == 0)
		child_one(av, envp, shell_path, t);
	else
	{
		child2 = fork();
		if (child2 == 0)
			child_two(av, envp, shell_path, t);
	}
	if (child1 < 0)
		ft_error("Fork error ", shell_path);
	if (child2 < 0)
		ft_error("Fork error ", shell_path);
	close(t[0]);
	close(t[1]);
	waitpid(child1, &status, 0);
	waitpid(child2, &status, 0);
}

int	main(int ac, char **av, char **envp)
{
	int		t[2];
	char	*shell_path;
	int		len;

	if (ac != 5)
	{
		write(STDERR_FILENO, "Error: wrong number of arg\n", 28);
		exit(EXIT_FAILURE);
	}
	len = 0;
	shell_path = count_malloc(envp, &len, NULL);
	shell_path = assign_malloc(envp, len, shell_path);
	pipe(t);
	launch_child(t, av, envp, shell_path);
	free(shell_path);
	return (0);
}
