/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:33:19 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/31 15:40:21 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	print_env_vars(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putendl_fd(env[i], 1);
		i++;
	}
}

void	print_sorted_env(char **env)
{
	char	**sorted;

	sorted = dup_env_array(env);
	if (!sorted)
		return ;
	sort_env_array(sorted);
	print_env_vars(sorted);
	free_array(sorted);
}

void	sort_env_array(char **env)
{
	int	i;
	int	j;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strncmp(env[i], env[j], ft_strlen(env[i])) > 0)
				swap_env_vars(&env[i], &env[j]);
			j++;
		}
		i++;
	}
}
