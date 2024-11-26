/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:36:47 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/26 19:49:31 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

pthread_mutex_t *initialize_fork_mutexes(int num_philosophers)
{
    pthread_mutex_t *forks;
    int i;

    forks = malloc(sizeof(pthread_mutex_t) * num_philosophers);
    if (!forks)
    {
        printf("Error: Memory allocation for forks failed.\n");
        return (NULL);
    }
    i = 0;
    while (i < num_philosophers)
    {
        if (pthread_mutex_init(&forks[i], NULL) != 0)
        {
            printf("Error: Failed to initialize fork mutex %d.\n", i);
            while (--i >= 0)
                pthread_mutex_destroy(&forks[i]);
            free(forks);
            return (NULL);
        }
        i++;
    }
    return (forks);
}

int initialize_write_mutex(pthread_mutex_t *write_mutex)
{
    if (pthread_mutex_init(write_mutex, NULL) != 0)
    {
        printf("Error: Failed to initialize write mutex.\n");
        return (1);
    }
    return (0);
}

int initialize_shared_resources(t_shared *shared)
{
    shared->forks_mutex = initialize_fork_mutexes(shared->num_philosophers);
    if (!shared->forks_mutex)
        return (1);
    if (initialize_write_mutex(&shared->write_mutex))
    {
        destroy_fork_mutexes(shared->forks_mutex, shared->num_philosophers);
        return (1);
    }
    return (0);
}

t_philosopher *initialize_philosophers(t_shared *shared)
{
    t_philosopher *philosophers;
    int i;

    philosophers = malloc(sizeof(t_philosopher) * shared->num_philosophers);
    if (!philosophers)
        return NULL;
    i = 0;
    while (i < shared->num_philosophers)
    {
        philosophers[i].id = i + 1;
        philosophers[i].last_meal_time = 0;
        philosophers[i].meals_eaten = 0;
        philosophers[i].shared = shared;
        if (pthread_mutex_init(&philosophers[i].meal_mutex, NULL) != 0)
        {
            while (--i >= 0)
                pthread_mutex_destroy(&philosophers[i].meal_mutex);
            free(philosophers);
            return NULL;
        }
        i++;
    }
    return philosophers;
}

t_philosopher *setup_simulation(t_shared *shared)
{
    t_philosopher *philosophers;

    if (initialize_shared_resources(shared))
        return (NULL);
    philosophers = initialize_philosophers(shared);
    if (!philosophers)
    {
        cleanup_shared_resources(shared);
        return (NULL);
    }
    return (philosophers);
}
