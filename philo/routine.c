/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/20 12:19:40 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	take_forks(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->left_fork);
	log_action(philosopher, "has taken a fork", get_current_time_ms());
	pthread_mutex_lock(philosopher->right_fork);
	log_action(philosopher, "has taken a fork", get_current_time_ms());
	return (1);
}

void	put_forks(t_philosopher *philosopher)
{
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
}

int	eat(t_philosopher *philosopher)
{
	t_shared	*shared;
	long long	current_time;

	shared = philosopher->shared_data;
	if (is_simulation_end(shared))
		return (0);
	current_time = get_current_time_ms();
	pthread_mutex_lock(&philosopher->meal_lock);
	philosopher->last_meal_time = current_time;
	philosopher->meals_eaten++;
	pthread_mutex_unlock(&philosopher->meal_lock);
	log_action(philosopher, "is eating", current_time);
	smart_sleep(shared->time_to_eat, shared);
	return (1);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philosopher;
	t_shared		*shared;

	philosopher = (t_philosopher *)arg;
	shared = philosopher->shared_data;
	update_last_meal_time(philosopher);
	if (philosopher->id % 2 == 0)
		usleep(100);
	while (!is_simulation_end(shared))
	{
		log_action(philosopher, "is thinking", get_current_time_ms());
		if (is_simulation_end(shared) || !take_forks(philosopher))
			break ;
		if (is_simulation_end(shared) || !eat(philosopher))
		{
			put_forks(philosopher);
			break ;
		}
		put_forks(philosopher);
		log_action(philosopher, "is sleeping", get_current_time_ms());
		smart_sleep(shared->time_to_sleep, shared);
	}
	return (NULL);
}
