/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:36:47 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/29 01:33:32 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int allocate_thread_data(t_thread_data *data, t_shared *shared)
{
    data->threads = malloc(sizeof(pthread_t) * shared->num_philosophers);
    if (!data->threads)
        return (0);
    data->philosophers = malloc(sizeof(t_philosopher) * shared->num_philosophers);
    if (!data->philosophers)
    {
        free(data->threads);
        return (0);
    }
    data->shared = shared;
    return (1);
}

int init_log_mutex(t_shared *shared)
{
    if (pthread_mutex_init(&shared->log_lock, NULL) != 0)
        return (0);
    return (1);
}

int init_shared_data(t_shared *shared, char **argv)
{
    if (!parse_shared_parameters(shared, argv))
        return (0);
    shared->simulation_end = 0;
    if (!init_log_mutex(shared))
        return (0);
    return (1);
}

int init_forks(t_shared *shared)
{
    int i;

    i = 0;
    while (i < shared->num_philosophers)
    {
        if (pthread_mutex_init(&shared->forks[i], NULL) != 0)
        {
            while (--i >= 0)
                pthread_mutex_destroy(&shared->forks[i]);
            return (0);
        }
        i++;
    }
    return (1);
}

int initialize_simulation(t_shared *shared, int argc, char **argv)
{
    if (!parse_arguments(argc, argv, shared))
    {
        printf("Error: Invalid arguments.\n");
        return (0);
    }
    if (!init_shared_data(shared, argv))
    {
        printf("Error: Failed to initialize shared data.\n");
        return (0);
    }
    if (!init_forks(shared))
    {
        pthread_mutex_destroy(&shared->log_lock);
        printf("Error: Failed to initialize forks.\n");
        return (0);
    }
    return (1);
}
