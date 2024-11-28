/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:36:47 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 21:38:19 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int init_shared_data(t_shared *shared, char **argv)
{
    shared->num_philosophers = safe_atoi(argv[1]);
    shared->time_to_die = safe_atoi(argv[2]);
    shared->time_to_eat = safe_atoi(argv[3]);
    shared->time_to_sleep = safe_atoi(argv[4]);
    if (argv[5])
        shared->meals_required = safe_atoi(argv[5]);
    else
        shared->meals_required = -1;
    shared->simulation_end = 0;
    shared->forks = malloc(sizeof(pthread_mutex_t) * shared->num_philosophers);
    if (!shared->forks)
        return (0);
    if (pthread_mutex_init(&shared->log_lock, NULL) != 0)
    {
        free(shared->forks);
        return (0);
    }
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
        print_error("Error: Invalid arguments.\n");
        return (0);
    }
    if (!init_shared_data(shared, argv))
    {
        print_error("Error: Failed to initialize shared data.\n");
        return (0);
    }
    if (!init_forks(shared))
    {
        free(shared->forks);
        pthread_mutex_destroy(&shared->log_lock);
        print_error("Error: Failed to initialize forks.\n");
        return (0);
    }
    return (1);
}
