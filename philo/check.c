/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:42:35 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/15 22:45:45 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	should_stop_simulation(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->control->control_mutex);
	if (philosopher->control->stop_simulation)
	{
		pthread_mutex_unlock(&philosopher->control->control_mutex);
		return (1);//stop simulation
	}
	pthread_mutex_unlock(&philosopher->control->control_mutex);
	return (0);//continue simulation
}
