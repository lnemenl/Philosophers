/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/12 18:00:44 by rkhakimu         ###   ########.fr       */
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
			usleep(200);//Value needs to be adjusted
			continue;
		}
		pthread_mutex_unlock(&philosopher->control->group_mutex);
		pthread_mutex_lock(philosopher->left_fork);
		pthread_mutex_lock(philosopher->right_fork);
		philosopher->last_meal_time = get_current_time_ms();
		printf("Philosopher %d is eating\n", philosopher->id);
		usleep(500000);//Value needs to be adjusted
		pthread_mutex_unlock(philosopher->left_fork);
		pthread_mutex_unlock(philosopher->right_fork);
		printf("Philosopher %d is thinking\n", philosopher->id);
		usleep(500000);//Value needs to be adjusted
		printf("Philosopher %d is sleeping\n", philosopher->id);
		usleep(500000);//Value needs to be adjusted
	}
	return (NULL);
}
