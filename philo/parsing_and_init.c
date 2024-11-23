/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_and_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:09:17 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/23 21:15:46 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	parse_input(int argc, char **argv, t_shared *shared)
{
	if (argc != 5)
	{
		printf("Error: Invalid number of arguments. Usage: ./philosophers num_philosophers time_to_die time_to_eat time_to_sleep\n");
		return (1);
	}
	shared->num_philosophers = ft_atoi(argv[1]);
	shared->time_to_die = ft_atoi(argv[2]);
	shared->time_to_eat = ft_atoi(argv[3]);
	shared->time_to_sleep = ft_atoi(argv[4]);
	if (shared->num_philosophers <= 0 || shared->time_to_die <= 0 || 
		shared->time_to_eat <= 0 || shared->time_to_sleep <= 0)
	{
		printf("Error: All timing values must be positive integers in ms.\n");
		return (1);
	}
	shared->is_simulation_running = 1;
	return (0);
}

int	initialize_forks(t_shared *shared, int i)
{
	if (i >= shared->num_philosophers)
		return (0);
	shared->forks[i] = 0;
	return (initialize_forks(shared, i + 1));
}

int initialize_shared_resources(t_shared *shared)
{
	int	i;

    shared->forks = malloc(sizeof(int) * shared->num_philosophers);
    if (!shared->forks)
        return (1);
    i = 0;
    while (i < shared->num_philosophers)
	{
        shared->forks[i] = 0;
        i++;
    }
    if (pthread_mutex_init(&shared->forks_mutex, NULL) != 0)
        return (1);
    if (pthread_mutex_init(&shared->write_mutex, NULL) != 0)
	{
        pthread_mutex_destroy(&shared->forks_mutex);
        return (1);
    }
    return (0);
}


int	setup_philosopher(t_philosopher *philosophers, t_shared *shared, int i)
{
	if (i >= shared->num_philosophers)
		return (0);
	philosophers[i].id = i + 1;
	philosophers[i].last_meal_time = get_current_time();
	philosophers[i].shared = shared;
	return (setup_philosopher(philosophers, shared, i + 1));
}

t_philosopher	*init_philosophers(t_shared *shared)
{
	t_philosopher *philosophers = malloc(sizeof(t_philosopher) * shared->num_philosophers);
	if (!philosophers)
		return (NULL);
	if (setup_philosopher(philosophers, shared, 0) != 0)
	{
		free(philosophers);
		return (NULL);
	}
	return (philosophers);
}

int	initialize_simulation(int argc, char **argv, t_shared *shared)
{
	if (parse_input(argc, argv, shared) != 0)
		return (1);
	if (initialize_shared_resources(shared) != 0)
	{
		printf("Error: Failed to initialize shared resources.\n");
		return (1);
	}
	return (0);
}
