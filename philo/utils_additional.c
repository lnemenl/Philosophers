/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_additional.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 12:29:22 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/13 12:31:28 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_simulation_end(t_shared *shared)
{
	int	result;

	pthread_mutex_lock(&shared->simulation_end_lock);
	result = shared->simulation_end;
	pthread_mutex_unlock(&shared->simulation_end_lock);
	return (result);
}

void	set_simulation_end(t_shared *shared, int value)
{
	pthread_mutex_lock(&shared->simulation_end_lock);
	shared->simulation_end = value;
	pthread_mutex_unlock(&shared->simulation_end_lock);
}

void	update_last_meal_time(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->meal_lock);
	philosopher->last_meal_time = get_current_time_ms();
	pthread_mutex_unlock(&philosopher->meal_lock);
}
