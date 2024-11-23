/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:43:43 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/23 20:17:26 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	start_threads(t_shared *shared, t_philosopher *philosophers, pthread_t **threads)
{
	int i;

	i = 0;
	*threads = malloc(sizeof(pthread_t) * (shared->num_philosophers + 1));
	if (!(*threads))
		return (1);
	while (i < shared->num_philosophers)
	{
		if (pthread_create(&(*threads)[i], NULL, philosopher_routine, &philosophers[i]) != 0)
		{
			printf("Error: Failed to create thread for philosopher %d\n", i + 1);
			shared->is_simulation_running = 0;
			return (1);
		}
		i++;
	}
	if (pthread_create(&(*threads)[i], NULL, monitor_health, philosophers) != 0)
	{
		printf("Error: Failed to create monitoring thread\n");
		shared->is_simulation_running = 0;
		return (1);
	}
	return (0);
}

void	join_threads(t_shared *shared, pthread_t *threads)
{
    int i;

	i = 0;
    while (i < shared->num_philosophers + 1)
	{
        pthread_join(threads[i], NULL);
        i++;
    }
}
