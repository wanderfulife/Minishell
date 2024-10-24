/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/24 18:40:34 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0] || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	**dup_env_array(char **env)
{
	char	**new;
	int		i;
	int		size;

	size = 0;
	while (env[size])
		size++;
	new = malloc(sizeof(char *) * (size + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new[i] = ft_strdup(env[i]);
		if (!new[i])
		{
			while (--i >= 0)
				free(new[i]);
			free(new);
			return (NULL);
		}
		i++;
	}
	new[i] = NULL;
	return (new);
}

static void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static void	print_sorted_env(char **env)
{
	char	**sorted;
	char	*temp;
	int		i;
	int		j;

	sorted = dup_env_array(env);
	if (!sorted)
		return ;
	i = 0;
	while (sorted[i])
	{
		j = i + 1;
		while (sorted[j])
		{
			if (ft_strncmp(sorted[i], sorted[j], ft_strlen(sorted[i])) > 0)
			{
				temp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = temp;
			}
			j++;
		}
		ft_putstr_fd("declare -x ", 1);
		ft_putendl_fd(sorted[i], 1);
		i++;
	}
	free_array(sorted);
}

int	builtin_export(char **args, t_shell *shell)
{
	int	i;
	int	status;

	status = 0;
	if (!args[1])
	{
		print_sorted_env(shell->env);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else
			env_set(shell, args[i], NULL);
		i++;
	}
	return (status);
}
