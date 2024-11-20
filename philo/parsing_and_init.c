/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_and_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:09:17 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/20 15:53:25 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int parse_input(int argc, char **argv, t_shared *shared)
{
    if (argc < 5 || argc > 6)
        return (1);
    shared->num_philosophers = atoi(argv[1]);
    shared->time_to_die = atoi(argv[2]);
    shared->time_to_eat = atoi(argv[3]);
    shared->time_to_sleep = atoi(argv[4]);
    shared->meals_required = (argc == 6) ? atoi(argv[5]) : -1; // Optional argument
    if (shared->num_philosophers <= 0 || shared->time_to_die <= 0 || 
        shared->time_to_eat <= 0 || shared->time_to_sleep <= 0)
        return (1);
    shared->is_simulation_running = 1;
    return (0);
}

int init_shared_resources(t_shared *shared)
{
    int i;

    shared->forks = malloc(sizeof(pthread_mutex_t) * shared->num_philosophers);
    if (!shared->forks)
        return (1);
    i = 0;
    while (i < shared->num_philosophers)
    {
        if (pthread_mutex_init(&shared->forks[i], NULL) != 0)
        {
            while (--i >= 0)
                pthread_mutex_destroy(&shared->forks[i]);
            free(shared->forks);
            return (1);
        }
        i++;
    }
    if (pthread_mutex_init(&shared->write_lock, NULL) != 0 ||
        pthread_mutex_init(&shared->monitor_lock, NULL) != 0)
    {
        free(shared->forks);
        return (1);
    }
    return (0);
}

t_philosopher *init_philosophers(t_shared *shared)
{
    t_philosopher   *philosophers;
    int             i;

    philosophers = malloc(sizeof(t_philosopher) * shared->num_philosophers);
    if (!philosophers)
        return (NULL);
    i = 0;
    while (i < shared->num_philosophers)
    {
        philosophers[i].id = i + 1;
        philosophers[i].meals_eaten = 0;
        philosophers[i].last_meal_time = 0;
        philosophers[i].left_fork = &shared->forks[i];
        philosophers[i].right_fork = &shared->forks[(i + 1) % shared->num_philosophers];
        philosophers[i].shared = shared;
        i++;
    }
    return (philosophers);
}

int initialize_simulation(int argc, char **argv, t_shared *shared)
{
    if (parse_input(argc, argv, shared))
    {
        printf("Error: Invalid input\n");
        return (1);
    }
    if (init_shared_resources(shared))
    {
        printf("Error: Resource initialization failed\n");
        return (1);
    }
    return (0);
}