/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:42:17 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/12 13:50:13 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_control	*initialize_control(int num_philosophers)
{
	t_control	*control;
	
	control = malloc(sizeof(t_control));
	if (!control)
		return (NULL);
	control->number_of_philosophers = num_philosophers;
	control->active_group = 0;
	if (pthread_mutex_init(&control->group_mutex, NULL) != 0)
	{
		free(control);
		return (NULL);
	}
	control->forks = malloc(sizeof(pthread_mutex_t) * num_philosophers);
	if (!control->forks)
	{
		pthread_mutex_destroy(&control->group_mutex);
		free(control);
		return (NULL);
	}
	return (control);
}
int	initialize_forks(pthread_mutex_t *forks, int num_philosophers)
{
	int	i;

	i = 0;
	while (i < num_philosophers)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (i);
		i++;
	}
	return (0);
}