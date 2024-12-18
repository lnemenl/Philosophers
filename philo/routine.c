/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/19 20:15:08 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	get_forks_order(t_philosopher *philosopher,
							pthread_mutex_t **first_fork,
							pthread_mutex_t **second_fork)
{
	int	left_fork_num;
	int	right_fork_num;

	left_fork_num = philosopher->id;
	right_fork_num = (philosopher->id % philosopher->shared_data->num_philosophers) + 1;

	if (left_fork_num < right_fork_num)
	{
		*first_fork = philosopher->left_fork;
		*second_fork = philosopher->right_fork;
	}
	else
	{
		*first_fork = philosopher->right_fork;
		*second_fork = philosopher->left_fork;
	}
}

int	take_forks(t_philosopher *philosopher)
{
	t_shared		*shared;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	shared = philosopher->shared_data;
	get_forks_order(philosopher, &first_fork, &second_fork);
	pthread_mutex_lock(first_fork);
	log_action(philosopher, "has taken a fork");
	if (is_simulation_end(shared))
	{
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	pthread_mutex_lock(second_fork);
	log_action(philosopher, "has taken a fork");
	if (is_simulation_end(shared))
	{
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(first_fork);
		return (0);
	}
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
	log_action(philosopher, "is eating");
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
		usleep(1000);  // Even philosophers wait 1ms at start
	while (!is_simulation_end(shared))
	{
		log_action(philosopher, "is thinking");
		if (philosopher->id == shared->num_philosophers)
			usleep(500);  // Last philosopher waits a bit longer
		if (is_simulation_end(shared) || !take_forks(philosopher))
			break;
		if (is_simulation_end(shared) || !eat(philosopher))
		{
			put_forks(philosopher);
			break;
		}
		put_forks(philosopher);
		if (is_simulation_end(shared))
			break;
		log_action(philosopher, "is sleeping");
		smart_sleep(shared->time_to_sleep, shared);
	}
	return (NULL);
}
