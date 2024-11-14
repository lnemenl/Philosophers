/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:54:11 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/14 17:54:19 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_control(t_control *control)
{
	int	i;

	if (control->forks)
	{
		i = 0;
		while (i < control->number_of_philosophers)
		{
			pthread_mutex_destroy(&control->forks[i]);
			i++;
		}
		pthread_mutex_destroy(&control->group_mutex);
		free(control->forks);
	}
	free(control);
}

void	free_simulation(t_simulation *simulation)
{
	if (simulation->philosophers)
		free(simulation->philosophers);
	free_control(simulation->control);
}

