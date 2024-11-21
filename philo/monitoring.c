/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:05 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/21 12:56:00 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_starvation(t_philosopher *philosophers, t_shared *shared)
{
	int	i;

	i = 0;
	while (i < shared->num_philosophers)
	{
		pthread_mutex_lock(&shared->monitor_lock);
		if (get_current_time() - philosophers[i].last_meal_time > shared->time_to_die)
		{
			log_action(&philosophers[i], "died");
			shared->is_simulation_running = 0;
			pthread_mutex_unlock(&shared->monitor_lock);
			return (1);
		}
		pthread_mutex_unlock(&shared->monitor_lock);
		i++;
	}
	return (0);
}

int	check_meal_requirement(t_philosopher *philosophers, t_shared *shared)
{
	int	i;

	if (shared->meals_required == -1)
		return (0);
	i = 0;
	while (i < shared->num_philosophers)
	{
		if (philosophers[i].meals_eaten < shared->meals_required)
			return (0);
		i++;
	}
	shared->is_simulation_running = 0;
	return (1);
}

void	*monitor_health(void *arg)
{
	t_philosopher	*philosophers;
	t_shared		*shared;
	
	philosophers = (t_philosopher *)arg;
	shared = philosophers[0].shared;
	while (shared->is_simulation_running)
	{
		if (check_starvation(philosophers, shared))
		{
			pthread_mutex_lock(&shared->monitor_lock);
			shared->is_simulation_running = 0;
			pthread_mutex_unlock(&shared->monitor_lock);
			break;
		}
		usleep(500);
	}
	return (NULL);
}
