/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:08:49 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/05 10:15:09 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int main(int argc, char **argv)
{
    t_shared shared;
    t_thread_data data;
    int cleanup_flags;

    cleanup_flags = 0;
    if (!initialize_simulation(&shared, argc, argv, &cleanup_flags))
    {
        clean_up_simulation(NULL, &shared, cleanup_flags);
        return (1);
    }
    if (!allocate_thread_data(&data, &shared, &cleanup_flags))
    {
        printf("Error: Failed to allocate thread data.\n");
        clean_up_simulation(NULL, &shared, cleanup_flags);
        return (1);
    }
    if (!init_forks(&shared, &cleanup_flags))
    {
        clean_up_simulation(&data, &shared, cleanup_flags);
        return (1);
    }
    if (!launch_threads(&data, &cleanup_flags))
    {
        printf("Error: Failed to launch threads.\n");
        clean_up_simulation(&data, &shared, cleanup_flags);
        return (1);
    }
    clean_up_simulation(&data, &shared, cleanup_flags);
    return (0);
}

