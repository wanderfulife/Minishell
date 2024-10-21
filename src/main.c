/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:53:23 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/20 19:40:56 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


static t_list *create_env_node(char *env_var)
{
    t_list *new_node;

    // Allocate memory for a new node
    new_node = (t_list *)malloc(sizeof(t_list));
    if (!new_node)
        return (NULL);  // If malloc fails, return NULL

    // Copy the environment variable string
    new_node->str = ft_strdup(env_var);
    if (!new_node->str)
    {
        free(new_node);  // If strdup fails, free the node and return NULL
        return (NULL);
    }

    // Initialize next and prev pointers to NULL
    new_node->next = NULL;
    new_node->prev = NULL;

    return (new_node);  // Return the newly created node
}

int make_env(t_data *data, char **env)
{
    int i;
    t_list *new_node;
    t_list *last_node;

    // Initialize the environment list to NULL
    data->env_list = NULL;
    i = 0;

    // Loop through all environment variables
    while (env[i])
    {
        // Create a new node for this environment variable
        new_node = create_env_node(env[i]);
        if (!new_node)
        {
            // If node creation failed, free all previously created nodes
            while (data->env_list)
            {
                last_node = data->env_list;
                data->env_list = data->env_list->next;
                free(last_node->str);
                free(last_node);
            }
            return (0);  // Return 0 to indicate failure
        }

        // If this is the first node, make it the head of the list
        if (!data->env_list)
            data->env_list = new_node;
        else
        {
            // Otherwise, find the last node in the list
            last_node = data->env_list;
            while (last_node->next)
                last_node = last_node->next;

            // Append the new node to the end of the list
            last_node->next = new_node;
            new_node->prev = last_node;
        }

        i++;  // Move to the next environment variable
    }

    return (1);  // Return 1 to indicate success
}


int main(int argc, char **argv, char **env)
{
    t_data  data;
    char    *input;

    (void)argc;
    (void)argv;

    if (!make_env(&data, env))
        return (1);
    
    signals();

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break;
        
        if (*input)
            add_history(input);
        
        // TODO: Implement parsing and execution
        // Use data.token_list, data.cmd_list, data.last_exit_code, 
        // data.pipe_fd, and data.in_single_quote as needed

        free(input);
    }

    // TODO: Clean up and exit
    // Don't forget to free data.env_list, data.token_list, and data.cmd_list

    return (0);
}