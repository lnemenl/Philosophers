/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:43:43 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/19 14:16:36 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	allocate_thread_data(t_thread_data *data,
							t_shared *shared, int *cleanup_flags)
{
	int	i;

	data->threads = malloc(sizeof(pthread_t) * shared->num_philosophers);
	if (!data->threads)
		return (0);
	data->philosophers = malloc(sizeof(t_philosopher)
			* shared->num_philosophers);
	if (!data->philosophers)
	{
		free(data->threads);
		return (0);
	}
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

static int	create_philosopher_threads(t_thread_data *data, int *cleanup_flags)
{
	int	i;

	i = 0;
	while (i < data->shared->num_philosophers)
	{
		if (pthread_create(&data->threads[i], NULL, philosopher_routine,
				(void *)&data->philosophers[i]) != 0)
		{
			log_error("Error: Failed to create philosopher thread");
			while (i > 0)
			{
				i--;
				pthread_join(data->threads[i], NULL);
			}
			return (0);
		}
		i++;
	}
	*cleanup_flags |= THREADS_INITIALIZED;
	return (1);
}

static int	create_monitor_thread(t_thread_data *data)
{
	int	i;

	if (pthread_create(&data->monitor_thread,
			NULL, monitor_routine, (void *)data) != 0)
	{
		log_error("Error: Failed to create monitor thread.");
		i = 0;
		while (i < data->shared->num_philosophers)
		{
			pthread_join(data->threads[i], NULL);
			i++;
		}
		return (0);
	}
	return (1);
}

int	launch_threads(t_thread_data *data, int *cleanup_flags)
{
	if (!create_philosopher_threads(data, cleanup_flags))
	{
		clean_up_simulation(data, data->shared, *cleanup_flags);
		return (0);
	}
	if (!create_monitor_thread(data))
	{
		clean_up_simulation(data, data->shared, *cleanup_flags);
		return (0);
	}
	return (1);
}
