/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:45:59 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/15 23:06:49 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	handle_eating(t_philosopher *philosopher)
{
	// Odd philosophers pick right fork first, even philosophers pick left fork first
	if (philosopher->id % 2 == 0)
	{
		pthread_mutex_lock(philosopher->left_fork);
		printf("%ld Philosopher %d has taken a fork\n", get_current_time_ms(), philosopher->id);
		pthread_mutex_lock(philosopher->right_fork);
		printf("%ld Philosopher %d has taken a fork\n", get_current_time_ms(), philosopher->id);
	}
	else
	{
		pthread_mutex_lock(philosopher->right_fork);
		printf("%ld Philosopher %d has taken a fork\n", get_current_time_ms(), philosopher->id);
		pthread_mutex_lock(philosopher->left_fork);
		printf("%ld Philosopher %d has taken a fork\n", get_current_time_ms(), philosopher->id);
	}
	philosopher->last_meal_time = get_current_time_ms();
	printf("%ld Philosopher %d is eating\n", philosopher->last_meal_time, philosopher->id);
	sleep_ms(philosopher->control->time_to_eat);
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
}

void	handle_sleeping(t_philosopher *philosopher)
{
	printf("%ld Philosopher %d is sleeping\n", get_current_time_ms(), philosopher->id);
	sleep_ms(philosopher->control->time_to_sleep);
}
void	handle_thinking(t_philosopher *philosopher)
{
	printf("%ld Philosopher %d is thinking\n", get_current_time_ms(), philosopher->id);
	sleep_ms(5);
}
