/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:36:47 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/04 11:36:32 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int allocate_memory(t_thread_data *data, t_shared *shared, int *cleanup_flags)
{
    data->threads = malloc(sizeof(pthread_t) * shared->num_philosophers);
    if (!data->threads)
        return (0);
    data->philosophers = malloc(sizeof(t_philosopher) * shared->num_philosophers);
    if (!data->philosophers)
    {
        *cleanup_flags |= MEMORY_ALLOCATED;
        clean_up_simulation(data, shared, *cleanup_flags);
        return (0);
    }
    *cleanup_flags |= MEMORY_ALLOCATED;
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
    if (pthread_mutex_init(&philosopher->meal_lock, NULL) != 0)
        set_simulation_end(shared, 1);
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
    if (pthread_mutex_init(&shared->simulation_end_lock, NULL) != 0)
        return (0);
    shared->forks = malloc(sizeof(pthread_mutex_t) * shared->num_philosophers);
    if (!shared->forks)
    {
        pthread_mutex_destroy(&shared->simulation_end_lock);
        return (0);
    }
    if (!init_log_mutex(shared))
    {
        free(shared->forks);
        pthread_mutex_destroy(&shared->simulation_end_lock);
        return (0);
    }
    return (1);
}

int init_forks(t_shared *shared, int *cleanup_flags)
{
    int i;
    
    i = 0;
    while (i < shared->num_philosophers)
    {
        if (pthread_mutex_init(&shared->forks[i], NULL) != 0)
        {
            *cleanup_flags |= MUTEXES_INITIALIZED;
            clean_up_simulation(NULL, shared, *cleanup_flags);
            return (0);
        }
        i++;
    }
    *cleanup_flags |= MUTEXES_INITIALIZED;
    return (1);
}

int allocate_thread_data(t_thread_data *data, t_shared *shared, int *cleanup_flags)
{
    int i;

    if (!allocate_memory(data, shared, cleanup_flags))
        return (0);
    *cleanup_flags |= MEMORY_ALLOCATED;
    i = 0;
    while (i < shared->num_philosophers)
    {
        initialize_philosopher(&data->philosophers[i], i + 1, shared);
        i++;
    }
    data->shared = shared;
    return (1);
}

int initialize_simulation(t_shared *shared, int argc, char **argv, int *cleanup_flags)
{
    if (!parse_arguments(argc, argv, shared))
    {
        printf("Error: Invalid arguments.\n");
        return (0);
    }
    if (!init_shared_data(shared, argv))
    {
        *cleanup_flags |= MEMORY_ALLOCATED;
        clean_up_simulation(NULL, shared, *cleanup_flags);
        return (0);
    }
    *cleanup_flags |= MEMORY_ALLOCATED;
    if (!init_forks(shared, cleanup_flags))
    {
        clean_up_simulation(NULL, shared, *cleanup_flags);
        return (0);
    }
    *cleanup_flags |= MUTEXES_INITIALIZED;
    if (shared->meals_required == 0)
    {
        clean_up_simulation(NULL, shared, *cleanup_flags);
        return (0);
    }
    if (shared->num_philosophers == 1)
    {
        printf("1 has taken a fork\n");
        usleep(shared->time_to_die * 1000);
        printf("%lld 1 died\n", get_current_time_ms());
        clean_up_simulation(NULL, shared, *cleanup_flags);
        return (0);
    }
    return (1);
}
