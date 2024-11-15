/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/15 22:48:02 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	perform_eating(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->left_fork);
	printf("%ld Philosopher %d has taken a fork\n", get_current_time_ms(), philosopher->id);
	pthread_mutex_lock(philosopher->right_fork);
	printf("%ld Philosopher %d has taken a fork\n", get_current_time_ms(), philosopher->id);
	philosopher->last_meal_time = get_current_time_ms();
	printf("%ld Philosopher %d is eating\n", philosopher->last_meal_time, philosopher->id);
	sleep_ms(philosopher->control->time_to_eat);
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
}

void	*philosopher_routine(void *philosopher_data)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)philosopher_data;
	while (1)
	{
		if (should_stop_simulation(philosopher))
			return (NULL);

		handle_eating(philosopher);
		handle_sleeping(philosopher);
		handle_thinking(philosopher);
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
		if (pthread_create(&threads[i], NULL, philosopher_routine, &simulation->philosophers[i]) != 0)
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
