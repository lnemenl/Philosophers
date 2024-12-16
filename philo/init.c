/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:36:47 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/16 18:35:23 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philosophers.h"

void	initialize_philosopher(t_philosopher *philosopher,
								int id, t_shared *shared)
{
	philosopher->id = id;
	philosopher->meals_eaten = 0;
	philosopher->left_fork = &shared->forks[id - 1];
	philosopher->right_fork = &shared->forks[id % shared->num_philosophers];
	philosopher->shared_data = shared;
	if (pthread_mutex_init(&philosopher->meal_lock, NULL) != 0)
		set_simulation_end(shared, 1);
}

int	init_forks(t_shared *shared, int *cleanup_flags)
{
	int	i;

	i = 0;
	while (i < shared->num_philosophers)
	{
		if (pthread_mutex_init(&shared->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&shared->forks[i]);
			*cleanup_flags |= MUTEXES_INITIALIZED;
			clean_up_simulation(NULL, shared, *cleanup_flags);
			return (0);
		}
		i++;
	}
	*cleanup_flags |= MUTEXES_INITIALIZED;
	return (1);
}

static int	initialize_mutexes(t_shared *shared, int *cleanup_flags)
{
	if (pthread_mutex_init(&shared->simulation_end_lock, NULL) != 0)
		return (0);
	shared->forks = malloc(sizeof(pthread_mutex_t) * shared->num_philosophers);
	if (!shared->forks)
	{
		pthread_mutex_destroy(&shared->simulation_end_lock);
		return (0);
	}
	if (pthread_mutex_init(&shared->log_lock, NULL) != 0)
	{
		free(shared->forks);
		pthread_mutex_destroy(&shared->simulation_end_lock);
		return (0);
	}
	*cleanup_flags |= MEMORY_ALLOCATED;
	return (1);
}

static int	handle_single_philosopher(t_shared *shared, int *cleanup_flags)
{
	t_philosopher	single_philo;

	single_philo.id = 1;
	single_philo.shared_data = shared;
	log_action(&single_philo, "has taken fork");
	usleep(shared->time_to_die * 1000);
	log_action(&single_philo, "died");
	clean_up_simulation(NULL, shared, *cleanup_flags);
	return (0);
}

int	initialize_simulation(t_shared *shared,
							int argc, char **argv, int *cleanup_flags)
{
	if (!parse_and_validate(argc, argv, shared))
	{
		log_error("Error: Invalid arguments");
		return (0);
	}
	shared->start_time = get_current_time_ms();
	shared->simulation_end = 0;
	if (!initialize_mutexes(shared, cleanup_flags))
		return (0);
	if (shared->num_philosophers == 1)
		return (handle_single_philosopher(shared, cleanup_flags));
	return (1);
}
