/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:05 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/15 18:33:13 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*monitor_routine(void *data)
{
	t_simulation	*simulation;
	
	simulation = (t_simulation *)data;
	while (1)
	{
		if (check_stop_simulation(simulation))
			return (NULL);
		if (check_philosopher_status(simulation))
			return (NULL);
		sleep_ms(10);
	}
	return (NULL);
}

int	check_stop_simulation(t_simulation *simulation)
{
	pthread_mutex_lock(&simulation->control->control_mutex);
	if (simulation->control->stop_simulation)
	{
		pthread_mutex_unlock(&simulation->control->control_mutex);
		return (1);//Simulation should stop
	}
	pthread_mutex_unlock(&simulation->control->control_mutex);
	return (0);//Continue monitoring
}

int	check_philosopher_status(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->control->number_of_philosophers)
	{
		pthread_mutex_lock(&simulation->control->control_mutex);
		if (get_current_time_ms() - simulation->philosophers[i].last_meal_time > simulation->control->time_to_die)
		{
			printf("%ld Philosopher %d has died\n", get_current_time_ms(), simulation->philosophers[i].id);
			simulation->control->stop_simulation = 1;
			pthread_mutex_unlock(&simulation->control->control_mutex);
			return (1);
		}
		pthread_mutex_unlock(&simulation->control->control_mutex);
		i++;
	}
	return (0);
}