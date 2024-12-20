/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_additional.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 12:29:22 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/20 12:05:40 by rkhakimu         ###   ########.fr       */
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
