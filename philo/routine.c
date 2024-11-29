/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/29 04:52:13 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int take_forks(t_philosopher *philosopher)
{
	t_shared *shared;
	shared = philosopher->shared_data;
	if (shared->simulation_end)
		return (0);
	pthread_mutex_lock(philosopher->left_fork);
	log_action(philosopher, "has taken a fork");
	if (shared->simulation_end)
	{
		pthread_mutex_unlock(philosopher->left_fork);
		return (0);
	}
	pthread_mutex_lock(philosopher->right_fork);
	log_action(philosopher, "has taken a fork");
	if (shared->simulation_end)
	{
		put_forks(philosopher);
		return (0);
	}
	return (1);
}

void put_forks(t_philosopher *philosopher)
{
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
}

int eat(t_philosopher *philosopher)
{
	t_shared *shared;

	shared = philosopher->shared_data;
	if (shared->simulation_end)
		return (0);
	log_action(philosopher, "is eating");
	philosopher->last_meal_time = get_current_time_ms();
	philosopher->meals_eaten++;
	smart_sleep(shared->time_to_eat, shared);
	if (shared->simulation_end)
		return (0);
	return (1);
}

void *philosopher_routine(void *arg)
{
	t_philosopher	*philosopher;
	t_shared		*shared;

	philosopher = (t_philosopher *)arg;
	shared = philosopher->shared_data;
	while (!shared->simulation_end)
	{
		if (!shared->simulation_end)
			log_action(philosopher, "is thinking");
		if (shared->simulation_end || !take_forks(philosopher))
			break;
		if (shared->simulation_end || !eat(philosopher))
		{
			put_forks(philosopher);
			break;
		}
		put_forks(philosopher);
		if (!shared->simulation_end)
			log_action(philosopher, "is sleeping");
		smart_sleep(shared->time_to_sleep, shared);
	}
	return (NULL);
}
