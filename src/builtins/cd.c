/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/29 17:10:54 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <unistd.h>

char	*get_home_dir(t_shell *shell)
{
	return (env_get_value("HOME", shell->env));
}

static void	update_pwd_vars(t_shell *shell)
{
	char	*old_pwd;
	char	cwd[4096];

	old_pwd = env_get_value("PWD", shell->env);
	if (old_pwd)
		env_set(shell, "OLDPWD", old_pwd);
	if (getcwd(cwd, sizeof(cwd)))
		env_set(shell, "PWD", cwd);
}

int	print_cd_error(char *message, char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (path)
		ft_putstr_fd(path, 2);
	ft_putendl_fd(message, 2);
	return (1);
}

char	*get_cd_path(char **args, t_shell *shell)
{
	char	*oldpwd;
	char	*home;
	char	*path;

	if (!args[1])
		return (get_home_dir(shell));
	if (args[2])
		return (NULL);
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		oldpwd = env_get_value("OLDPWD", shell->env);
		if (!oldpwd)
			return (NULL);
		ft_putendl_fd(oldpwd, 1);
		return (oldpwd);
	}
	if (args[1][0] == '~')
	{
		home = get_home_dir(shell);
		if (!home)
			return (NULL);
		if (args[1][1] == '\0')
			return (home);
		if (args[1][1] == '/')
		{
			path = ft_strjoin(home, args[1] + 1);
			if (!path)
				return (NULL);
			return (path);
		}
	}
	return (args[1]);
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;

	path = get_cd_path(args, shell);
	if (!path)
	{
		if (args[2])
			return (print_cd_error("too many arguments", NULL));
		if (!args[1])
			return (print_cd_error("HOME not set", NULL));
		if (ft_strncmp(args[1], "-", 2) == 0)
			return (print_cd_error("OLDPWD not set", NULL));
		return (print_cd_error(": No such file or directory", args[1]));
	}
	if (chdir(path) == -1)
		return (print_cd_error(": No such file or directory", path));
	update_pwd_vars(shell);
	return (0);
}
