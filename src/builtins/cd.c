/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/24 18:40:41 by JoWander         ###   ########.fr       */
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

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;

	if (!args[1])
	{
		path = get_home_dir(shell);
		if (!path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (1);
		}
	}
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (1);
	}
	update_pwd_vars(shell);
	return (0);
}
