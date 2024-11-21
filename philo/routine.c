/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/21 12:57:17 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_philosopher *philo)
{
	if (!philo->shared->is_simulation_running)
		return;

	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		if (!philo->shared->is_simulation_running)
		{
			pthread_mutex_unlock(philo->right_fork);
			return;
		}
		log_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		if (!philo->shared->is_simulation_running)
		{
			pthread_mutex_unlock(philo->left_fork);
			return;
		}
		log_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
	}
	if (!philo->shared->is_simulation_running)
	{
		release_forks(philo);
		return;
	}
	log_action(philo, "has taken a fork");
}

void	release_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	eat(t_philosopher *philo)
{
	if (!philo->shared->is_simulation_running)
		return;
	pthread_mutex_lock(&philo->shared->monitor_lock);
	philo->last_meal_time = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->shared->monitor_lock);
	log_action(philo, "is eating");
	wait_or_exit(philo, philo->shared->time_to_eat);
}

void	sleep_and_think(t_philosopher *philo)
{
	log_action(philo, "is sleeping");
	wait_or_exit(philo, philo->shared->time_to_sleep);
	if (!philo->shared->is_simulation_running)
		return;
	log_action(philo, "is thinking");
}

void	*philosopher_routine(void *arg)
{
	t_philosopher *philo = (t_philosopher *)arg;

	while (philo->shared->is_simulation_running)
	{
		take_forks(philo);
		if (!philo->shared->is_simulation_running)
			break;
		eat(philo);
		release_forks(philo);
		if (!philo->shared->is_simulation_running)
			break;
		sleep_and_think(philo);
	}
	release_forks(philo);
	return (NULL);
}
