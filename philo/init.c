/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:36:47 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/29 03:14:19 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int allocate_memory(t_thread_data *data, t_shared *shared)
{
    data->threads = malloc(sizeof(pthread_t) * shared->num_philosophers);
    if (!data->threads)
        return (0);
    data->philosophers = malloc(sizeof(t_philosopher) * shared->num_philosophers);
    if (!data->philosophers)
    {
        cleanup_partial_thread_data(data);
        return (0);
    }
    return (1);
}

void initialize_philosopher(t_philosopher *philosopher, int id, t_shared *shared)
{
    philosopher->id = id;
    philosopher->meals_eaten = 0;
    philosopher->last_meal_time = get_current_time_ms();
    philosopher->left_fork = &shared->forks[id - 1];
    philosopher->right_fork = &shared->forks[id % shared->num_philosophers];
    philosopher->shared_data = shared;
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
    shared->forks = malloc(sizeof(pthread_mutex_t) * shared->num_philosophers);
    if (!shared->forks)
        return (0);
    if (!init_log_mutex(shared))
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

int allocate_thread_data(t_thread_data *data, t_shared *shared)
{
    int i;

    if (!allocate_memory(data, shared))
        return (0);
    i = 0;
    while (i < shared->num_philosophers)
    {
        initialize_philosopher(&data->philosophers[i], i + 1, shared);
        i++;
    }
    data->shared = shared;
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
        printf("Error: Failed to initialize forks.\n");
        destroy_forks(shared);
        pthread_mutex_destroy(&shared->log_lock);
        return (0);
    }
    if (shared->num_philosophers == 1)
    {
        printf("0 1 has taken a fork\n");
        printf("%lld 1 died\n", (long long)shared->time_to_die);
        destroy_forks(shared);
        pthread_mutex_destroy(&shared->log_lock);
        return (0);
    }
    return (1);
}

