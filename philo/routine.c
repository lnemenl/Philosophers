/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/03 10:02:44 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int take_forks(t_philosopher *philosopher)
{
	t_shared *shared;
	shared = philosopher->shared_data;
	if (is_simulation_end(shared))
		return (0);
	pthread_mutex_lock(philosopher->left_fork);
	log_action(philosopher, "has taken a fork");
	if (is_simulation_end(shared))
	{
		pthread_mutex_unlock(philosopher->left_fork);
		return (0);
	}
	pthread_mutex_lock(philosopher->right_fork);
	log_action(philosopher, "has taken a fork");
	if (is_simulation_end(shared))
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
	if (is_simulation_end(shared))
		return (0);
	log_action(philosopher, "is eating");
	philosopher->last_meal_time = get_current_time_ms();
	philosopher->meals_eaten++;
	smart_sleep(shared->time_to_eat, shared);
	if (is_simulation_end(shared))
		return (0);
	return (1);
}

void *philosopher_routine(void *arg)
{
	t_philosopher	*philosopher;
	t_shared		*shared;

	philosopher = (t_philosopher *)arg;
	shared = philosopher->shared_data;
	while (!is_simulation_end(shared))
	{
		if (!is_simulation_end(shared))
			log_action(philosopher, "is thinking");
		if (is_simulation_end(shared) || !take_forks(philosopher))
			break;
		if (is_simulation_end(shared) || !eat(philosopher))
		{
			put_forks(philosopher);
			break;
		}
		put_forks(philosopher);
		if (!is_simulation_end(shared))
			log_action(philosopher, "is sleeping");
		smart_sleep(shared->time_to_sleep, shared);
	}
	return (NULL);
}
