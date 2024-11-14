/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/14 15:07:36 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosopher_routine(void *philosopher_data)
{
	t_philosopher	*philosopher;
	
	philosopher = (t_philosopher *)philosopher_data;
	while (1)
	{
		pthread_mutex_lock(&philosopher->control->group_mutex);
		if (philosopher->control->active_group != (philosopher->id % 2))
		{
			pthread_mutex_unlock(&philosopher->control->group_mutex);
			sleep_ms(500);//Value needs to be adjusted
			continue;
		}
		pthread_mutex_unlock(&philosopher->control->group_mutex);
		pthread_mutex_lock(philosopher->left_fork);
		pthread_mutex_lock(philosopher->right_fork);
		philosopher->last_meal_time = get_current_time_ms();
		printf("Philosopher %d is eating\n", philosopher->id);
		sleep_ms(philosopher->control->time_to_eat);//Value needs to be adjusted
		pthread_mutex_unlock(philosopher->left_fork);
		pthread_mutex_unlock(philosopher->right_fork);
		printf("Philosopher %d is sleeping\n", philosopher->id);
		sleep_ms(philosopher->control->time_to_sleep);//Value needs to be adjusted
		printf("Philosopher %d is thinking\n", philosopher->id);
		sleep_ms(500);//Value needs to be adjusted
	}
	return (NULL);
}
int	start_philosophers(t_simulation *simulation)
{
	pthread_t	*threads;
	int			i;

	threads = malloc(sizeof(pthread_t) * simulation->control->number_of_philosophers);
	if (!threads)
		return (-1);
	i = 0;
	while (i < simulation->control->number_of_philosophers)
	{
		if (pthread_create(&threads[i], NULL, philosopher_routine, &simulation->philosophers[i] != 0))
		{
			free(threads);
			return (-1);
		}
		i++;
	}
	i = 0;
	while (i < simulation->control->number_of_philosophers)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free(threads);
	return (0);
}