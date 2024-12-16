/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/16 18:35:37 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	join_threads(t_thread_data *data, t_shared *shared)
{
	int	i;

	i = 0;
	while (i < shared->num_philosophers)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	pthread_join(data->monitor_thread, NULL);
}

static void	destroy_mutexes(t_thread_data *data, t_shared *shared)
{
	int	i;

	i = 0;
	while (i < shared->num_philosophers)
	{
		pthread_mutex_destroy(&shared->forks[i]);
		if (data && data->philosophers)
			pthread_mutex_destroy(&data->philosophers[i].meal_lock);
		i++;
	}
	pthread_mutex_destroy(&shared->log_lock);
	pthread_mutex_destroy(&shared->simulation_end_lock);
}

static void	free_memory(t_thread_data *data, t_shared *shared)
{
	if (data)
	{
		if (data->threads)
			free(data->threads);
		if (data->philosophers)
			free(data->philosophers);
	}
	if (shared && shared->forks)
	{
		free(shared->forks);
		shared->forks = NULL;
	}
}

void	clean_up_simulation(t_thread_data *data,
							t_shared *shared, int cleanup_flags)
{
	if ((cleanup_flags & THREADS_INITIALIZED) && data && data->threads)
		join_threads(data, shared);
	if ((cleanup_flags & MUTEXES_INITIALIZED) && shared && shared->forks)
		destroy_mutexes(data, shared);
	if (cleanup_flags & MEMORY_ALLOCATED)
		free_memory(data, shared);
}
